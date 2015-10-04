#ifndef PF_RECEIVER_H
#define PF_RECEIVER_H

#include <QObject>

class pf_receiver : public QObject
{
    Q_OBJECT
public:
    explicit pf_receiver(QObject *parent = 0);

signals:

public slots:
};

#endif // PF_RECEIVER_H
