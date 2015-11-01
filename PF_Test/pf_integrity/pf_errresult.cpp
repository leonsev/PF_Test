#include "pf_errresult.h"


pf_errresult::pf_errresult(const QByteArray &message_, const QByteArray &noize_, const QByteArray corrupt_message_, quint8 pf_crc8_, bool pf_crc8_fault_, quint8 modb_crc8_, bool modb_crc8_fault_):
    message(message_), noize(noize_), corrupt_message(corrupt_message_), pf_crc8(pf_crc8_), pf_crc8_fault(pf_crc8_fault_), modb_crc8(modb_crc8_), modb_crc8_fault(modb_crc8_fault_)
{

}

