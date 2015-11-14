#include "pf_rxbuffer.h"

pf_rxbuffer::pf_rxbuffer(int i) : m_buffer(), m_available(), m_reserved(0)
{
    if (i > 0)
    {
        reserv(i);
    }
}

retval_t pf_rxbuffer::acquire(QByteArray &out_data, unsigned long time)
{
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

retval_t pf_rxbuffer::add(const QByteArray& in_data)
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

retval_t pf_rxbuffer::reserv(int i)
{
    QMutexLocker locker(&m_mutex);

    m_reserved += i;
    return RV_OK;
}

retval_t pf_rxbuffer::dereserv(int i)
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

retval_t pf_rxbuffer::drop()
{
    QMutexLocker locker(&m_mutex);

    m_reserved = 0;
    while(m_available.tryAcquire());
    m_buffer.clear();

    return RV_OK;
}

pf_rxbuffer::~pf_rxbuffer()
{
    this->drop();
}



