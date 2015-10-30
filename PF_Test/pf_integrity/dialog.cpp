#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    connect(&integ_ctrl, SIGNAL(complete(complete_t)),
            this, SLOT(pf_test_complete(complete_t)));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_start_clicked()
{
    ui->progressBar->setValue(0);

    integ_ctrl.start();
}

void Dialog::on_stop_clicked()
{
    integ_ctrl.stop();
}

void Dialog::on_add_clicked()
{
    //message_suit_p message_suit = new message_suit_t;

    message_suit_t message_suit;

    QByteArray noize_pattern_ = QByteArray::fromHex(ui->noize_pattern->text().toLocal8Bit());
    QByteArray message_ = QByteArray::fromHex(ui->message->text().toLocal8Bit());
    message_suit.append(message_);
    pf_testsuit_p test_suit = new pf_testsuit(
                pf_noize(noize_pattern_),
                message_suit,
                100
                );

    integ_ctrl.add_suit(test_suit);
}

void Dialog::pf_test_complete(complete_t done)
{
    //QDebug(QtDebugMsg) << "on_pf_test_complete " << done;
    ui->progressBar->setValue(done);
}
