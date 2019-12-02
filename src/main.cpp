#include "loginwindow.h"
#include"dataframe.h"
#include <QApplication>
#include"ChangeHeaderWnd.h"
#include "chatdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;

    w.show();

    return a.exec();
}
