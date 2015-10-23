#ifndef PF_CYCLIC_REQUEST_H
#define PF_CYCLIC_REQUEST_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QPointer>


class pf_controller : public QObject
{
    Q_OBJECT
public:
    explicit pf_controller(QObject *parent = 0);
    virtual ~pf_controller();



private: //types

    struct request_t : QObject
    {
        request_t(QByteArray request_data_, quint32 period_,
                  quint32 pause_, bool wait_reply_);

        request_t(const pf_controller::request_t& obj );

        request_t operator=( const request_t& obj);

        void start_timer() {ptimer->setInterval(period); ptimer->setSingleShot(true); ptimer->start();}
        void stop_timer() {ptimer->stop();}
        bool is_timeout() {return (ptimer->remainingTime() == 0) || (ptimer->remainingTime() == -1);}

        ~request_t();

        QByteArray request_data;
        quint32 period;
        quint32 pause;
        bool wait_reply;
        QPointer<QTimer> ptimer;
    };

signals:
    void transmitt(QByteArray, bool);
public slots:
    void next_request();
    void start();
    void stop();
    void reset(void);
    void add(QByteArray request_sequence_, quint32 period_, quint32 pause_, bool wait_reply_);
    void single_request(QByteArray request_sequence_, bool request_, quint32 pause_ = 0);
private slots:
    void timeout();
    void pause_off(void);

private: //methods
    void try_send(QPointer<request_t> forse_request = NULL);

private: //members
    typedef enum
    {
        STOP = 0,
        RUN,
        PAUSE
    }state_t;

    QByteArray request_sequence;
    QByteArray single_request_m;
    bool single_request_type;
    quint32 period;
    QTimer* timer;
    state_t state;
    bool tx_ready;
    bool timeout_ready;

    typedef QList<request_t> requests_list_t;
    typedef QList< QPointer<request_t> > requests_queue_t;

    requests_list_t requests;
    requests_queue_t requests_queue;
};


#endif // PF_CYCLIC_REQUEST_H
