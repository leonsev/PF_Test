#include <QDebug>

#include "pf_adaptor.h"

pf_adaptor::pf_adaptor(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(open_serial(QString, QString, qint32)),
            &tx, SLOT(open_serial(QString, QString, qint32)));

    connect(this, SIGNAL(request(QByteArray, bool)),
            &tx, SLOT(transmitt(QByteArray, bool)));

    //TODO Can work in existing thread
    tx.moveToThread(&tr_tx);

    tr_tx.start();

    tr_tx.setPriority(QThread::TimeCriticalPriority);
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

void pf_adaptor::open(const QString &tx, const QString &rx, qint32 baud_rate)
{    
    emit (open_serial(tx, rx, baud_rate));
}

