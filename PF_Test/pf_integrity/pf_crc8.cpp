#include "pf_crc8.h"

//---------------------------------------------------------------------------



//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//---------------------------------------------------------------------------
/* Table for CRC-8 polynomial 0x31,	x^8 + x^5 + x^4 + 1	*/

const quint8 TCrc8s::cTblLo[0x10] = {
  0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
  0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
} ;

const quint8 TCrc8s::cTblHi[0x10] = {
  0x00, 0x9d, 0x23, 0xbe, 0x46, 0xdb, 0x65, 0xf8,
  0x8c, 0x11, 0xaf, 0x32, 0xca, 0x57, 0xe9, 0x74
} ;
//---------------------------------------------------------------------------

void TCrc8s::Ini(quint32 crc)
{
  Acc  = crc ;
}
//---------------------------------------------------------------------------

quint32 TCrc8s::Add(quint8 x)
{
  quint32 i = Acc ^ x ;
  return   Acc = cTblLo[i & 0x0f] ^ cTblHi[i >> 4 & 0x00ff] ;
}
//---------------------------------------------------------------------------

quint32 TCrc8s::Add(const quint8 *p, quint32 cnt)
{
  while (cnt--) Add(*p++) ;
  return Crc() ;
}
//---------------------------------------------------------------------------

quint32 TCrc8s::Add(const quint8 *p, quint32 cnt, quint32 ini)
{
  Ini(ini) ;
  return Add(p, cnt) ;
}
//---------------------------------------------------------------------------


quint8 pf_crc8::get(const QByteArray &data)
{
    TCrc8s Crc8s;
    return (quint8)(0xFF & Crc8s.Add(reinterpret_cast<const quint8*>(data.data()), data.size(), 0));
}
