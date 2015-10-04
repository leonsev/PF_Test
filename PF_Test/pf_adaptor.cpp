#include <QDebug>

#include "pf_adaptor.h"

pf_adaptor::pf_adaptor(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(open_serial(QString, QString)),
            &tx, SLOT(open_serial(QString, QString)));

    connect(this, SIGNAL(send_broadcast(QByteArray, bool)),
            &tx, SLOT(transmitt(QByteArray, bool)));

    //TODO Can work in existing thread
    tx.moveToThread(&tr_tx);

    tr_tx.setPriority(QThread::TimeCriticalPriority);

    tr_tx.start();
}

pf_adaptor::~pf_adaptor()
{
    QDebug(QtDebugMsg) << "~pf_adaptor()";
    tr_tx.terminate();
    tr_tx.wait(5000);
    if(tr_tx.isRunning())
    {
       QDebug(QtWarningMsg) << "Can't stop thread";
    }
}

void pf_adaptor::open(const QString &tx, const QString &rx)
{
    emit (open_serial(tx, rx));
}

