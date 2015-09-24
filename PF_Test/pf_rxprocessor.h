#ifndef PF_RXPROCESSOR_H
#define PF_RXPROCESSOR_H

#include <QObject>

#include "pf_retval.h"

class pf_rxprocessor : public QObject
{
    Q_OBJECT
public:
    explicit pf_rxprocessor(size_t inti_size = 1024);
    retval_t insert (const QByteArray& inbyte);

signals:
    void messageReceived(QByteArray message);

private: //members
    enum state_t
    {
        WAIT_START = 0,
        WAIT_END
    };

    QByteArray data;
    state_t state;
    size_t it;

private: //methods
    retval_t process (void);
};

#endif // PF_RXPROCESSOR_H
