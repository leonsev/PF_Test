#include "pf_rxbuffer.h"

template <class T>
pf_rxbuffer<T>::pf_rxbuffer() : m_buffer(), m_available(), m_reserved(0)
{

}

template <class T>
retval_t pf_rxbuffer<T>::acquire(T &out_data, unsigned long time)
{
    QMutexLocker locker(&m_mutex);
    if(m_available.tryAcquire(1, time))
    {
        out_data = m_buffer.dequeue();
        return RV_OK;
    }
    else
    {
        return RV_TIMEOUT;
    }

}

template <class T>
retval_t pf_rxbuffer<T>::add(T& in_data)
{
    QMutexLocker locker(&m_mutex);

    if(m_reserved > 0)
    {
        m_buffer.enqueue(in_data);
        m_reserved--;
        m_available.release(1);
        return RV_OK;
    }
    else
    {
        return RV_CANT_INSERT_DATA;
    }
}

template <class T>
retval_t pf_rxbuffer<T>::reserv(int i)
{
    QMutexLocker locker(&m_mutex);

    m_reserved += i;
    return RV_OK;
}

template <class T>
retval_t pf_rxbuffer<T>::dereserv(int i)
{
    QMutexLocker locker(&m_mutex);

    if(m_reserved > i)
    {
        m_reserved -= i;
    }
    else
    {
        m_reserved = 0;
    }
    return RV_OK;
}

template <class T>
retval_t pf_rxbuffer<T>::drop()
{
    QMutexLocker locker(&m_mutex);

    m_reserved = 0;
    while(m_available.tryAcquire());
    m_buffer.clear();

    return RV_OK;
}

template <class T>
pf_rxbuffer<T>::~pf_rxbuffer()
{
    this->drop();
}



