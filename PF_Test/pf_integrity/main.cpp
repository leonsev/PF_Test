#include "dialog.h"
#include <QApplication>
#include "pf_testcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<complete_t>("complete_t");
    qRegisterMetaType<pf_errresult::pointer_t>("pf_errresult::pointer_t");

    Dialog w;

    w.show();

    return a.exec();
}
