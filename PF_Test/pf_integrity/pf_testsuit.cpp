#include "pf_testsuit.h"
#include "pf_crc.h"
#include "pf_crc8.h"
#include "pf_noize.h"

//pf_testsuit::pf_testsuit(QObject *parent) : QThread(parent)
//{

//}

pf_testsuit::pf_testsuit(const QString &name_, const pf_noize &noize_, const message_suit_t &suit_, quint64 iterations_) :
    noize(noize_), /*suit(suit_),*/ iterations(iterations_), iterations_counter(0), name(name_)
{

    QDebug(QtDebugMsg) << "pf_testsuit:" << name_ << noize_.getMask_pattern() << suit_.at(0) << iterations_ << QString::number(pf_crc::gen(suit_.at(0))) << suit_.size();

    for(message_suit_t::const_iterator it = suit_.begin(); it < suit_.end(); it++)
    {
        suit_data.append(suit_data_t(it[0],pf_crc::gen(it[0]),pf_crc8::gen(it[0])));

        //QDebug(QtDebugMsg) << "CRC:" << it[0] << QString::number(pf_crc::gen(it[0])) << QString(pf_crc::gen(it[0])) << pf_crc::gen(it[0]);
    }
}

//pf_testsuit::pf_testsuit(const pf_testsuit &obj) : QThread(0),
//    noize(obj.noize), suit(obj.suit), iterations(obj.iterations), iterations_counter(0), done(0)
//{

//}

void pf_testsuit::restart()
{
    iterations_counter =0;
    done = 0;
    start();
}

void pf_testsuit::run()
{
    iterations_counter=0;
    done=0;
    QSet<QByteArray> noize_set;
    while(iterations_counter < iterations)
    {
        noize_ins_p current_noize;
        do
        {
            current_noize = noize.get();
        }while(noize_set.contains(current_noize->get()));

        noize_set.insert(current_noize->get());

        for(QList<suit_data_t>::iterator it = suit_data.begin(); it < suit_data.end(); it++)
        {
            QByteArray corrupt_message(current_noize->apply_or(it[0].message));

            if(corrupt_message != it[0].message)
            {
                if(pf_crc::gen(corrupt_message) == it[0].pf_crc)
                {

                    //QDebug(QtDebugMsg) << "Message corruption isn't detected";
                    emit(error_result(QString(name+"_crcPF"),pf_errresult::pointer_t(
                                          new pf_errresult
                                          (
                                            it[0].message,
                                            current_noize->get(),
                                            corrupt_message,
                                            it[0].pf_crc,
                                            true,
                                            0,
                                            false
                            )
                            )));
                }

                if(pf_crc::gen(corrupt_message) == it[0].crc8)
                {

                    //QDebug(QtDebugMsg) << "Message corruption isn't detected";
                    emit(error_result(QString(name+"_crc8"),pf_errresult::pointer_t(
                                          new pf_errresult
                                          (
                                            it[0].message,
                                            current_noize->get(),
                                            corrupt_message,
                                            it[0].crc8,
                                            true,
                                            0,
                                            false
                            )
                            )));
                }
            }

            //QDebug(QtDebugMsg) << "Running testsuit:" << it[0].message << current_noize->noize <<  current_noize->get();
        }



        iterations_counter++;
        complete_t i = iterations_counter*100/iterations;

        if((i) > done)
        {
            done = i;
            //QDebug(QtDebugMsg) << "Complete " << done;
            emit(complete(done));
        }
    }
}

const QList<pf_testsuit::suit_data_t> &pf_testsuit::getSuit_data() const
{
    return suit_data;
}

QString pf_testsuit::getName() const
{
    return name;
}

quint64 pf_testsuit::getIterations() const
{
    return iterations;
}

pf_noize pf_testsuit::getNoize() const
{
    return noize;
}

