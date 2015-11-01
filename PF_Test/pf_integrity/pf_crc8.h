#ifndef PF_CRC8_H
#define PF_CRC8_H

#include <QtCore>

//---------------------------------------------------------------------------

class  TCrc8s ;
extern TCrc8s Crc8s ;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class TCrc8s
{
public :

  enum { eRevCrc8s = 0x13030900 } ;

private :

  enum { eACC  = 0x0000 } ;

  static const quint8 cTblLo[0x10],
            cTblHi[0x10] ;

  quint8	Acc ;

public :

  quint32 Crc(void) const { return Acc ; }
  void	Ini(quint32 crc = 0) ;

  quint32 Add(quint8  x) ;
  quint32 Add(short x) { Add((quint8)x) ; return Add((quint8)(x >>  8)) ; }
  quint32 Add(quint16  x) { Add((quint8)x) ; return Add((quint8)(x >>  8)) ; }
  quint32 Add(int   x) { Add((quint16)x) ; return Add((quint16)(x >> 16)) ; }
  quint32 Add(quint32  x) { Add((quint16)x) ; return Add((quint16)(x >> 16)) ; }
  quint32 Add(long  x) { Add((quint16)x) ; return Add((quint16)(x >> 16)) ; }
  quint32 Add(quint64 x) { Add((quint16)x) ; return Add((quint16)(x >> 16)) ; }

  quint32 Add(const quint8 *p, quint32 cnt) ;
  quint32 Add(const quint8 *p, quint32 cnt, quint32 ini) ;

} ;
//---------------------------------------------------------------------------



class pf_crc8
{
public:
    static quint8 get(const QByteArray &data);
    static quint8 gen(const QByteArray &data){return get(data);}
};


#endif // PF_CRC8_H
