#ifndef INVITEFRIEND_H
#define INVITEFRIEND_H

#include <QWidget>
#include <QXmppClient.h>
#include <QXmppMucManager.h>
#include <QMessageBox>
#include <QXmppBookmarkManager.h>
#include <QXmppBookmarkSet.h>
#include "nowebasewindow.h"
#include "group.h"

namespace Ui {
class InviteFriend;
}

class InviteFriend : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit InviteFriend(Group * myGroup, QWidget *parent = nullptr);
    ~InviteFriend();
    void inviteFriend(QString jid);



private slots:
    void on_cancelBtn_clicked();

    void on_inviteBtn_clicked();

private:
    Ui::InviteFriend *ui;
    QPoint last;
    QXmppClient *client;
    Group * thisGroup;
};

#endif // INVITEFRIEND_H
