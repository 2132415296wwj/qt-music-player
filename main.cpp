#include "loginregsitdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginRegsitDialog w;
    w.show();
    return a.exec();
}
