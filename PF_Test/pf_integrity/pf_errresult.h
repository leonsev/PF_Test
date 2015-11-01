#ifndef PF_ERRRESULT_H
#define PF_ERRRESULT_H

#include <QObject>
#include <QPointer>

class pf_errresult : public QObject
{
    Q_OBJECT
public:
    typedef QPointer<pf_errresult> pointer_t;

    pf_errresult(const QByteArray &message_, const QByteArray &noize_, const QByteArray corrupt_message_,
                 quint8 pf_crc8_, bool pf_crc8_fault_, quint8 modb_crc8_, bool modb_crc8_fault_);

    pf_errresult(const pf_errresult&);

signals:

public slots:

public: //members
    const QByteArray message;
    const QByteArray noize;
    const QByteArray corrupt_message;
    const quint8 pf_crc8;
    const bool pf_crc8_fault;
    const quint8 modb_crc8;
    const bool modb_crc8_fault;
};

#endif // PF_ERRRESULT_H
