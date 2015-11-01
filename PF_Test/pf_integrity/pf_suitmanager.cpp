#include <QVariant>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QDebug>

#include "variantdata.h"
#include "reader.h"
#include "writer.h"

#include "pf_suitmanager.h"


pf_suitmanager::pf_suitmanager(QObject *parent) : QObject(parent)
{

}

pf_testsuit_p pf_suitmanager::gen_suit(const QString &name_,
                                       const QByteArray &message_pattern_, const QByteArray &message_mask_,
                                       const QByteArray &noize_pattern_, quint32 mess_numb_, quint64 iterations_ )
{
    //QHash<QByteArray*, QPointer<QByteArray> > suit_map;
    message_suit_t message_suit;

    if(message_pattern_.size() != noize_pattern_.size() || message_pattern_.size() != message_mask_.size())
    {
        QDebug(QtWarningMsg) << "message_pattern_ size " << message_pattern_.size() << "isn't equal the noize_pattern_ size " << noize_pattern_.size();
    }
    else
    {
        quint64 max_tries = mess_numb_*100;
        while(mess_numb_ > 0)
        {
            QByteArray new_mess(message_pattern_);
            for(int i = 0; i < message_pattern_.size(); i++)
            {
                if(message_mask_[i] != 0)
                {
                    new_mess[i] = new_mess[i] | ((static_cast<char>(qrand()) & 0xFF) & message_mask_[i]);
                }
            }

            if(!message_suit.contains(new_mess))
            {
                //suit_map.insert(new_mess,new_mess);
                message_suit.append(new_mess);
                mess_numb_--;
            }

            if(max_tries > 0)
            {
                max_tries--;
            }
            else
            {
                QDebug(QtWarningMsg) << "Can't generate required messages for suit number";
                break;
            }
        }
    }

    return new pf_testsuit(
                name_,
                pf_noize(noize_pattern_),
                message_suit,
                iterations_
                );
}

void pf_suitmanager::export_suit(const pf_testsuit_p test_suit_)
{
    QtCSV::VariantData varData;

    varData.addRow("Iterations");
    varData.addRow(QString::number(test_suit_->getIterations()));
    varData.addEmptyRow();

    varData.addRow("Noize pattern");
    varData.addRow(QString(test_suit_->getNoize().getMask_pattern().toHex()));
    varData.addEmptyRow();

    varData.addRow("Messages suit");
    for(QList<pf_testsuit::suit_data_t>::const_iterator it = test_suit_->getSuit_data().begin();
        it < test_suit_->getSuit_data().end();
        it++)
    {
        varData.addRow(it[0].message.toHex());
    }

    QString filePath = QDir::currentPath() + "/" + test_suit_->getName() + "_suit.csv";

    QDebug(QtDebugMsg) << "Writing:" << filePath;

    if ( false == QtCSV::Writer::write(filePath, varData) )
    {
        QDebug(QtWarningMsg) << "Failed to write to a file:" << filePath;
    }

    QDebug(QtDebugMsg) << "Writing done";
}

pf_testsuit_p pf_suitmanager::import_suit(const QString &name_)
{
    QString filePath = QDir::currentPath() + "/" + name_ + "_suit.csv";

    QDebug(QtDebugMsg) << "Reading:" << filePath;

    QList<QStringList> readData = QtCSV::Reader::readToList(filePath);
    if(readData[0].at(0) != QString("Iterations"))
    {
        QDebug(QtWarningMsg) << "Failed to read a file:" << filePath;
    }

    message_suit_t message_suit;

    for (QList<QStringList>::const_iterator it = readData.begin() + 7; it < readData.end(); it++)
    {
        message_suit.append(QByteArray::fromHex(it[0].join(",").toLocal8Bit()));
    }

    quint64 iterations_ = readData.at(1).join(",").toDouble();
    QByteArray noize_pattern_(QByteArray::fromHex(readData.at(4).join(",").toLocal8Bit()));

    QDebug(QtWarningMsg) << "Importing:" << iterations_ << ":" << noize_pattern_ << ":"  << message_suit.at(0);

    QDebug(QtDebugMsg) << "Reading done";

    return new pf_testsuit(
                name_,
                pf_noize(noize_pattern_),
                message_suit,
                iterations_
                );
}

void pf_suitmanager::export_report(const QString &name_, const QList<pf_errresult::pointer_t> & report_list)
{
    QtCSV::VariantData varData;

    varData.addRow("Total errors");
    varData.addRow(QString::number(report_list.size()));

    varData.addEmptyRow();
    varData.addEmptyRow();

    QList<QVariant> header;
    header << "#" << "Original message" << "Noize" << "Corrupted message" << "CRC";
    varData << header;
    varData.addEmptyRow();


    quint64 num = 1;
    for(QList<pf_errresult::pointer_t>::const_iterator it = report_list.begin(); it < report_list.end(); it++)
    {
        QList<QVariant> report_line;

        report_line << num
                        << it[0]->message.toHex()
                        << it[0]->noize.toHex()
                        << it[0]->corrupt_message.toHex()
                        << it[0]->pf_crc8;
                        //<< it[0]->pf_crc8_fault
                        //<< it[0]->modb_crc8
                        //<< it[0]->modb_crc8_fault;

        varData.addRow(report_line);

        num++;
    }



    QString filePath = QDir::currentPath() + "/" + name_ + "_report.csv";

    QDebug(QtDebugMsg) << "Writing:" << filePath;

    if ( false == QtCSV::Writer::write(filePath, varData) )
    {
        QDebug(QtWarningMsg) << "Failed to write to a file:" << filePath;
    }
}

