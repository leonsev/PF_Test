#ifndef PF_ERRRESULT_H
#define PF_ERRRESULT_H

#include <QObject>
#include <QPointer>

class pf_errresult : public QObject
{
    Q_OBJECT
public:
    typedef QPointer<pf_errresult> pointer_t;

    explicit pf_errresult(QObject *parent = 0);

signals:

public slots:
};

#endif // PF_ERRRESULT_H
