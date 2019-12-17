#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "nowebasewindow.h"
#include <QMainWindow>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QXmppClient.h>
#include "groupmanager.h"
#include "addnewfriend.h"
#include <QXmppMucManager.h>
#include <QXmppBookmarkManager.h>
#include <QXmppBookmarkSet.h>
#include <QXmppDiscoveryManager.h>
#include <QXmppPubSubIq.h>
#include "groupmanager.h"
#include "addnewfriend.h"
#include "createroom.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWidget *setItem(QString mainTitle,QString iconAddr,QString subTitle,bool ifVIP=false,bool ifOnline=true);
    QMenu *menu;
    QMenu *addMenu;

    QWidget *createItem(QString jid, QString mainTitle, QString iconAddr, QString subTitle, bool ifVIP, bool ifOnline);
    QWidget *createRoomItem(QString mainTitle, QString iconAddr);

    QTreeWidgetItem *createFriendGroup(QString grpName);
    QTreeWidgetItem *addFriendtoGroup(QTreeWidgetItem *grp, QString mainTitle, QString subTitle, QString avatarAddr,QString jid);
    QTreeWidgetItem *removeFriendOrGroup(QTreeWidgetItem *toSet);
    QTreeWidgetItem *addFriendtoGroupAtTop(QTreeWidgetItem *grp, QString mainTitle, QString subTitle, QString avatarAddr,QString jid);
    QTreeWidgetItem *setFriendToTop(QTreeWidgetItem *toSet, QString mainTitle, QString subTitle, QString avatarAddr, QTreeWidgetItem *grp = nullptr, QString jid=nullptr);
    QTreeWidgetItem *createMessage(QString mainTitle, QString subTitle, QString avatarAddr,QString jid);

    QTreeWidgetItem *addRoom(QString roomName,QString avatarAddr);

    void createRoom(QString roomName);

    void createBookMark(QString markName);

    void setAvatar(QPixmap &avatar, int length, int width, int radius);
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    void setMainTitle(QString string);
    void setSubTitle(QString string);
    void setMenu();
    void setAddMenu();
    void displayAddPanel();
    void displayCreateRoomPanel();

    void displayProfilePanel();
    void displayAvatarChangePanel();
    GroupManager grpMng;

    void on_presenceChanged();
    void addFriend();
    void flushAllFriends();
    QString username;
    void showAddNewFriendPanel(QString jid);
private:
    Ui::MainWindow *ui;
    QXmppClient *const client;

    QXmppBookmarkManager *myBookMarkManager;
    QXmppMucManager *myRoomManager;
    bool loadDone=false;

public slots:
    void windowclosed();
    void updateAllFriends();
    void on_subscriptionReceived(const QString &bareJid);
    void on_messageReceived(const QXmppMessage &msg);
    void on_invitationReceived(const QString &roomJid, const QString &inviter, const QString &reason);
    void on_AddItemBtn_clicked();
private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

    void on_roomTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_friendTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_messageTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_messageTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_friendTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_roomTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_4_clicked();

    void on_clientVCardReceived();

    void on_rosterReceived();

    void on_roomReceived(const QXmppBookmarkSet &bookmarks);


    void setMucManager();



signals:
    void friendClicked(QString usrName);
    void msgClicked(QString usrName);
    void roomClicked(QString roomName);
};

#endif // MAINWINDOW_H
