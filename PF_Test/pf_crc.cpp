#include "pf_crc.h"

unsigned char pf_crc::get(const QByteArray &data)
{
    unsigned char crc = 0;
    for (size_t it = 0; it < data.size(); it++)
    {
        crc^=data[it];
    }

    if(START_CHAR == crc || END_CHAR == crc)
    {
        crc = 0x55;
    }

    return crc;
}
