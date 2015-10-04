#ifndef PF_TRANSMITTER_H
#define PF_TRANSMITTER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

#include "pf_receiver.h"

class pf_transmitter : public QObject
{
    Q_OBJECT
public:
    explicit pf_transmitter(QObject *parent = 0);
    virtual ~pf_transmitter();

signals:

public slots:
    void open_serial(QString, QString);
    void transmitt(QByteArray, bool);

private slots:
    void data_received();
    void timeout();

private: //members
    QSerialPort* tx_port;
    QSerialPort* rx_port;
    pf_receiver pf_rec;
    QByteArray current_request;
    QTimer *timer;
    const int echo_timeout;
    const int request_timeout;

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
