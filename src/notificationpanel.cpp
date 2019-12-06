#include "notificationpanel.h"
#include "ui_notificationpanel.h"
#include "mainwindow.h"
#include <QXmppRosterManager.h>
#include <QPropertyAnimation>
#include <QXmppMessage.h>
#include "chatdialog.h"
#include "QXmppUtils.h"

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

void NotificationPanel::setTitle(QString title)
{
    ui->title->setText(title);
}

void NotificationPanel::setExplanation(QString explain)
{
    ui->explanation->setText(explain);
}

void NotificationPanel::setHint(QString hint)
{
    ui->hint->setText(hint);
}

void NotificationPanel::setID(QString id)
{
    ui->id->setText(id);
}

void NotificationPanel::setInstruction(QString instruction)
{
    ui->instruction->setText(instruction);
}

void NotificationPanel::setAgreeButtonTitle(QString title)
{
    ui->agreeBtn->setText(title);
}

void NotificationPanel::setRejectButtonTitle(QString title)
{
    ui->rejectBtn->setText(title);
}

void NotificationPanel::setIconForMessage()
{
    ui->icon->setPixmap(QPixmap(":/images/mail.png"));
}

void NotificationPanel::setMessageReceiveMode(QString id,QString content,const QXmppMessage &message,QString username)
{
    setIconForMessage();
    setTitle("新消息");
    setExplanation("您收到了一条新消息！");
    setInstruction("您收到了一条新消息。\n消息详细信息如下：");
    setHint(id);
    setID(content);
    setAgreeButtonTitle("查看消息");
    setRejectButtonTitle("忽略消息");
    this->message=message;
    this->username=username;
    messageMode=true;
}

void NotificationPanel::on_agreeBtn_clicked()
{
    if(messageMode==false)
     {
     QXmppRosterManager* rstMng = client->findExtension<QXmppRosterManager>();
     rstMng->acceptSubscription(jid);
     close();
    }
    else{
        ChatDialog::getChatDialog(QXmppUtils::jidToBareJid(message.from()),username,"",QXmppUtils::jidToBareJid(message.from()),QPixmap());
        close();
    }
}


void NotificationPanel::on_rejectBtn_clicked()
{
    if(messageMode==false)
    {
        QXmppRosterManager* rstMng = client->findExtension<QXmppRosterManager>();
    rstMng->refuseSubscription(jid);
    close();
    }
    else {
        close();
    }
}
