#ifndef PF_CONTROLLER_H
#define PF_CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QSerialPort>

#include "pf_transmitter.h"
#include "pf_receiver.h"
#include "pf_error.h"



class pf_adaptor : public QObject
{
    Q_OBJECT
public:
    explicit pf_adaptor(QObject *parent = 0);
    virtual ~pf_adaptor();

signals:
    /* in */
    void request_s(QByteArray, bool reply);
    void open_serial_s(QString, QString, qint32 /*baud_rate*/);
    /* out */
    void reply_s(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ );

public slots:
    void open_serial_sl(const QString&, const QString&, const qint32 /*baud_rate*/);
    void request_sl(const QByteArray&, const bool reply);

private slots:
    void reply_sl(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ );

private: //members
    QThread* tr_tx_p;
    pf_transmitter* tx_p;

};

#endif // PF_CONTROLLER_H
