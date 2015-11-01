#ifndef PF_SUITMANAGER_H
#define PF_SUITMANAGER_H

#include <QObject>

#include "pf_testsuit.h"

class pf_suitmanager : public QObject
{
    Q_OBJECT
public:
    explicit pf_suitmanager(QObject *parent = 0);

signals:

public slots:

public: //methods
    static pf_testsuit_p gen_suit(const QString &name_,
                                  const QByteArray &message_pattern_, const QByteArray &message_mask_,
                                  const QByteArray &noize_pattern_, quint32 mess_numb_, quint64 iterations_);
    static void export_suit(const pf_testsuit_p test_suit_);
    static pf_testsuit_p import_suit(const QString &name_);
    static void export_report(const QString &name_, const QList<pf_errresult::pointer_t>&);
};

#endif // PF_SUITMANAGER_H
