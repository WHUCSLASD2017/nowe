#include "notificationpanel.h"
#include "ui_notificationpanel.h"
#include "mainwindow.h"
#include <QXmppRosterManager.h>

NotificationPanel::NotificationPanel(QWidget *parent, QXmppClient *client) :
    NoweBaseWindow(parent),
    ui(new Ui::NotificationPanel)
{
    ui->setupUi(this);
    this->client=client;
    NoweBaseWindow::initNoweStyle();
    setWindowModality(Qt::WindowModal);
}

NotificationPanel::~NotificationPanel()
{
    delete ui;
}

void NotificationPanel::setJid(QString jid)
{
    this->jid=jid;
    ui->id->setText(jid);
}

void NotificationPanel::on_agreeBtn_clicked()
{
     QXmppRosterManager* rstMng = client->findExtension<QXmppRosterManager>();
     rstMng->acceptSubscription(jid);
     close();
}


void NotificationPanel::on_rejectBtn_clicked()
{
    QXmppRosterManager* rstMng = client->findExtension<QXmppRosterManager>();
    rstMng->refuseSubscription(jid);
    close();
}
