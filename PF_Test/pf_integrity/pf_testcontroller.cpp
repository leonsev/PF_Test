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
    suit_list.append(suit_);

    qRegisterMetaType<pf_errresult::pointer_t>("pf_errresult::pointer_t");

    connect(suit_list.last(), SIGNAL(error_result(pf_errresult::pointer_t)), this, SIGNAL(error_result(pf_errresult::pointer_t)), Qt::QueuedConnection);
    connect(suit_list.last(), SIGNAL(complete(complete_t)), this, SIGNAL(complete(complete_t)), Qt::QueuedConnection);
}

void pf_testcontroller::start()
{
    for(QList<pf_testsuit_p>::iterator it = suit_list.begin(); it < suit_list.end(); it++)
    {
        it[0]->start();
    }
}

void pf_testcontroller::stop()
{
    for(QList<pf_testsuit_p>::iterator it = suit_list.begin(); it < suit_list.end(); it++)
    {
        it[0]->quit();
    }
}

void pf_testcontroller::restart()
{
    for(QList<pf_testsuit_p>::iterator it = suit_list.begin(); it < suit_list.end(); it++)
    {
        it[0]->restart();
    }
}

