#include <QDebug>

#include "pf_adaptor.h"

pf_adaptor::pf_adaptor(QObject *parent) : QObject(parent), tr_tx_p(new QThread()), tx_p(new pf_transmitter())
{
    connect(this, SIGNAL(open_serial_s(QString, QString, qint32)),
            tx_p, SLOT(open_serial(QString, QString, qint32)));

    connect(this, SIGNAL(request_s(QByteArray, bool)),
            tx_p, SLOT(transmitt(QByteArray, bool)));

    connect(tx_p, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
            this, SLOT(reply_sl(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )));

    //TODO Can work in existing thread
    tx_p->moveToThread(tr_tx_p);

    tr_tx_p->start();

    tr_tx_p->setPriority(QThread::TimeCriticalPriority);
}

pf_adaptor::~pf_adaptor()
{
    QDebug(QtDebugMsg) << "~pf_adaptor()";

    tx_p->close_port();
    delete(tx_p);

    QThread * tr = tr_tx_p->thread();
    tr_tx_p->terminate();
    tr_tx_p->wait(5000);
    if(tr_tx_p->isRunning())
    {
       QDebug(QtWarningMsg) << "Can't stop thread";
    }
    else
    {
        QDebug(QtDebugMsg) << "Thread stopped" << (void*)tr;
    }

    delete(tr_tx_p);
}

void pf_adaptor::open_serial_sl(const QString &tx, const QString &rx, const qint32 baud_rate)
{    
    emit (open_serial_s(tx, rx, baud_rate));
}


void pf_adaptor::request_sl (const QByteArray& data, const bool reply)
{
    emit (request_s(data, reply));
}

void pf_adaptor::reply_sl(QByteArray reply, QByteArray request, qint32 time)
{
    emit (reply_s(reply,request,time));
}

