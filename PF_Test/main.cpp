#include "dialog.h"
#include <QApplication>
#include <QMetaType>

#include "pf_error.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<pf_error>("pf_error");

    QApplication a(argc, argv);
    Dialog dialog;
    dialog.show();

    return a.exec();
}
