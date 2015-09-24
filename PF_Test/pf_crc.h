#ifndef PF_CRC_H
#define PF_CRC_H

#include <stddef.h>
#include <QByteArray>

class pf_crc
{
public:
    static unsigned char get(const QByteArray& data);
};

#endif // PF_CRC_H
