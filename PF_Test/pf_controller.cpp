#include <QDebug>

#include "pf_controller.h"


pf_controller::pf_controller(QObject *parent) : QObject(parent),
    request_sequence(), single_request_m(), single_request_type(false),
    period(0), timer(NULL), state(INIT),
    tx_ready(true), timeout_ready(true)
{

}

pf_controller::~pf_controller()
{
    stop();
}

void pf_controller::next_request()
{
    //QDebug(QtDebugMsg) << "ready signal";
    tx_ready = true;
    if(READY == state)
    {
        try_send();
    }
}

void pf_controller::start()
{
    if (NULL == timer)
    {
        QDebug(QtDebugMsg) << "Create timer";
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    }
    state = pf_controller::READY;
    try_send();
}

void pf_controller::stop()
{
    state = pf_controller::STOP;
    if(NULL != timer)
    {
        QDebug(QtDebugMsg) << "cc delete timer";
        timer->stop();
        delete(timer);
        timer = NULL;
    }
}

void pf_controller::set(QByteArray request_sequence_, quint32 period_)
{
    QDebug(QtDebugMsg) << "Got cc request: Tr:" << this->thread();
    request_sequence = request_sequence_;
    period = period_;
    start();
}

void pf_controller::single_request(QByteArray request_sequence_, bool request_)
{
    single_request_m = request_sequence_;
    single_request_type = request_;
    try_send();
}

void pf_controller::timeout()
{
    //QDebug(QtDebugMsg) << "cc timeout";
    timeout_ready = true;
    timer->stop();
    if(READY == state)
    {
        try_send();
    }
}

void pf_controller::try_send()
{
    //QDebug(QtDebugMsg) << "Try send";
    if(true == tx_ready && true == timeout_ready)
    {
        tx_ready = false;
        if (0 != period) {timer->start(period); timeout_ready = false;}
        if(single_request_m.size() > 0)
        {
            emit(transmitt(single_request_m,single_request_type));
            single_request_m.clear();
        }
        else
        {
            emit(transmitt(request_sequence,true));
        }
    }
    else
    {
        //QDebug(QtDebugMsg) << "Not ready to send:" << tx_ready << timeout_ready;
    }
}

