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
    qDebug() << "NotificationPanel::startAnimation " << endl;

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

void NotificationPanel::setIconForInvitation()
{
    ui->icon->setPixmap(QPixmap(":/images/invitation.png"));
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
    this->messageMode=true;
}

void NotificationPanel::setInvitationReceiveMode(const QString &roomJid, const QString &inviter, const QString &reason)
{
    qDebug() << "NotificationPanel::setInvitationReceiveMode start " << "roomJid " << roomJid << " inviter " << inviter << " reason " << reason << endl;

    setIconForMessage();
    setTitle("群聊邀请");
    setExplanation("您收到了群聊邀请消息!");
    setInstruction("您收到了" + inviter + "发来的群聊邀请消息。\n消息详细信息如下：");
    setHint(roomJid);
    setID(reason);
    setAgreeButtonTitle("同意");
    setRejectButtonTitle("拒绝");
    this->invitationMode = true;
    this->roomJid = roomJid;

    qDebug() << "NotificationPanel::setInvitationReceiveMode end " << "roomJid " << roomJid << " inviter " << inviter << " reason " << reason << endl;
}

void NotificationPanel::on_agreeBtn_clicked()
{
    if (messageMode == true) {
        ChatDialog::getChatDialog(QXmppUtils::jidToBareJid(message.from()),username,"",QXmppUtils::jidToBareJid(message.from()),QPixmap());
        close();

    } else if (invitationMode == true) {
        addRoom();
        close();

    } else {
        QXmppRosterManager* rstMng = client->findExtension<QXmppRosterManager>();
        rstMng->acceptSubscription(jid);
        ((MainWindow *)parent())->showAddNewFriendPanel(jid);
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

void NotificationPanel::addRoom()
{
    qDebug() << "NotificationPanel::addRoom " << "this->roomJid " << this->roomJid << endl;

    //服务器名
    QString suffix = "@conference.chirsz.cc";
    //聊天室JID
    QString roomName = this->roomJid.replace(this->roomJid.indexOf('@'), suffix.length(), "");

    qDebug() << "NotificationPanel::addRoom " << "roomName " << roomName << endl;

    QXmppMucManager * roomMsg = client->findExtension<QXmppMucManager>();
    QXmppBookmarkManager * bookMsg = client->findExtension<QXmppBookmarkManager>();

    //服务器书签列表
    QXmppBookmarkSet markset = bookMsg->bookmarks();
    QList<QXmppBookmarkConference> markList= markset.conferences();

    QList<QXmppMucRoom*> rooms = roomMsg->rooms();

    //若该书签已经存在则返回
    foreach(QXmppBookmarkConference mark, markList)
    {
        if(mark.jid() == this->roomJid)
        {
            QMessageBox::critical(this,"加入失败","已加入该群聊！");
            return ;
        }
    }

    //将加入信息打包发送给服务器
    QXmppPresence p;
    QString to = this->roomJid + "/" + Nowe::myJidBare();
    p.setTo(to);
    p.setFrom(Nowe::myJid());
    client -> sendPacket(p);

    //添加群组
    roomMsg->addRoom(jid);

    Nowe::createBookMark(roomName);

    Groups groups = Groups::getMyGroups();
    groups.addGroup(this->roomJid);

    qDebug() << "NotificationPanel::addRoom" << "roomName" << roomName << endl;

}
