#ifndef PF_NOIZE_H
#define PF_NOIZE_H

#include <QObject>
#include <QtGlobal>
#include <QPointer>
#include <QDebug>

class pf_noize_ins;

typedef QPointer<pf_noize_ins> noize_ins_p;

class pf_noize_ins : public QObject
{
    Q_OBJECT

    friend class pf_noize;

private:
    explicit pf_noize_ins(QObject *parent = 0);
    pf_noize_ins(QByteArray& noize_);
    pf_noize_ins(const pf_noize_ins&);

signals:

public slots:
    QByteArray apply_and(const QByteArray& message_);

private:
    const QByteArray noize;
};

class pf_noize : public QObject
{
    Q_OBJECT
public:
    //explicit pf_noize(QObject *parent = 0);
    pf_noize(const QByteArray& mask_pattern_);
    pf_noize(const pf_noize&);

signals:

public slots:
    noize_ins_p get();
    int size(void);

private: //methods
    QByteArray& generate(QByteArray&);

private: //members
    QByteArray mask_pattern;
};



#endif // PF_NOIZE_H
