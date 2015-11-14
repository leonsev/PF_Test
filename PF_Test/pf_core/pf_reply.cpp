#include "pf_reply.h"

pf_reply::pf_reply()
{

}

pf_reply::pf_reply(QByteArray& reply_, pf_reply::reply_t type_) :
    reply(reply_), request(), type(type_), delay(-1), timestamp(QTime::currentTime())
{

}

pf_reply::pf_reply(QByteArray &reply_, QByteArray &request_, qint32 delay_) :
reply(reply_), request(request_), type(pf_reply::TYPE_REQUEST), delay(delay_), timestamp(QTime::currentTime())
{

}

pf_reply::pf_reply(const pf_reply &reply_obj) :
    reply(reply_obj.reply), request(reply_obj.request),
    type(reply_obj.type), delay(reply_obj.delay), timestamp(reply_obj.timestamp)
{

}

const QString pf_reply::get_timestamp_str()
{
    return (timestamp.toString() + QString(" ") + QString::number(timestamp.msec()));
}

pf_reply::~pf_reply()
{
//    if(NULL != request)
//    {
//        delete(request);
//        request = NULL;
//    }
//    if(NULL != reply)
//    {
//        delete(reply);
//        reply = NULL;
//    }
//    if(NULL != timestamp)
//    {
//        delete(timestamp);
//        timestamp = NULL;
//    }
}

pf_reply pf_reply::operator =(const pf_reply &obj)
{
    return pf_reply(obj);
}

const QByteArray& pf_reply::get_request() const
{
    return request;
}

const QByteArray& pf_reply::get_reply() const
{
    return reply;
}

pf_reply::reply_t pf_reply::get_type() const
{
    return type;
}

qint32 pf_reply::get_delay() const
{
    return delay;
}

const QTime& pf_reply::get_timestamp() const
{
    return timestamp;
}
