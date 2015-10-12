#ifndef PF_TRANSMITTER_H
#define PF_TRANSMITTER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

#include "pf_receiver.h"
#include "pf_error.h"
#include "pf_reply.h"

class pf_transmitter : public QObject
{
    Q_OBJECT
public:
    explicit pf_transmitter(QObject *parent = 0);
    virtual ~pf_transmitter();


signals:
    //void reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ );
    void reply(pf_reply);
    void ready();
    void error(pf_error);
public slots:
    void open_serial(QString, QString, qint32 baud_rate = QSerialPort::Baud9600);
    void transmitt(QByteArray, bool);
    void close_port(void);

private slots:
    void data_received();
    void timeout();

private: //members
    QSerialPort* tx_port;
    QSerialPort* rx_port;
    pf_receiver pf_rec;
    QByteArray current_request;
    QTimer *timer;
    const int max_timeout;

    enum State_t
    {
        INIT = 0,
        READY,
        WAIT_ECHO_BROADCAST,
        WAIT_ECHO_REQUEST,
        WAIT_REPLY

    } state;

private: //methods
    void setState(State_t);
    State_t getState(void) const;

};

#endif // PF_TRANSMITTER_H
