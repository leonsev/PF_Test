#ifndef PF_RECEIVER_H
#define PF_RECEIVER_H

#include <QObject>
#include <QSerialPort>

#include "pf_error.h"


class pf_receiver : public QObject
{
    Q_OBJECT
public:
    explicit pf_receiver(QObject *parent = 0);
    virtual ~pf_receiver();

     bool process(const QByteArray& in_data, QByteArray& telegram);
     bool is_empty(void);
signals:
     void error(pf_error);
public slots:

private: //members
    enum state_t
    {
        WAIT_START = 0,
        WAIT_END
    };

    QByteArray data;
    state_t state;
    size_t it;
};

#endif // PF_RECEIVER_H
