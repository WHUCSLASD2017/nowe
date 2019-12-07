#include "notificationpanel.h"
#include "ui_notificationpanel.h"
#include "mainwindow.h"
#include <QXmppRosterManager.h>
#include <QPropertyAnimation>
#include <QXmppMessage.h>
#include "chatdialog.h"
#include "QXmppUtils.h"
#include <QGraphicsOpacityEffect>

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
    animation2->setDuration(2000);
    animation2->setStartValue(QRect(10,10,81,71));
    animation2->setEndValue(QRect(10,10,81,71));
    animation2->setKeyValueAt(0.5,QRect(20,15,81,71));
    animation2->setEasingCurve(QEasingCurve::InCubic);
    animation2->setLoopCount(1);
    animation2->start();


    QGraphicsOpacityEffect * m_pGraphicsOpacityEffect1=new QGraphicsOpacityEffect(ui->icon);
    m_pGraphicsOpacityEffect1->setOpacity(0);
    ui->icon->setGraphicsEffect(m_pGraphicsOpacityEffect1);

    QPropertyAnimation *anime3=new QPropertyAnimation(m_pGraphicsOpacityEffect1,"opacity");
    anime3->setDuration(2000);
    anime3->setStartValue(0);
    anime3->setKeyValueAt(0.5,1);
    anime3->setEndValue(0);
    anime3->setEasingCurve(QEasingCurve::OutCubic);
    anime3->setLoopCount(-1);
    anime3->start();

    QPropertyAnimation *animation3=new QPropertyAnimation(ui->content,"geometry");
    animation3->setDuration(1000);
    animation3->setStartValue(QRect(0,400,500,400));
    animation3->setEndValue(QRect(0,0,500,400));
    animation3->setEasingCurve(QEasingCurve::OutBounce);
    animation3->start();

    QPropertyAnimation *animation4=new QPropertyAnimation(this,"geometry");
    animation4->setDuration(2000);
    int x=frameGeometry().x();
    int y=frameGeometry().y();
    animation4->setStartValue(QRect(x,y,500,400));
    animation4->setKeyValueAt(0.25,QRect(x+10,y+10,500,400));
    animation4->setKeyValueAt(0.5,QRect(x,y+10,500,400));
    animation4->setKeyValueAt(0.75,QRect(x+10,y,500,400));
    animation4->setEndValue(QRect(x,y,500,400));
    animation4->setEasingCurve(QEasingCurve::OutBounce);
    animation4->setLoopCount(-1);
    animation4->start();


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
    ((MainWindow *)parent())->showAddNewFriendPanel(jid);
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
