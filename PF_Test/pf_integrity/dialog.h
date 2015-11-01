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

    void on_error_result(QString, pf_errresult::pointer_t);

    void pf_test_complete(complete_t);

    void on_gen_suit_clicked();

    void on_import_suit_clicked();

    void on_export_csv_clicked();

private:
    Ui::Dialog *ui;

    pf_testcontroller integ_ctrl;
    QMap<QString, QList<pf_errresult::pointer_t> > err_res;

};

#endif // DIALOG_H
