#include <QDebug>

#include <stddef.h>

#include "pf_mcontroller.h"
#include "pf_retval.h"
#include "pf_crc.h"

pf_mcontroller:: pf_mcontroller(QString out, QString in):
    m_tx_port(new QSerialPort(out)),
    m_rx_port(out == in ? m_tx_port : new QSerialPort(in)),
    m_proc(),
    request_mutex()
{

    setup(*m_tx_port);

    if(m_tx_port != m_rx_port)
    {
        setup(*m_rx_port);
    }

    QDebug(QtDebugMsg) << "Opening port:";

    if(RV_OK == open())
    {
        QDebug(QtDebugMsg) << "Port is opened!!!";
        connect(m_rx_port, SIGNAL(readyRead()), this, SLOT(rxEvent()));
        connect(&m_proc, SIGNAL(messageReceived(QByteArray)), this, SLOT(telegramReceived(QByteArray)));
    }
    else
    {
        QDebug(QtWarningMsg) << "Can't open a port";
    }


}

pf_mcontroller::~pf_mcontroller()
{
    //finish the thread

    //delete serial ports instances
    if(m_tx_port == m_rx_port)
    {
        delete m_tx_port;
    }
    else
    {
        delete m_tx_port;
        delete m_rx_port;
    }
}

void pf_mcontroller::telegramReceived(QByteArray telegram)
{
    QDebug(QtDebugMsg) << "Telegram received:" << telegram;
    m_buf.add(telegram);
}

bool pf_mcontroller::setup (QSerialPort& port)
{
    //Set unlimited Rx buffer size
    port.setReadBufferSize(0);

    return
    port.setBaudRate(QSerialPort::Baud9600) ||
    port.setDataBits(QSerialPort::Data8) ||
    port.setParity(QSerialPort::NoParity) ||
    port.setStopBits(QSerialPort::OneStop) ||
    port.setFlowControl(QSerialPort::NoFlowControl);
}

retval_t pf_mcontroller::transmit (const QByteArray& data)
{
    retval_t rv = RV_OK;

    QByteArray mdata(data);

    mdata.append(pf_crc::get(mdata));

    mdata.prepend(START_CHAR);

    mdata.append(END_CHAR);

    //reserv buffer for echo;
    m_buf.reserv();

    QDebug(QtDebugMsg) << "Writing data:" << mdata;

    m_tx_port->write(mdata);

    QByteArray echodata;
    if(RV_OK == m_buf.acquire(echodata, 100))
    {
        if(mdata != echodata)
        {
            rv = RV_NO_ECHO;
            QDebug(QtWarningMsg) << "Echo isn't received. Wrong data:" << echodata;
        }
    }
    else
    {
        rv = RV_NO_ECHO;
        QDebug(QtWarningMsg) << "Echo isn't received. Timeout";
    }

    return rv;
}

retval_t pf_mcontroller::request(const QByteArray &request, QByteArray &reply)
{
    retval_t rv = RV_OK;
    QMutexLocker locker(&request_mutex);
    (void)reply;

    rv = transmit(request);

    return rv;
}

retval_t pf_mcontroller::broadcast(const QByteArray &request)
{
    retval_t rv = RV_OK;
    QMutexLocker locker(&request_mutex);

    rv = transmit(request);

    return rv;
}



retval_t pf_mcontroller::open (void)
{
    retval_t rv = RV_OK;

    do
    {
        if(m_tx_port == m_rx_port)
        {
            if(!m_tx_port->open(QIODevice::ReadWrite))
            {
                rv = RV_CANT_OPEN_PORT;
            }
        }
        else
        {
            if(!m_tx_port->open(QIODevice::WriteOnly))
            {
                rv = RV_CANT_OPEN_PORT;
            }
            else if(!m_rx_port->open(QIODevice::ReadOnly))
            {
                rv = RV_CANT_OPEN_PORT;
                m_tx_port->close();
            }
        }
    }while(0);


    return rv;
}


void pf_mcontroller::rxEvent()
{
    QByteArray inp_data(m_rx_port->readAll());
    m_proc.insert(inp_data);
}
