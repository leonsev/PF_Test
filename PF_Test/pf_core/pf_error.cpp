#include <QVector>
#include <QString>

#include "pf_error.h"

namespace
{
const QString err2txt_data[pf_error::ERR_TOTAL] =
{
    "ERR_NO",
    "ERR_UNEXPECTED_MESSAGE_RECEIVED",
    "ERR_UNEXPECTED_DATA_RECEIVED",
    "ERR_WRONG_CRC",
    "ERR_NO_ECHO",
    "ERR_NO_REPLY",
    "ERR_WRONG_ECHO"
};
}

pf_error::pf_error(pf_error::error_t err_no_): err_no(err_no_), time(QTime::currentTime())
{

}

pf_error::pf_error(const pf_error &obj) : err_no(obj.err_no), time(obj.time)
{

}

pf_error::~pf_error()
{

}

pf_error::error_t pf_error::get_err_no() const
{
    return err_no;
}

const QString &pf_error::get_txt()
{
    return err2txt(err_no);
}

const QTime pf_error::get_timestamp()
{
    return time;
}

const QString pf_error::get_timestamp_str()
{
    return (time.toString() + QString(" ") + QString::number(time.msec()));
}

const QString &pf_error::err2txt(pf_error::error_t err_no_)
{
    return err2txt_data[err_no_];
}


