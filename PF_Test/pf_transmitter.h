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
    void send_broadcast(QByteArray);

private slots:
    void data_received();
    void timeout();

private: //methods
    void transmitt(QByteArray);

private: //members
    QSerialPort* tx_port;
    QSerialPort* rx_port;
    pf_receiver pf_rec;
    QByteArray current_request;
    QTimer *timer;

    enum State_t
    {
        INIT = 0,
        READY,
        WAIT_ECHO,

    } state;

};

#endif // PF_TRANSMITTER_H
