#include <QDebug>

#include "pf_adaptor.h"

pf_adaptor::pf_adaptor(QObject *parent) : QObject(parent),
    tr_tx_p(new QThread()), tr_cc_p(new QThread()), tx_p(new pf_transmitter()), controller_p(new pf_controller())
{
    connect(this, SIGNAL(open_serial(QString, QString, qint32)),
            tx_p, SLOT(open_serial(QString, QString, qint32)),  Qt::BlockingQueuedConnection);

    connect(this, SIGNAL(close_serial(void)),
            tx_p, SLOT(close_port(void)),  Qt::BlockingQueuedConnection);

//    connect(this, SIGNAL(request(QByteArray, bool)),
//            tx_p, SLOT(transmitt(QByteArray, bool)),  Qt::BlockingQueuedConnection);

    connect(this, SIGNAL(request(QByteArray, bool)),
            controller_p, SLOT(single_request(QByteArray, bool)),  Qt::BlockingQueuedConnection);

    connect(controller_p, SIGNAL(transmitt(QByteArray, bool)),
            tx_p, SLOT(transmitt(QByteArray, bool)),  Qt::BlockingQueuedConnection);

//    connect(tx_p, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
//            this, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )), Qt::QueuedConnection);

    connect(tx_p, SIGNAL(reply(pf_reply)),
            this, SIGNAL(reply(pf_reply)));

    connect(tx_p ,SIGNAL(error(pf_error)),
            this,SIGNAL(error(pf_error)), Qt::QueuedConnection);

    connect(tx_p ,SIGNAL(ready()),
            controller_p,SLOT(next_request()), Qt::QueuedConnection);

    connect(this ,SIGNAL(add_cyclic(QByteArray, quint32, quint32, bool)),
            controller_p,SLOT(add(QByteArray, quint32, quint32, bool)),  Qt::QueuedConnection);

    connect(this ,SIGNAL(stop_cyclic()),
            controller_p,SLOT(stop()),  Qt::BlockingQueuedConnection);

    connect(this ,SIGNAL(reset_cyclic()),
            controller_p,SLOT(reset()),  Qt::BlockingQueuedConnection);

    connect(this ,SIGNAL(start_cyclic()),
            controller_p,SLOT(start()),  Qt::BlockingQueuedConnection);

    //TODO Can work in existing thread
    tx_p->moveToThread(tr_tx_p);
    controller_p->moveToThread(tr_cc_p);

    tr_tx_p->start();
    tr_cc_p->start();

    //tr_cc_p->setPriority(QThread::HighPriority);
    tr_tx_p->setPriority(QThread::TimeCriticalPriority);
}

pf_adaptor::~pf_adaptor()
{
    QDebug(QtDebugMsg) << "~pf_adaptor()";

    emit(this->stop_cyclic());
    emit(this->close_serial());

    delete(tx_p);
    delete(controller_p);

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

    delete(tr_tx_p);
    delete(tr_cc_p);
}

