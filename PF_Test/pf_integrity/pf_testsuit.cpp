#include "pf_testsuit.h"
#include "pf_crc.h"
#include "pf_noize.h"

//pf_testsuit::pf_testsuit(QObject *parent) : QThread(parent)
//{

//}

pf_testsuit::pf_testsuit(const pf_noize &noize_, const message_suit_t &suit_, quint64 iterations_) :
    noize(noize_), suit(suit_), iterations(iterations_), iterations_counter(0)
{

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
    while(iterations_counter < iterations)
    {
        noize_ins_p cuttent_noize = noize.get();

        for(message_suit_t::iterator it = suit.begin(); it < suit.end(); it++)
        {
            QByteArray corrupt_message(cuttent_noize->apply_and(it[0]));

            if(pf_crc::chk(corrupt_message) == 0)
            {
                if(corrupt_message != it[0])
                {
                    QDebug(QtDebugMsg) << "Message corruption isn't detected";
                }
            }

            QDebug(QtDebugMsg) << "Running testsuit:" << it[0] << corrupt_message;
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

