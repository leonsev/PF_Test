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
#include "pf_cyclic_request.h"
#include "pf_reply.h"



class pf_adaptor : public QObject
{
    Q_OBJECT
public:
    explicit pf_adaptor(QObject *parent = 0);
    virtual ~pf_adaptor();

signals:
    /* in */
    void request(QByteArray, bool reply);
    void open_serial(QString, QString, qint32 /*baud_rate*/);    
    void close_serial(void);

    void cyclic_request(QByteArray, quint32 /*period*/);
    void cyclic_stop();


    /* out */
//    void reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ );
    void reply(pf_reply);
    void error(pf_error);

public slots:

private: //members
    QThread* tr_tx_p;
    QThread* tr_cc_p;
    pf_transmitter* tx_p;
    pf_cyclic_request* cc_req;

};

#endif // PF_CONTROLLER_H
