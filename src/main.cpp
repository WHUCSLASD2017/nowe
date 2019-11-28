#include "loginwindow.h"
#include"dataframe.h"
#include <QApplication>
#include"ChangeHeaderWnd.h"
#include "chatdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //LoginWindow w;
    MainWindow s;
    DataFrame d;
    //ChangeHeaderWnd p;
    ChatDialog chat;
    //p.show();
    //d.show();
    s.show();
    //w.show();
    chat.show();
    chat.setSender("Wuxuhui");
    chat.setReceiver("接受者昵称");
    chat.insertOutMessage("aaaaa");
    chat.insertInMessage("bbbbbb");
    chat.insertInMessage("我和我的祖国");
    chat.insertOutMessage("aaaaa");
    chat.insertInMessage("bbbbbb");
    chat.insertInMessage("我和我的祖国");
    chat.insertOutMessage("aaaaa");
    chat.insertInMessage("bbbbbb");
    chat.insertInMessage("我和我的祖国");
    chat.insertOutMessage("一刻也不能分割！");
    chat.setSignature("哥就是这么帅气！");
    QPixmap avatar;
    avatar.load("avatar.jpg");
    chat.setAvatar(avatar,80,80,45);
    return a.exec();
}
