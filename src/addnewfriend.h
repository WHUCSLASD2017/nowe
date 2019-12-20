#ifndef ADDNEWFRIEND_H
#define ADDNEWFRIEND_H

#include <QDialog>
#include <QXmppClient.h>
#include "nowebasewindow.h"
#include "groups.h"

namespace Ui {
class AddNewFriend;
}

class AddNewFriend : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit AddNewFriend(QXmppClient *client,QWidget *parent = nullptr);
    ~AddNewFriend();
    void setLineEditContent(QString jid);
    void addRoom(QString RoomName);

private:
    Ui::AddNewFriend *ui;
    QPoint last;
    QXmppClient *client;
    QString roomName;
    bool firstAdd = true;   //防止由于出发信号对一个房间多次加入

private slots:
    void on_cancelButton_clicked();
    void on_confirmButton_clicked();
    void on_addFrBtn_clicked();
    void on_addRoomBtn_clicked();
    void on_cancelButton_3_clicked();
    void on_confirmButton_3_clicked();
    void judgeAddRoom(const QXmppDiscoveryIq & iq);
    void test(const QXmppDiscoveryIq & iq);

};

#endif // ADDNEWFRIEND_H
