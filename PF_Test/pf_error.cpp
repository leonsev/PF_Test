#include <QVector>
#include <QString>

#include "pf_error.h"

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


pf_error::pf_error(pf_error::error_t err_no_): err_no(err_no_)
{

}

pf_error::pf_error(const pf_error &obj)
{
    this->err_no = obj.get_err_no();;
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

const QString &pf_error::err2txt(pf_error::error_t err_no_)
{
    return err2txt_data[err_no_];
}


