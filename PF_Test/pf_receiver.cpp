#include <QByteArray>
#include <QDebug>

#include "pf_receiver.h"
#include "pf_retval.h"
#include "pf_crc.h"


pf_receiver::pf_receiver(QObject *parent) : QObject(parent), data(), state(WAIT_START), it(0)
{
    data.reserve(1024);
}

pf_receiver::~pf_receiver()
{
    QDebug(QtDebugMsg) << "~pf_receiver()";
}

bool pf_receiver::process(const QByteArray& in_data, QByteArray& telegram)
{
    data.append(in_data);

    while(it < static_cast<size_t>(data.size()))
    {
        switch(state)
        {
        case WAIT_START:
            if(data[it] == START_CHAR)
            {
                state = WAIT_END;

                //Push rubbish data
                if(it >= 1)
                {
                    QDebug(QtWarningMsg) << "Unexpected data on a line" << data.left(it);
                    emit(error(pf_error(pf_error::ERR_UNEXPECTED_DATA_RECEIVED)));
                }

                //Reset the data beggin
                data.remove(0,it+1);
                it = 0;
            }
            else
            {
                it++;
            }
            break;

        case WAIT_END:
            if(data[it] == END_CHAR)
            {
                state = WAIT_START;

                //Push useful data
                //QDebug(QtDebugMsg) << "A new message received:" << data.left(it);

                if(0 == pf_crc::get(data.left(it)))
                {
                    //Return a new message
                    telegram = data.left(it-1);
                    //Reset the data beggin
                    data.remove(0,it+1);
                    it = 0;
                    return true;
                }
                else
                {
                    //Push useful data
                    QDebug(QtWarningMsg) << "Wrong CRC!!!";                    
                    emit(error(pf_error(pf_error::ERR_WRONG_CRC)));
                }

                //Reset the data beggin
                data.remove(0,it+1);
                it = 0;
            }
            else if(data[it] == START_CHAR)
            {
                //Push rubbish data
                QDebug(QtWarningMsg) << "Unexpected data on a line" << data.left(it+1);
                emit(error(pf_error(pf_error::ERR_UNEXPECTED_DATA_RECEIVED)));

                //Reset the data beggin
                data.remove(0,it+1);
                it = 0;
            }
            else
            {
                it++;
            }
            break;

        default:
            break;
        }
    }


    return false;
}

bool pf_receiver::is_empty()
{
    return(it >= static_cast<size_t>(data.size()));
}
