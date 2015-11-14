#include "pf_crc.h"

#define MAGIC_NUMBER 0x55

quint8 pf_crc::get(const QByteArray &data)
{
    quint8 crc = ((unsigned char)data[0] == START_CHAR) ? 0 : START_CHAR;
    for (int it = 0; it < data.size(); it++)
    {
        crc^=data[it];
    }

    return crc;
}

quint8 pf_crc::gen(const QByteArray &data)
{
    quint8 crc = get(data);

    if(START_CHAR == crc || END_CHAR == crc)
    {
        crc = MAGIC_NUMBER;
    }
    return crc;
}

quint8 pf_crc::chk(const QByteArray &data)
{
    return gen(data.left(data.size()-1)) ^ data[data.size()-1];
}
