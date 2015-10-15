#include <QDebug>
#include <QTimer>

#include "pf_transmitter.h"
#include "pf_crc.h"

namespace
{
bool setup_port (QSerialPort& port, qint32 baud_rate)
{
    //Set unlimited Rx buffer size
    port.setReadBufferSize(0);

    QDebug(QtDebugMsg) << "Setup port:" << port.portName() << "Baud rate:" << baud_rate;

    return
    port.setBaudRate(baud_rate) ||
    port.setDataBits(QSerialPort::Data8) ||
    port.setParity(QSerialPort::NoParity) ||
    port.setStopBits(QSerialPort::OneStop) ||
    port.setFlowControl(QSerialPort::NoFlowControl);
}
}

pf_transmitter::pf_transmitter(QObject *parent) : QObject(parent),
    tx_port(NULL), rx_port(NULL), timer(NULL), max_timeout(50),
    state(INIT)
{
    connect(&pf_rec,SIGNAL(error(pf_error)),this,SIGNAL(error(pf_error)), Qt::QueuedConnection);
}

pf_transmitter::~pf_transmitter()
{
    QDebug(QtDebugMsg) << "~pf_transmitter()";

    close_port();
}

void pf_transmitter::open_serial(QString tx_port_, QString rx_port_, qint32 baud_rate)
{
    if(NULL == timer)
    {
        timer = new QTimer;
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()), Qt::QueuedConnection);
    }

    if(NULL == tx_port)
    {
        QDebug(QtDebugMsg) << "Creating tx port:" << tx_port_;
        tx_port = new QSerialPort(tx_port_);
        if(!setup_port(*tx_port, baud_rate))
        {
            QDebug(QtWarningMsg) << "Can't setup port:" << tx_port->portName();
        }
    }
    else if(tx_port_ != tx_port->portName())
    {
        tx_port->close();
    }
    if(!tx_port->open(QIODevice::ReadWrite))
    {
        QDebug(QtWarningMsg) << "Can't open port:" << tx_port->portName();
    }

    if(tx_port_ == rx_port_)
    {
        QDebug(QtDebugMsg) << "Set rx to tx:" << tx_port_;
        rx_port = tx_port;
    }
    else
    {
        if(NULL == rx_port)
        {
            QDebug(QtDebugMsg) << "Creating rx port:" << rx_port_;
            rx_port = new QSerialPort(rx_port_);
            if(!setup_port(*rx_port, baud_rate))
            {
                QDebug(QtWarningMsg) << "Can't setup port:" << rx_port->portName();
            }
        }
        else if(rx_port_ != rx_port->portName())
        {
            rx_port->close();
        }
        if(!rx_port->open(QIODevice::ReadWrite))
        {
            QDebug(QtWarningMsg) << "Can't open port:" << tx_port->portName();
        }
    }

    if(NULL != rx_port && NULL != tx_port)
    {
        connect(rx_port, SIGNAL(readyRead(void)),
                this, SLOT(data_received(void)));
    }
    else
    {
        QDebug(QtWarningMsg) << "Can't opent port";
    }

    setState(READY);
    current_request.remove(0, current_request.size());

    return;
}

void pf_transmitter::transmitt(QByteArray tx_data, bool request)
{
    //QDebug(QtWarningMsg) << "pf_transmitter::transmitt thread" << this->thread();

    if(getState() == READY)
    {
        if(request)
        {
            setState(WAIT_ECHO_REQUEST);
        }
        else
        {
            setState(WAIT_ECHO_BROADCAST);
        }

        //TODO remove
        //setState(WAIT_REPLY);

        current_request = tx_data;

        tx_data.prepend(START_CHAR);

        tx_data.append(pf_crc::gen(tx_data));

        tx_data.append(END_CHAR);

        //TODO remove
//        tx_data.prepend(0xAA);
//        tx_data.prepend(0xFE);
//        tx_data.prepend(0xBB);

        //QDebug(QtDebugMsg) << "Writing data:" << tx_data;

        //tx_port->waitForBytesWritten(-1);

        tx_port->write(tx_data);

        //TODO remove
        //tx_port->waitForBytesWritten(-1);

        timer->start(max_timeout);

        //while(READY != getState() && timer->remainingTime() > 10) {data_received();}
    }
    else
    {
        QDebug(QtWarningMsg) << "Busy. State is:" << getState();
    }
}

void pf_transmitter::close_port()
{
    if(NULL != tx_port)
    {
        QDebug(QtDebugMsg) << "Closing:" << tx_port->portName();
        tx_port->close();
        delete(tx_port);
        if (tx_port == rx_port)
        {
            rx_port = NULL;
        }
        tx_port = NULL;
    }
    if(NULL != rx_port && tx_port != rx_port)
    {
        QDebug(QtDebugMsg) << "Closing:" << rx_port->portName();
        rx_port->close();
        delete(rx_port);
        rx_port = NULL;
    }
    if(NULL != timer)
    {
        timer->stop();
        delete(timer);
        timer = NULL;
    }
    setState(INIT);
}

void pf_transmitter::setState(pf_transmitter::State_t state_)
{
    state = state_;
    if(state == READY)
    {
        emit(ready());
    }
}

pf_transmitter::State_t pf_transmitter::getState() const
{
    return state;
}

void pf_transmitter::data_received()
{
    QByteArray telegram;
    do
    {
        //QDebug(QtDebugMsg) << "data_received()";
        if(pf_rec.process(rx_port->readAll(),telegram))
        {
            //QDebug(QtDebugMsg) << "got data";
            if (WAIT_ECHO_BROADCAST == getState() || WAIT_ECHO_REQUEST == getState())
            {
                if(telegram == current_request)
                {
                    timer->stop();
                    //QDebug(QtDebugMsg) << "Echo is received";

                    if(WAIT_ECHO_REQUEST == getState())
                    {
                        //in case of request message the new timeout for receive need to be activated
                        setState(WAIT_REPLY);
                        timer->start(max_timeout);
                    }
                    else
                    {
                        setState(READY);
                    }
                }
                else
                {
                    // TODO Emit error signal;
                    QDebug(QtWarningMsg) << "Incorrect echo received. Transmitt: " << current_request << "Echo: " << telegram;
                    emit(error(pf_error(pf_error::ERR_WRONG_ECHO)));
                }
            }
            else if (WAIT_REPLY == getState())
            {

                int reply_time = max_timeout - timer->remainingTime();

                /* Timeout workaround */

                int message_length = ((telegram.size() + 3/*start+stop+crc*/) * rx_port->baudRate())/QSerialPort::Baud9600;

                if(message_length >= reply_time) {reply_time=0;}
                else
                {
                    reply_time -=message_length;
                }

                /* Timeout workaround end */


                timer->stop();
                setState(READY);
                //QDebug(QtDebugMsg) << "Reply received. Request: " << current_request << "Reply: " << telegram << "Delay: " << reply_time;
                emit(reply(pf_reply(telegram, current_request, reply_time)));

            }
            else
            {
                QDebug(QtWarningMsg) << "Unexpected message is received";
                emit(error(pf_error(pf_error::ERR_UNEXPECTED_MESSAGE_RECEIVED)));
                setState(READY);
            }
        }
    }while(!pf_rec.is_empty());
}

void pf_transmitter::timeout()
{
    timer->stop();
    if (WAIT_ECHO_BROADCAST == getState() || WAIT_ECHO_REQUEST == getState())
    {
        QDebug(QtWarningMsg) << "Timeout. Echo isn't received";
        emit(error(pf_error(pf_error::ERR_NO_ECHO)));
    }
    else if (WAIT_REPLY == getState())
    {
        QDebug(QtWarningMsg) << "Timeout. Reply isn't received";
        emit(error(pf_error(pf_error::ERR_NO_REPLY)));
    }
    else
    {
        QDebug(QtWarningMsg) << "Must never get here";
    }
    setState(READY);
}

