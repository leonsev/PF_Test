#ifndef PF_CYCLIC_REQUEST_H
#define PF_CYCLIC_REQUEST_H

#include <QObject>
#include <QVector>
#include <QTimer>


class pf_controller : public QObject
{
    Q_OBJECT
public:
    explicit pf_controller(QObject *parent = 0);
    virtual ~pf_controller();

signals:
    void transmitt(QByteArray, bool);
public slots:
    void next_request();
    void start();
    void stop();
    void set(QByteArray request_sequence_, quint32 period_);
    void single_request(QByteArray request_sequence_, bool request_);
private slots:
    void timeout();

private: //methods
    void try_send(void);

private: //members
    typedef enum
    {
        INIT =0,
        READY,
        STOP
    }state_t;

    QByteArray request_sequence;
    QByteArray single_request_m;
    bool single_request_type;
    quint32 period;
    QTimer* timer;
    state_t state;
    bool tx_ready;
    bool timeout_ready;
};


#endif // PF_CYCLIC_REQUEST_H
