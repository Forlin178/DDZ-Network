#include "login.h"
#include "cards.h"
#include <QApplication>
#include <QDebug>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    QResource::registerResource("./resource.rcc");
    Login w;
    int ret = w.exec();
    if (ret == QDialog::Accepted)
        return a.exec();
}
