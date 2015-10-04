#include <QDebug>
#include <QTimer>

#include "pf_transmitter.h"
#include "pf_crc.h"

pf_transmitter::pf_transmitter(QObject *parent) : QObject(parent),
    tx_port(NULL), rx_port(NULL), timer(NULL),
    echo_timeout(500), request_timeout(500),
    state(INIT)
{

}

pf_transmitter::~pf_transmitter()
{
    QDebug(QtDebugMsg) << "~pf_transmitter()";

    if(NULL != tx_port)
    {
        QDebug(QtDebugMsg) << "Closing:" << tx_port->portName();
        tx_port->close();
        delete(tx_port);
    }
    if(NULL != rx_port && tx_port != rx_port)
    {
        QDebug(QtDebugMsg) << "Closing:" << rx_port->portName();
        rx_port->close();
        delete(rx_port);
        rx_port = NULL;
    }
    delete(timer);
}

void pf_transmitter::open_serial(QString tx_port_, QString rx_port_)
{
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    if(NULL == tx_port)
    {
        QDebug(QtDebugMsg) << "Creating tx port:" << tx_port_;
        tx_port = new QSerialPort(tx_port_);
    }
    else if(tx_port_ != tx_port->portName())
    {
        tx_port->close();
    }
    if(tx_port->open(QIODevice::ReadWrite))
    {
        QDebug(QtDebugMsg) << "Tx port is opened";
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
        }
        else if(rx_port_ != rx_port->portName())
        {
            rx_port->close();
        }
        if(rx_port->open(QIODevice::ReadWrite))
        {
            QDebug(QtDebugMsg) << "Rx port is opened";
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
    QDebug(QtWarningMsg) << "pf_transmitter::transmitt thread" << this->thread();

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

        tx_data.append(pf_crc::get(tx_data));

        tx_data.prepend(START_CHAR);

        tx_data.append(END_CHAR);

        QDebug(QtDebugMsg) << "Writing data:" << tx_data;

        tx_port->write(tx_data);

        //TODO remove
        tx_port->waitForBytesWritten(-1);

        timer->start(echo_timeout);
    }
    else
    {
        QDebug(QtWarningMsg) << "Busy. State is:" << getState();
    }
}

void pf_transmitter::setState(pf_transmitter::State_t state_)
{
    state = state_;
}

pf_transmitter::State_t pf_transmitter::getState() const
{
    return state;
}

void pf_transmitter::data_received()
{
    QByteArray telegram;
    if(pf_rec.process(rx_port->readAll(),telegram))
    {
        if (WAIT_ECHO_BROADCAST == getState() || WAIT_ECHO_REQUEST == getState())
        {
            if(telegram == current_request)
            {
                timer->stop();
                QDebug(QtDebugMsg) << "Echo is received";

                if(WAIT_ECHO_REQUEST == getState())
                {
                    //in case of request message the new timeout for receive need to be activated
                    setState(WAIT_REPLY);
                    timer->start(request_timeout);
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
            }
        }
        else if (WAIT_REPLY == getState())
        {

            int reply_time = request_timeout - timer->remainingTime();
            timer->stop();
            setState(READY);
            QDebug(QtDebugMsg) << "Reply received. Request: " << current_request << "Reply: " << telegram << "Delay: " << reply_time;
            //TODO Emit reply signal
        }
    }
}

void pf_transmitter::timeout()
{
    timer->stop();
    if (WAIT_ECHO_BROADCAST == getState() || WAIT_ECHO_REQUEST == getState())
    {
        QDebug(QtWarningMsg) << "Timeout. Echo isn't received";
    }
    else if (WAIT_REPLY == getState())
    {
        QDebug(QtWarningMsg) << "Timeout. Reply isn't received";
    }
    else
    {
        QDebug(QtWarningMsg) << "Must never get here";
    }
    setState(READY);
}

