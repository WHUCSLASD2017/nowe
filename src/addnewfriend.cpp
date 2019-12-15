#include "addnewfriend.h"
#include "ui_addnewfriend.h"
#include "nowebasewindow.h"
#include <QMessageBox>
#include <QXmppRosterManager.h>
#include <QXmppBookmarkManager.h>
#include <QXmppBookmarkSet.h>
#include <QXmppMucManager.h>
#include <QXmppMucIq.h>
#include "NoweGlobal.h"
#include <QXmppDiscoveryManager.h>

AddNewFriend::AddNewFriend(QXmppClient *client,QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::AddNewFriend)
{
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();
    this->client=client;
    connect(client->findExtension<QXmppDiscoveryManager>(), &QXmppDiscoveryManager::infoReceived,
            this, &AddNewFriend::judgeAddRoom);
    connect(client->)
}

AddNewFriend::~AddNewFriend()
{
    delete ui;
}

void AddNewFriend::setLineEditContent(QString jid)
{
    ui->lineEdit->setText(jid);
}

void AddNewFriend::on_cancelButton_clicked()
{
    close();
}

void AddNewFriend::on_confirmButton_clicked()
{
    auto rstMng = client->findExtension<QXmppRosterManager>();
    QString user=ui->lineEdit->text();
    if(user.simplified().length()==0)
        QMessageBox::critical(this,"输入非法","您输入的ID非法！");
    else {
        rstMng->subscribe(user);
        //close();
    }
}

void AddNewFriend::on_addFrBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AddNewFriend::on_addRoomBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}



void AddNewFriend::on_cancelButton_3_clicked()
{
    close();
}

//加入房间
void AddNewFriend::addRoom(QString roomName)
{
    QString serverName = "chirsz.cc";
    //聊天室JID
    QString jid=roomName+"@conference."+serverName;

    QXmppMucManager* roomMsg = client->findExtension<QXmppMucManager>();
    QXmppBookmarkManager *bookMsg = client->findExtension<QXmppBookmarkManager>();

    QXmppBookmarkSet markset = bookMsg->bookmarks();

    //服务器书签列表
    QList<QXmppBookmarkConference> markList= markset.conferences();

    QList<QXmppMucRoom*> rooms = roomMsg->rooms();


    //若该书签已经存在则返回
    foreach(QXmppBookmarkConference mark, markList)
    {
        if(mark.jid() == jid)
        {
            QMessageBox::critical(this,"加入失败","已加入该群聊！");
            return ;
        }
    }

    //将加入信息打包发送给服务器
    QXmppPresence p;
    QString to = jid+"/"+Nowe::myJidBare();
    p.setTo(to);
    p.setFrom(Nowe::myJid());
    client->sendPacket(p);

    Nowe::createBookMark(roomName);

    firstAdd = false;

    close();
    /*QXmppMucRoom*  m_pRoom = roomMsg->addRoom(jid);

    if(m_pRoom)
    {
        //群名片：必须设置，否则将创建失败
        m_pRoom->setNickName(Nowe::myJidBare());

        //进入群:此命令必须在设置房间属性之前，只有加入房间后才能设置属性
        m_pRoom->join();

    }*/

}


//点击群组加入按钮
void AddNewFriend::on_confirmButton_3_clicked()
{
    roomName=ui->lineEdit_3->text();
    //服务器名
    QString serverName = "chirsz.cc";
    //聊天室JID
    QString jid=roomName+"@conference."+serverName;

    if(roomName.simplified().length()==0)
        QMessageBox::critical(this,"输入非法","您输入的群聊名称非法");
    else {
        //发送查看群组信号
        QXmppDiscoveryManager * mydisc = client->findExtension<QXmppDiscoveryManager>();
        mydisc->requestInfo(jid);
        firstAdd = true;
    }
}



//判断加入的群聊是否存在,若存在则加入
void AddNewFriend::judgeAddRoom(const QXmppDiscoveryIq &iq)
{
    QList<QXmppDiscoveryIq::Identity> identities = iq.identities();
    if(identities.size()&&firstAdd)
    {
        addRoom(roomName);
    }
    else if(!identities.size()){
        QMessageBox::critical(this,"加入失败","该群聊不存在！");
    }
}
