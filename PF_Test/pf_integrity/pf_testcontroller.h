#ifndef PF_TESTCONTROLLER_H
#define PF_TESTCONTROLLER_H

#include <QObject>

#include "pf_testsuit.h"

class pf_testcontroller : public QObject
{
    Q_OBJECT
public:
    explicit pf_testcontroller(QObject *parent = 0);
    pf_testcontroller(const pf_testcontroller&);

    virtual ~pf_testcontroller();

    void add_suit(pf_testsuit_p suit_);
signals:
    complete(complete_t);
    error_result(pf_errresult::pointer_t);

public slots:
    void start();
    void stop();
    void restart();

private:
    QList<pf_testsuit_p> suit_list;
};

#endif // PF_TESTCONTROLLER_H
