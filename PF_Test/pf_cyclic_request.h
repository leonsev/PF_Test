#ifndef PF_CYCLIC_REQUEST_H
#define PF_CYCLIC_REQUEST_H

#include <QObject>
#include <QVector>
#include <QTimer>


class pf_cyclic_request : public QObject
{
    Q_OBJECT
public:
    explicit pf_cyclic_request(QObject *parent = 0);
    virtual ~pf_cyclic_request();

signals:
    void transmitt(QByteArray, bool);
public slots:
    void next_request();
    void start();
    void stop();
    void set(QByteArray request_sequence_, quint32 period_);
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
    quint32 period;
    QTimer* timer;
    state_t state;
    bool tx_ready;
    bool timeout_ready;
};


#endif // PF_CYCLIC_REQUEST_H
