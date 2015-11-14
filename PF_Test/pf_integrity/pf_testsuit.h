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
    struct suit_data_t
    {
        suit_data_t(const QByteArray& message_, quint8 pf_crc_, quint8 modb_crc8_) :
            message(message_), pf_crc(pf_crc_), crc8(modb_crc8_) {}
        QByteArray message;
        quint8 pf_crc;
        quint8 crc8;
    };
public:

    //explicit pf_testsuit(QObject *parent = 0);
    pf_testsuit(const QString& name_, const pf_noize&, const message_suit_t&, quint64 iterations_);

    pf_noize getNoize() const;

    quint64 getIterations() const;

    QString getName() const;

    const QList<pf_testsuit::suit_data_t>& getSuit_data() const;

private:
    //pf_testsuit(const pf_testsuit&obj);

signals:
    error_result(QString suit_name,pf_errresult::pointer_t);
    complete(complete_t);

public slots:
//    void start();
//    void stop();
    void restart();

private:
    void run();

private: //members
    pf_noize noize;
    quint64 iterations;
    quint64 iterations_counter;
    complete_t done;
    QString name;
    QList<suit_data_t> suit_data;
};


#endif // PF_TESTSUIT_H
