#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "pf_testcontroller.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:

    void on_start_clicked();

    void on_stop_clicked();

    void on_add_clicked();

    void pf_test_complete(complete_t);

private:
    Ui::Dialog *ui;

    pf_testcontroller integ_ctrl;
};

#endif // DIALOG_H
