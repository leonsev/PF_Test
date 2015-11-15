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
#include "pf_controller.h"
#include "pf_reply.h"



class pf_adaptor : public QObject
{
    Q_OBJECT
public:
    explicit pf_adaptor(QObject *parent = 0);
    virtual ~pf_adaptor();

signals:
    /* in */
    void request(QByteArray, bool reply, quint32 pause);
    void open_serial(QString, QString, qint32 /*baud_rate*/);    
    void close_serial(void);

    void start_cyclic();
    void reset_cyclic();
    void add_cyclic(QByteArray, quint32 /*period*/ , quint32 /*pause*/, bool /*wait_reply*/);
    void stop_cyclic();


    /* out */
//    void reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ );
    void reply(pf_reply);
    void error(pf_error);

public slots:

private: //members
    QThread* tr_tx_p;
    QThread* tr_cc_p;
    pf_transmitter* tx_p;
    pf_controller* controller_p;

};

#endif // PF_CONTROLLER_H
