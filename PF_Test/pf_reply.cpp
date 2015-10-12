#include "pf_reply.h"

pf_reply::pf_reply()
{

}

pf_reply::pf_reply(QByteArray& reply_, pf_reply::reply_t type_) :
    reply(new QByteArray(reply_)), request(NULL), type(type_), delay(-1), timestamp(new QTime(QTime::currentTime()))
{

}

pf_reply::pf_reply(QByteArray &reply_, QByteArray &request_, qint32 delay_) :
reply(new QByteArray(reply_)), request(new QByteArray(request_)), type(pf_reply::TYPE_REQUEST), delay(delay_), timestamp(new QTime(QTime::currentTime()))
{

}

pf_reply::pf_reply(const pf_reply &reply_obj) :
    reply(new QByteArray(*reply_obj.reply)), request(new QByteArray(*reply_obj.request)),
    type(reply_obj.type), delay(reply_obj.delay), timestamp(new QTime(*reply_obj.timestamp))
{

}

pf_reply::~pf_reply()
{
    if(NULL != request)
    {
        delete(request);
        request = NULL;
    }
    if(NULL != reply)
    {
        delete(reply);
        reply = NULL;
    }
    if(NULL != timestamp)
    {
        delete(timestamp);
        timestamp = NULL;
    }
}

QByteArray& pf_reply::get_request() const
{
    return *request;
}

QByteArray& pf_reply::get_reply() const
{
    return *reply;
}

pf_reply::reply_t pf_reply::get_type() const
{
    return type;
}

qint32 pf_reply::get_delay() const
{
    return delay;
}

QTime& pf_reply::get_timestamp() const
{
    return *timestamp;
}
