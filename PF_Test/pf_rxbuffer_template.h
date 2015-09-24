#ifndef PF_RXBUFFER_H
#define PF_RXBUFFER_H

#include <QObject>
#include <QSemaphore>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include <limits.h>


#include "pf_retval.h"

template <class T>
class pf_rxbuffer : public QObject
{
    Q_OBJECT

public:
    pf_rxbuffer();

    retval_t acquire(T &out_data, unsigned long time = ULONG_MAX);
    retval_t add(T& in_data);
    retval_t reserv (int i = 1);
    retval_t dereserv (int i = 1);
    retval_t drop ();

    virtual ~pf_rxbuffer();

private:
    QMutex m_mutex;
    QQueue<T> m_buffer;
    QSemaphore m_available;
    int m_reserved;
};


#endif // PF_RXBUFFER_H
