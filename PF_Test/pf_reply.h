#ifndef PF_REPLY_H
#define PF_REPLY_H

#include <QObject>
#include <QTime>

class pf_reply : public QObject
{
    Q_OBJECT
public:
    typedef enum
    {
        TYPE_BROADCAST = 0,
        TYPE_REQUEST
    }reply_t;

    explicit pf_reply();
    pf_reply(QByteArray& , pf_reply::reply_t type_ = pf_reply::TYPE_BROADCAST);
    pf_reply(QByteArray& reply_, QByteArray& request_, qint32 delay_);
    pf_reply(const pf_reply&);
    virtual ~pf_reply();
    pf_reply operator =(const pf_reply&);



    void set_request(QByteArray& request_);
    void set_delay(qint32 delay_);

    QByteArray& get_request(void) const;
    QByteArray& get_reply(void) const;
    pf_reply::reply_t get_type(void) const;
    qint32 get_delay(void) const;
    QTime& get_timestamp(void) const;


signals:

public slots:

private: // members
    QByteArray* reply;
    QByteArray* request;
    pf_reply::reply_t type;
    qint32 delay;
    QTime* timestamp;
};

#endif // PF_REPLY_H
