#include "pf_noize.h"

pf_noize_ins::pf_noize_ins(QObject *parent) : QObject(parent)
{

}

pf_noize_ins::pf_noize_ins(QByteArray &noize_) : noize(noize_)
{

}

QByteArray pf_noize_ins::apply_or(const QByteArray &message_)
{
    QByteArray corrupted_message(message_);
    if(message_.size() == noize.size())
    {
        for(int n = 0; n < message_.size(); n++)
        {
            corrupted_message[n] = corrupted_message[n] | noize[n];
        }
        return corrupted_message;
    }
    QDebug(QtWarningMsg) << "Message and noize different size:" << message_ << ":" << noize.size();
    return QByteArray();
}

QByteArray pf_noize_ins::apply_and_not(const QByteArray &message_)
{
    QByteArray corrupted_message(message_);
    if(message_.size() == noize.size())
    {
        for(int n = 0; n < message_.size(); n++)
        {
            corrupted_message[n] = corrupted_message[n] & ~noize[n];
        }
        return corrupted_message;
    }
    QDebug(QtWarningMsg) << "Message and noize different size:" << message_ << ":" << noize.size();
    return QByteArray();
}


//pf_noize::pf_noize(QObject *parent) : QObject(parent)
//{

//}

pf_noize::pf_noize(const QByteArray& mask_pattern_) : mask_pattern(mask_pattern_)
{

}

pf_noize::pf_noize(const pf_noize & obj) : QObject(0), mask_pattern(obj.mask_pattern)
{

}

QByteArray& pf_noize::generate(QByteArray& noize_)
{
    for(int i = 0; i < mask_pattern.size(); i++)
    {
        noize_[i] = (static_cast<char>(qrand()) & 0xFF) & mask_pattern[i];
    }
    return noize_;
}

const QByteArray &pf_noize::getMask_pattern() const
{
    return mask_pattern;
}

noize_ins_p pf_noize::get()
{
    QByteArray noize_(mask_pattern.size(), Qt::Uninitialized);

    generate(noize_);

    //QDebug(QtDebugMsg) << "Generate noize:" << noize_;

    return new pf_noize_ins(noize_);
}

int pf_noize::size(){return mask_pattern.size();}

