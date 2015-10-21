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
    reset();
}

void pf_controller::next_request()
{
    //QDebug(QtDebugMsg) << "ready signal";
    tx_ready = true;
    if(RUN == state)
    {
        try_send();
    }
}

void pf_controller::start()
{
//    if (NULL == timer)
//    {
//        //QDebug(QtDebugMsg) << "Create timer";
//        timer = new QTimer();
//        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
//    }
//    state = pf_controller::READY;

    stop();
    for(requests_list_t::iterator it = requests.begin(); it < requests.end(); it++)
    {
        it->start_timer();
    }
    state = RUN;
    try_send();

}

void pf_controller::stop()
{
//    state = pf_controller::STOP;
//    if(NULL != timer)
//    {
//        //QDebug(QtDebugMsg) << "cc delete timer";
//        timer->stop();
//        delete(timer);
//        timer = NULL;
//    }


    for(requests_list_t::iterator it = requests.begin(); it < requests.end(); it++)
    {
        it->stop_timer();
    }
    //clean requests queue;
    requests_queue.clear();
    state = STOP;
}


void pf_controller::add(QByteArray request_sequence_, quint32 period_, quint32 pause_, bool wait_reply_)
{
    bool done = false;
    int i = 0;
    pf_controller::request_t request(request_sequence_, period_, pause_, wait_reply_);
    for(requests_list_t::iterator it = requests.begin(); it < requests.end(); it++)
    {
        if(it->period <= period_)
        {
            requests.insert(i,request);
            QDebug(QtWarningMsg) << "Insert in:" << i;
            done = true;
            break;
        }
        i++;
    }
    if(!done)
    {
        i = requests.size();
        requests.insert(i, request);
        QDebug(QtWarningMsg) << "Insert in2:" << i << requests[i].ptimer;
    }    
    connect(requests[i].ptimer, SIGNAL(timeout()), this, SLOT(timeout()));

    if(RUN == state)
    {
        requests[i].start_timer();
    }


//    for(requests_list_t::iterator it = requests.begin(); it < requests.end(); it++)
//    {
//        connect(it->ptimer, SIGNAL(timeout()), this, SLOT(timeout());
//    }
    start();
}

void pf_controller::reset(void)
{
    stop();
    for(requests_list_t::iterator it = requests.begin(); it < requests.end(); it++)
    {
        disconnect(it->ptimer, SIGNAL(timeout()), this, SLOT(timeout()));
    }
    requests.clear();
}


void pf_controller::single_request(QByteArray request_sequence_, bool request_, quint32 pause_)
{
//    single_request_m = request_sequence_;
//    single_request_type = request_;
//    try_send();
}

void pf_controller::timeout()
{
//    //QDebug(QtDebugMsg) << "cc timeout";
//    timeout_ready = true;
//    timer->stop();
//    if(READY == state)
//    {
//        try_send();
//    }

    if(RUN == state)
    {

        //When timeout occures we try to push_back request in a request queue.
        //The request will be added in a queue only if there is no similar request in a queue
        bool timeout_source_found = false;
        for(requests_list_t::iterator request_it = requests.begin(); request_it < requests.end(); request_it++)
        {
            if(request_it->is_timeout())
            {
                bool request_is_in_a_queue = false;
                //The request is added in a queue only if there is no same pending request in a queue
                for(requests_queue_t::iterator rq_it = requests_queue.begin(); rq_it < requests_queue.end(); rq_it++)
                {
                    if(rq_it[0] == &(request_it[0]))
                    {
                        request_is_in_a_queue = true;
                        break;
                    }
                }
                if(!request_is_in_a_queue)
                {
                    requests_queue.push_back(&(request_it[0]));
                }
                // Restart timer
                request_it->start_timer();
                timeout_source_found = true;
            }
        }

        if(timeout_source_found)
        {
            try_send();
        }
        else
        {
            QDebug(QtWarningMsg) << "No reason for timeout";
        }
    }
}


void pf_controller::try_send()
{
//    //QDebug(QtDebugMsg) << "Try send";
//    if(true == tx_ready && true == timeout_ready)
//    {
//        tx_ready = false;
//        if (0 != period) {timer->start(period); timeout_ready = false;}
//        if(single_request_m.size() > 0)
//        {
//            emit(transmitt(single_request_m,single_request_type));
//            single_request_m.clear();
//        }
//        else
//        {
//            emit(transmitt(request_sequence,true));
//        }
//    }
//    else
//    {
//        //QDebug(QtDebugMsg) << "Not ready to send:" << tx_ready << timeout_ready;
//    }


    if(RUN == state && true == tx_ready && !requests_queue.isEmpty())
    {
        if(NULL != requests_queue[0])
        {
            emit(transmitt(requests_queue[0]->request_data,requests_queue[0]->wait_reply));
            requests_queue.removeFirst();
            tx_ready = false;
        }
        else
        {
            QDebug(QtWarningMsg) << "Null pointer in try_send";
        }
    }
}


pf_controller::request_t::request_t(QByteArray request_data_, quint32 period_, quint32 pause_, bool wait_reply_) :
    request_data(request_data_), period(period_ < 5 ? 5 : period_),
    pause(pause_), wait_reply(wait_reply_), ptimer(new QTimer())
{
    QDebug(QtDebugMsg) << "Constr request_t" << ptimer;
}

pf_controller::request_t::request_t(const pf_controller::request_t &obj):
    request_data(obj.request_data), period(obj.period),
    pause(obj.pause), wait_reply(obj.wait_reply), ptimer(obj.ptimer)
{
    QDebug(QtDebugMsg) << "Copy constr request_t" << this->ptimer;
}

pf_controller::request_t pf_controller::request_t::operator=(const pf_controller::request_t &obj)
{
    QDebug(QtDebugMsg) << "Copy = request_t";
    return request_t(obj);
}
