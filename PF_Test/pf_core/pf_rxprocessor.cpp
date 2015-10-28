#include <QByteArray>
#include <QDebug>

#include "pf_mcontroller.h"
#include "pf_rxprocessor.h"
#include "pf_retval.h"
#include "pf_crc.h"


pf_rxprocessor::pf_rxprocessor(size_t init_size) : data(), state(WAIT_START), it(0)
{
    data.reserve(init_size);
}

retval_t pf_rxprocessor::insert(const QByteArray& in)
{
    data.append(in);

    return process();
}

retval_t pf_rxprocessor::process(void)
{
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
                QDebug(QtDebugMsg) << "A new message received:" << data.left(it);

                if(0 == pf_crc::get(data.left(it)))
                {
                    //Return a new message
                    emit messageReceived(data.left(it));
                }
                else
                {
                    //Push useful data
                    QDebug(QtWarningMsg) << "Wrong CRC!!!";
                }

                //Reset the data beggin
                data.remove(0,it+1);
                it = 0;
            }
            else if(data[it] == START_CHAR)
            {
                //Push rubbish data
                QDebug(QtWarningMsg) << "Unexpected data on a line" << data.left(it+1);

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

    return RV_OK;
}

