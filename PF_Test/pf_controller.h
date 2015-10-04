#ifndef PF_CONTROLLER_H
#define PF_CONTROLLER_H

#include <QObject>

class pf_controller : public QObject
{
    Q_OBJECT
public:
    explicit pf_controller(QObject *parent = 0);

signals:

public slots:
};

#endif // PF_CONTROLLER_H
