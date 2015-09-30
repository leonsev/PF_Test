#ifndef PF_RXBUFFER_H
#define PF_RXBUFFER_H

#include <QSemaphore>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include <limits.h>


#include "pf_retval.h"

class pf_rxbuffer
{

public:
    pf_rxbuffer(int i = 0);

    retval_t acquire(QByteArray &out_data, unsigned long time = ULONG_MAX);
    retval_t add(const QByteArray& in_data);
    retval_t reserv (int i = 1);
    retval_t dereserv (int i = 1);
    retval_t drop ();

    virtual ~pf_rxbuffer();

private:
    QMutex m_mutex;
    QQueue<QByteArray> m_buffer;
    QSemaphore m_available;
    int m_reserved;
};


#endif // PF_RXBUFFER_H
