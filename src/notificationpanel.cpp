#include "notificationpanel.h"
#include "ui_notificationpanel.h"
#include "mainwindow.h"
#include <QXmppRosterManager.h>
#include <QPropertyAnimation>

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

void NotificationPanel::startAnimation()
{
    QPropertyAnimation *animation=new QPropertyAnimation(ui->TitleFrame,"geometry");
    animation->setDuration(1000);
    animation->setStartValue(QRect(0,-101,541,101));
    animation->setEndValue(QRect(0,0,541,101));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();

    QPropertyAnimation *animation2=new QPropertyAnimation(ui->icon,"geometry");
    animation2->setDuration(200);
    animation2->setStartValue(QRect(10,10,81,71));
    animation2->setEndValue(QRect(10,10,81,71));
    animation2->setKeyValueAt(0.5,QRect(25,15,81,71));
    animation2->setEasingCurve(QEasingCurve::InCubic);
    animation2->setLoopCount(-1);
    animation2->start();

    QPropertyAnimation *animation3=new QPropertyAnimation(ui->content,"geometry");
    animation3->setDuration(1000);
    animation3->setStartValue(QRect(0,400,500,400));
    animation3->setEndValue(QRect(0,0,500,400));
    animation3->setEasingCurve(QEasingCurve::OutBounce);
    animation3->start();

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
