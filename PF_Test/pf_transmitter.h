#ifndef PF_TRANSMITTER_H
#define PF_TRANSMITTER_H

#include <QObject>

class pf_transmitter : public QObject
{
    Q_OBJECT
public:
    explicit pf_transmitter(QObject *parent = 0);

signals:

public slots:
};

#endif // PF_TRANSMITTER_H
