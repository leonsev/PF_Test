#include "dialog.h"
#include <QApplication>
#include <QMetaType>

#include "pf_error.h"
#include "pf_reply.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<pf_error>("pf_error");
    qRegisterMetaType<pf_reply>("pf_reply");

    QApplication a(argc, argv);
    Dialog dialog;
    dialog.show();

    return a.exec();
}
