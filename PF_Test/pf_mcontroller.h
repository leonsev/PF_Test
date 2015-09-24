#ifndef PF_MCONTROLLER_H
#define PF_MCONTROLLER_H

#include <QMutex>
#include <QSemaphore>
#include <QThread>
#include <QtSerialPort/QSerialPort>

#include "pf_retval.h"
#include "pf_rxprocessor.h"
#include "pf_rxbuffer.h"

#define START_CHAR (unsigned char)(0xFE)
#define END_CHAR (unsigned char)(0xFF)

class pf_mcontroller : public QObject
{
    Q_OBJECT
public:
    pf_mcontroller(QString out_port, QString in_port);


    retval_t request (const QByteArray& request, QByteArray& reply);
    retval_t broadcast (const QByteArray& request);

    ~pf_mcontroller();

signals:

public slots:

private slots:
    void telegramReceived (QByteArray telegram);
    void rxEvent();

private: //members

    QSerialPort* const m_tx_port;
    QSerialPort* const m_rx_port;
    pf_rxprocessor m_proc;
    QMutex request_mutex;
    pf_rxbuffer m_buf;


private: //methods

    retval_t open (void);
    bool setup (QSerialPort& port);
    retval_t transmit (const QByteArray& data);


};

#endif // PF_MCONTROLLER_H
