#include <QApplication>
#include "loginwindow.h"
#include "mainwindow.h"
#include "notificationpanel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QXmppClient client;

    NotificationPanel k;
    k.show();

    LoginWindow w;
    MainWindow m;

    if (w.exec() == QDialog::Accepted) {
        // 登录成功，则转到主窗口
        m.show();
        return a.exec();
    } else {
        return 0;
    }
}
