#ifndef pf_error_H
#define pf_error_H

#include <QObject>
#include <QTime>

class pf_error : public QObject
{
    Q_OBJECT

public:
    typedef enum
    {
        ERR_NO = 0, // No error
        ERR_UNEXPECTED_MESSAGE_RECEIVED,
        ERR_UNEXPECTED_DATA_RECEIVED,
        ERR_WRONG_CRC,
        ERR_NO_ECHO,
        ERR_NO_REPLY,
        ERR_WRONG_ECHO,

        ERR_TOTAL
    }error_t;

    pf_error(pf_error::error_t err_no_ = pf_error::ERR_NO);
    pf_error(const pf_error&);
    virtual ~pf_error();

signals:

public slots:

public: //methods
    error_t get_err_no(void) const;
    const QString& get_txt(void);
    const QTime get_timestamp();
    const QString get_timestamp_str();

private: //methods
    const QString& err2txt(pf_error::error_t);

private: //members
    error_t err_no;
    QTime time;
};

#endif // pf_error_H
