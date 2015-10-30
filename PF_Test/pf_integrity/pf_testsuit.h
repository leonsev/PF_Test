#ifndef PF_TESTSUIT_H
#define PF_TESTSUIT_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QDebug>

#include "pf_errresult.h"
#include "pf_noize.h"

typedef int complete_t;
typedef QList<QByteArray> message_suit_t;
typedef QPointer<message_suit_t> message_suit_p;

class pf_testsuit;
class pf_noize;

typedef QPointer<pf_testsuit> pf_testsuit_p;

class pf_testsuit : public QThread
{
    Q_OBJECT
public:
    //explicit pf_testsuit(QObject *parent = 0);
    pf_testsuit(const pf_noize&, const message_suit_t&, quint64 iterations_);
private:
    //pf_testsuit(const pf_testsuit&obj);

signals:
    error_result(pf_errresult::pointer_t);
    complete(complete_t);

public slots:
//    void start();
//    void stop();
    void restart();

private:
    void run();

private: //members
    pf_noize noize;
    message_suit_t suit;
    quint64 iterations;
    quint64 iterations_counter;
    complete_t done;
};


#endif // PF_TESTSUIT_H
