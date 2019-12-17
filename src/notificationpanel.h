#ifndef NOTIFICATIONPANEL_H
#define NOTIFICATIONPANEL_H

#include <QWidget>
#include "nowebasewindow.h"
#include <QXmppClient.h>
#include <QXmppMessage.h>

#include "NoweGlobal.h"

namespace Ui {
class NotificationPanel;
}

class NotificationPanel : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit NotificationPanel(QWidget *parent = nullptr,QXmppClient *client=nullptr);
    ~NotificationPanel();
    QXmppClient * client;
    QString jid;
    QString username;
    void setJid(QString jid);
    void startAnimation();
    void setTitle(QString title);
    void setExplanation(QString explain);
    void setHint(QString hint);
    void setID(QString id);
    void setInstruction(QString instruction);
    void setAgreeButtonTitle(QString title);
    void setRejectButtonTitle(QString title);
    void setIconForMessage();
    void setIconForInvitation();
    void setMessageReceiveMode(QString id, QString content, const QXmppMessage &message, QString username);
    void setInvitationReceiveMode(const QString &roomJid, const QString &inviter, const QString &reason);
    bool messageMode=false;
    bool invitationMode = false;
    QXmppMessage message;
    QString roomJid;

private slots:
    void on_agreeBtn_clicked();

    void on_rejectBtn_clicked();

private:
    Ui::NotificationPanel *ui;

    void addRoom();
};

#endif // NOTIFICATIONPANEL_H
