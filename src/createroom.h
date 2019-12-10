#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QWidget>
#include <QXmppClient.h>
#include <QXmppMucManager.h>
#include <QMessageBox>
#include <QXmppBookmarkManager.h>
#include <QXmppBookmarkSet.h>
#include "nowebasewindow.h"

namespace Ui {
class CreateRoom;
}

class CreateRoom : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit CreateRoom(QXmppClient *client,QWidget *parent = nullptr);
    ~CreateRoom();
    void createRoom(QString roomName);
    void createBookMark( QString markName);



private slots:
    void on_cancelBtn_clicked();

    void on_createBtn_clicked();

private:
    Ui::CreateRoom *ui;
    QPoint last;
    QXmppClient *client;
};

#endif // CREATEROOM_H
