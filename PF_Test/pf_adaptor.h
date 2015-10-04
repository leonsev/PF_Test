#ifndef PF_CONTROLLER_H
#define PF_CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QByteArray>

#include "pf_transmitter.h"
#include "pf_receiver.h"



class pf_adaptor : public QObject
{
    Q_OBJECT
public:
    explicit pf_adaptor(QObject *parent = 0);
    virtual ~pf_adaptor();
    void open(const QString&, const QString&);

signals:
    void open_serial(QString, QString);
    void send_broadcast(QByteArray, bool reply);

public slots:

private: //members
    QThread tr_tx;

    pf_transmitter tx;

};

#endif // PF_CONTROLLER_H
