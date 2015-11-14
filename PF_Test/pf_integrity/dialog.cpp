#include "dialog.h"
#include "ui_dialog.h"
#include "pf_suitmanager.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(&integ_ctrl, SIGNAL(complete(complete_t)),
            this, SLOT(pf_test_complete(complete_t)));

    connect(&integ_ctrl, SIGNAL(error_result(QString, pf_errresult::pointer_t)),
            this, SLOT(on_error_result(QString, pf_errresult::pointer_t)));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_start_clicked()
{
    ui->progressBar->setValue(0);
    ui->err_detected->setText(QString::number(0));
    err_res.clear();

    integ_ctrl.start();
}

void Dialog::on_stop_clicked()
{
    integ_ctrl.stop();
}

//void Dialog::on_add_clicked()
//{
//    //message_suit_p message_suit = new message_suit_t;

////    message_suit_t message_suit;

//    QByteArray noize_pattern_ = QByteArray::fromHex(ui->noize_pattern->text().toLocal8Bit());
//    QByteArray message_patterm = QByteArray::fromHex(ui->message->text().toLocal8Bit());
//    QString test_name(ui->test_suit_name->text());

//    if(noize_pattern_.size() == message_patterm.size())
//    {
//        integ_ctrl.add_suit(pf_suitmanager::gen_suit(test_name, message_patterm, noize_pattern_, 100, 100));
//    }
//    else
//    {
//        QDebug(QtWarningMsg) << "Message size " << message_patterm.size() << "isn't equal the noize size " << noize_pattern_.size();
//    }
//}

void Dialog::on_error_result(QString name_, pf_errresult::pointer_t res_)
{
    err_res[name_].append(res_);

    ui->err_detected->setText(QString::number(ui->err_detected->text().toLong() + 1));

    QString status_;
    foreach(QString key, err_res.keys())
    {
        status_ += key + ": " + QString::number(err_res.value(key).size()) + "\r\n";
    }
    ui->err_detected->setText(status_);

    //QDebug(QtDebugMsg) << "on_error_result " << name_ << ":" << err_res[name_].last()->message << ":" << err_res[name_].last()->noize;
}

void Dialog::pf_test_complete(complete_t done)
{
    //QDebug(QtDebugMsg) << "on_pf_test_complete " << done;
    ui->progressBar->setValue(done);
}

void Dialog::on_gen_suit_clicked()
{

    QByteArray noize_pattern_ = QByteArray::fromHex(ui->noize_pattern->text().toLocal8Bit());
    QByteArray message_patterm = QByteArray::fromHex(ui->message->text().toLocal8Bit());
    QByteArray message_mask = QByteArray::fromHex(ui->message_mask->text().toLocal8Bit());
    QString test_name(ui->test_suit_name->text());

    if(noize_pattern_.size() == message_patterm.size())
    {
        pf_suitmanager::export_suit(pf_suitmanager::gen_suit(test_name,
                                                             message_patterm,
                                                             message_mask,
                                                             noize_pattern_,
                                                             ui->suit_size->text().toInt(),
                                                             ui->iterations->text().toLong()));

        on_import_suit_clicked();
    }
    else
    {
        QDebug(QtWarningMsg) << "Message size " << message_patterm.size() << "isn't equal the noize size " << noize_pattern_.size();
    }
}

void Dialog::on_import_suit_clicked()
{    
    integ_ctrl.add_suit(pf_suitmanager::import_suit(ui->test_suit_name->text()));
}

void Dialog::on_export_csv_clicked()
{
    if(err_res.size() == 0)
    {
        QDebug(QtDebugMsg) << "Report is empty";
    }
    else
    {
        foreach(QString key, err_res.keys())
        {
            pf_suitmanager::export_report(key, err_res.value(key));
        }
    }
}
