#include <QApplication>
#include "loginwindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QXmppClient client;

    LoginWindow w;
    MainWindow m;

    if (w.exec() == QDialog::Accepted) {
        m.show();
        return a.exec();
    } else {
        return 0;
    }
}
