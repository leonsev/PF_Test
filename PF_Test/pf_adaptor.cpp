#include <QDebug>

#include "pf_adaptor.h"

pf_adaptor::pf_adaptor(QObject *parent) : QObject(parent),
    tr_tx_p(new QThread()), tr_cc_p(new QThread()), tx_p(new pf_transmitter()), cc_req(new pf_cyclic_request())
{
    connect(this, SIGNAL(open_serial(QString, QString, qint32)),
            tx_p, SLOT(open_serial(QString, QString, qint32)));

    connect(this, SIGNAL(request(QByteArray, bool)),
            tx_p, SLOT(transmitt(QByteArray, bool)));

    connect(cc_req, SIGNAL(transmitt(QByteArray, bool)),
            tx_p, SLOT(transmitt(QByteArray, bool)));

    connect(tx_p, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
            this, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )));

    connect(tx_p ,SIGNAL(error(pf_error)),
            this,SIGNAL(error(pf_error)));

    connect(tx_p ,SIGNAL(ready()),
            cc_req,SLOT(next_request()));

    connect(this ,SIGNAL(cyclic_request(QByteArray, quint32)),
            cc_req,SLOT(set(QByteArray, quint32)));

    connect(this ,SIGNAL(cyclic_stop()),
            cc_req,SLOT(stop()));

    //TODO Can work in existing thread
    tx_p->moveToThread(tr_tx_p);
    cc_req->moveToThread(tr_cc_p);

    tr_tx_p->start();
    tr_cc_p->start();

    tr_tx_p->setPriority(QThread::TimeCriticalPriority);
}

pf_adaptor::~pf_adaptor()
{
    QDebug(QtDebugMsg) << "~pf_adaptor()";


    cc_req->stop();

    tx_p->close_port();

    QThread * tr_cc = tr_cc_p->thread();
    tr_cc_p->terminate();
    QThread * tr = tr_tx_p->thread();
    tr_tx_p->terminate();
    tr_cc_p->wait(1000);
    if(tr_cc_p->isRunning())
    {
       QDebug(QtWarningMsg) << "Can't stop thread";
    }
    else
    {
        QDebug(QtDebugMsg) << "Thread stopped" << (void*)tr_cc;
    }

    tr_tx_p->wait(1000);
    if(tr_tx_p->isRunning())
    {
       QDebug(QtWarningMsg) << "Can't stop thread";
    }
    else
    {
        QDebug(QtDebugMsg) << "Thread stopped" << (void*)tr;
    }


    delete(tx_p);
    delete(cc_req);
    delete(tr_tx_p);
    delete(tr_cc_p);
}

//void pf_adaptor::open_serial_sl(const QString &tx, const QString &rx, const qint32 baud_rate)
//{
//    emit (open_serial_s(tx, rx, baud_rate));
//}


//void pf_adaptor::request_sl (const QByteArray& data, const bool reply)
//{
//    emit (request_s(data, reply));
//}

