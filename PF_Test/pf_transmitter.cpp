#include <QDebug>
#include <QTimer>

#include "pf_transmitter.h"
#include "pf_crc.h"

pf_transmitter::pf_transmitter(QObject *parent) : QObject(parent), tx_port(NULL), rx_port(NULL), timer(NULL), state(INIT)
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

    state = READY;
    current_request.remove(0, current_request.size());

    return;
}

void pf_transmitter::send_broadcast(QByteArray tx_data)
{
    if(state == READY)
    {
        transmitt(tx_data);
    }
    else
    {
        QDebug(QtWarningMsg) << "Busy. State is:" << state;
    }
}

void pf_transmitter::transmitt(QByteArray tx_data)
{
    current_request = tx_data;

    tx_data.append(pf_crc::get(tx_data));

    tx_data.prepend(START_CHAR);

    tx_data.append(END_CHAR);

    QDebug(QtDebugMsg) << "Writing data:" << tx_data;

    tx_port->write(tx_data);

    state = WAIT_ECHO;

    timer->start(1000);

    return;
}

void pf_transmitter::data_received()
{
    QByteArray telegram;
    if(pf_rec.process(rx_port->readAll(),telegram))
    {
        QDebug(QtDebugMsg) << "Got telegram:" << telegram;

        if (WAIT_ECHO == state)
        {
            if(telegram == current_request)
            {
                timer->stop();
                state = READY;
                QDebug(QtDebugMsg) << "Echo is received";
            }
        }
    }
}

void pf_transmitter::timeout()
{
    timer->stop();
    if (WAIT_ECHO == state)
    {
        QDebug(QtWarningMsg) << "Echo isn't received";
        state = READY;
    }
    else
    {
        QDebug(QtWarningMsg) << "Must never get here";
    }
}

