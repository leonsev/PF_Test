#include <QDebug>

#include "pf_cyclic_request.h"


pf_cyclic_request::pf_cyclic_request(QObject *parent) : QObject(parent),
    request_sequence(), period(0), timer(NULL), state(INIT),
    tx_ready(true), timeout_ready(true)
{

}

pf_cyclic_request::~pf_cyclic_request()
{
    stop();
}

void pf_cyclic_request::next_request()
{
    QDebug(QtDebugMsg) << "ready signal";
    tx_ready = true;
    try_send();
}

void pf_cyclic_request::start()
{
    if (NULL == timer)
    {
        QDebug(QtDebugMsg) << "Create timer";
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    }
    state = pf_cyclic_request::READY;
    try_send();
}

void pf_cyclic_request::stop()
{
    state = pf_cyclic_request::STOP;
    if(NULL != timer)
    {
        QDebug(QtDebugMsg) << "cc delete timer";
        timer->stop();
        delete(timer);
        timer = NULL;
    }
}

void pf_cyclic_request::set(QByteArray request_sequence_, quint32 period_)
{
    QDebug(QtDebugMsg) << "Got cc request: Tr:" << this->thread();
    request_sequence = request_sequence_;
    period = period_;
    start();
}

void pf_cyclic_request::timeout()
{
    QDebug(QtDebugMsg) << "cc timeout";
    timeout_ready = true;
    timer->stop();
    try_send();
}

void pf_cyclic_request::try_send()
{
    QDebug(QtDebugMsg) << "Try send";
    if(true == tx_ready && true == timeout_ready && READY == state)
    {
        tx_ready = false;
        if (0 != period) {timer->start(period); timeout_ready = false;}
        emit(transmitt(request_sequence,true));
    }
    else
    {
        QDebug(QtDebugMsg) << "Not ready to send:" << tx_ready << timeout_ready;
    }
}

