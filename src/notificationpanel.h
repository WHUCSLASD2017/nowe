#ifndef NOTIFICATIONPANEL_H
#define NOTIFICATIONPANEL_H

#include <QWidget>
#include "nowebasewindow.h"
#include <QXmppClient.h>

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
    void setJid(QString jid);
    void startAnimation();

private slots:
    void on_agreeBtn_clicked();

    void on_rejectBtn_clicked();

private:
    Ui::NotificationPanel *ui;
};

#endif // NOTIFICATIONPANEL_H
