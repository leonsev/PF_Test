#include "pf_testcontroller.h"

pf_testcontroller::pf_testcontroller(QObject *parent) : QObject(parent)
{

}

pf_testcontroller::~pf_testcontroller()
{
    stop();
}

void pf_testcontroller::add_suit(pf_testsuit_p suit_)
{
    if(!active_suit.isNull())
    {
        disconnect(active_suit, SIGNAL(error_result(QString, pf_errresult::pointer_t)), this, SIGNAL(error_result(QString,pf_errresult::pointer_t)));
        disconnect(active_suit, SIGNAL(complete(complete_t)), this, SIGNAL(complete(complete_t)));

        stop();
        active_suit.clear();
    }
    active_suit = suit_;

    //qRegisterMetaType<pf_errresult::pointer_t>("pf_errresult::pointer_t");

    connect(active_suit, SIGNAL(error_result(QString, pf_errresult::pointer_t)), this, SIGNAL(error_result(QString,pf_errresult::pointer_t)), Qt::QueuedConnection);
    connect(active_suit, SIGNAL(complete(complete_t)), this, SIGNAL(complete(complete_t)), Qt::QueuedConnection);
}

void pf_testcontroller::start()
{
    active_suit->start();
}

void pf_testcontroller::stop()
{
    if(!active_suit.isNull())
    {
        active_suit->terminate();
        active_suit->wait();
    }
}


