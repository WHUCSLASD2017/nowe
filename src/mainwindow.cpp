#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataframe.h"
#include "ChangeHeaderWnd.h"
#include "NoweGlobal.h"
#include <QRegExpValidator>
#include <QMessageBox>
#include <QMouseEvent>
#include <QToolButton>
#include <QPixmap>
#include <QStyle>
#include <QDesktopWidget>
#include <QLabel>
#include <QGroupBox>
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QMenu>
#include <QXmppVCardIq.h>
#include <QXmppVCardManager.h>
#include <QBuffer>
#include <QImageReader>
#include <QXmppRosterManager.h>
#include <QXmppDiscoveryManager.h>
#include "notificationpanel.h"
#include <QPropertyAnimation>
#include <QXmppUtils.h>
#include <QtWebEngineWidgets>
#include "chatarea.h"
#include "groupchatarea.h"

MainWindow::MainWindow(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::MainWindow),
    client(Nowe::myClient())
{
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();

    //设置三个面板的表头、列数（被隐藏，但必须设置）
    ui->friendTree->setColumnCount(1);
    QStringList headers;
    headers<<"好友";
    ui->friendTree->setHeaderLabel("好友");

    ui->messageTree->setColumnCount(1);
    QStringList headers2;
    headers2<<"消息";
    ui->messageTree->setHeaderLabel("消息");
    ui->switchPanel->setCurrentIndex(0);
    ui->messageTree->setIndentation(0);
    setMenu();
    setAddMenu();

    //构造分组


    client->logger()->setLoggingType(QXmppLogger::StdoutLogging);

    //添加扩展
    myBookMarkManager = new QXmppBookmarkManager();
    myRoomManager = new QXmppMucManager();
    myBookMarkManager = client->findExtension<QXmppBookmarkManager>();
    //client->addExtension(myRoomManager);
    myRoomManager = client->findExtension<QXmppMucManager>();



    // 当服务器发送 VCard 时更新主窗口上个人资料
    connect(client->findExtension<QXmppVCardManager>(), &QXmppVCardManager::clientVCardReceived,
            this, &MainWindow::on_clientVCardReceived);

    connect(client->findExtension<QXmppRosterManager>(), &QXmppRosterManager::rosterReceived,
            this, &MainWindow::on_rosterReceived);

    connect(client->findExtension<QXmppRosterManager>(), &QXmppRosterManager::presenceChanged,
            this, &MainWindow::on_presenceChanged);

    connect(client->findExtension<QXmppRosterManager>(), &QXmppRosterManager::subscriptionReceived,
            this, &MainWindow::on_subscriptionReceived);

    connect(client->findExtension<QXmppVCardManager>(), &QXmppVCardManager::vCardReceived,
            this, &MainWindow::on_vcardReceived);

    connect(Nowe::myClient(), &QXmppClient::messageReceived, this, &MainWindow::on_messageReceived);

    connect(Nowe::myMucManager(), &QXmppMucManager::invitationReceived, this, &MainWindow::on_invitationReceived);

    loadDone=false;

    //向服务器请求并发送群组列表

    connect(client->findExtension<QXmppBookmarkManager>(), &QXmppBookmarkManager::bookmarksReceived,
            this, &MainWindow::on_roomReceived);

    // 去掉主页标签的关闭按钮
    ui->mainTabs->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);

    ui->mainTabs->setMovable(true);

    connect(ui->mainTabs, &QTabWidget::tabCloseRequested,[=](int index) {
       ui->mainTabs->removeTab(index);
    });

    ui->recommendPage->setUrl(QUrl("http://chirsz.cc/nowe/moban3914/"));

    connect(ui->recommendPage,&QWebEngineView::titleChanged,this,&MainWindow::onTitleChanged);

}

void MainWindow::onTitleChanged(const QString &title)
{
    if(title!="chirsz.cc/nowe/moban3914/")
    {
        auto l = title.split('@');
        if (l.length() == 2) {
            if (l[1] == "friend") {
                auto jid = l[0]+"@chirsz.cc";
                createMessage(l[0], "Online", QImage(":/images/1.png"), jid);
                auto rstMng = client->findExtension<QXmppRosterManager>();
                rstMng->subscribe(jid);
            } else if (l[1] == "room") {
                QString serverName = "chirsz.cc";
                auto roomName = l[0];
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

                Groups groups = Groups::getMyGroups();
                groups.addGroup(jid);
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    if(!myBookMarkManager) delete myBookMarkManager;
    if(myRoomManager)    delete myRoomManager;
}

void MainWindow::setMenu()
{
    //设置左下角弹出菜单的
    menu=new QMenu(this);
    QAction *setProfile=new QAction("设置个人资料",this);
    //QAction *setAvatar=new QAction("设置头像",this);
    //把建立的动作添加到菜单
    menu->addAction(setProfile);
    menu->addSeparator();
    //menu->addAction(setAvatar);

    connect(setProfile,&QAction::triggered,this,&MainWindow::displayProfilePanel);
    //connect(setAvatar,&QAction::triggered,this,&MainWindow::displayAvatarChangePanel);

    //设置右下角弹出菜单
    addMenu=new QMenu(this);
    QAction *addFriendOrRoom=new QAction("添加好友或群里",this);
    //QAction *setAvatar=new QAction("设置头像",this);
    //把建立的动作添加到菜单
    addMenu->addAction(addFriendOrRoom);
    addMenu->addSeparator();
    //menu->addAction(setAvatar);

    connect(addFriendOrRoom,&QAction::triggered,this,&MainWindow::displayProfilePanel);
    //connect(setAvatar,&QAction::triggered,this,&MainWindow::displayAvatarChangePanel);
}


void MainWindow::setAddMenu()
{
    //设置右下角弹出菜单
    addMenu=new QMenu(this);
    QAction *addFriendOrRoom=new QAction("添加好友或群组",this);
    QAction *createRoom=new QAction("创建群组",this);
    //把建立的动作添加到菜单
    addMenu->addAction(addFriendOrRoom);
    addMenu->addAction(createRoom);
    addMenu->addSeparator();
    //menu->addAction(setAvatar);

    connect(addFriendOrRoom,&QAction::triggered,this,&MainWindow::displayAddPanel);
    connect(createRoom,&QAction::triggered,this,&MainWindow::displayCreateRoomPanel);

}

void MainWindow::displayProfilePanel()
{
    //显示个人资料窗口
    DataFrame* d=new DataFrame;

    d->show();
}

void MainWindow::displayAvatarChangePanel()
{
    //显示头像
    ChangeHeaderWnd* d=new ChangeHeaderWnd;
    d->show();
}

bool MainWindow::ifChatExist(const QString &bareJID)
{
    int tabs = ui->mainTabs->count();
    for(int i=0; i < tabs; ++i) {
        auto a = dynamic_cast<ChatArea*>(ui->mainTabs->widget(i));
        if (a != nullptr && a->receiverJidBare() == bareJID) {
            return true;
        }
    }
    return false;
}

//新建一个好友分组
QTreeWidgetItem *MainWindow::createFriendGroup(QString grpName)
{
    QTreeWidgetItem *item1=new QTreeWidgetItem;
    item1->setText(0,grpName);
    //添加到列表的顶层
    ui->friendTree->addTopLevelItem(item1);
    return item1;
}
//在消息面板添加一个消息
QTreeWidgetItem *MainWindow::createMessage(QString mainTitle,QString subTitle,const QImage& avatar,QString jid)
{
    //先添加一个表项到消息列表
    QTreeWidgetItem *item1=new QTreeWidgetItem;
    ui->messageTree->addTopLevelItem(item1);
    //再把这个表项里的控件插入进去，下面都是这样
    ui->messageTree->setItemWidget(item1,0,createItem(jid,mainTitle,avatar,subTitle,true,true));
    return item1;
}


QTreeWidgetItem *MainWindow::addFriendtoGroup(QTreeWidgetItem *grp,QString mainTitle,QString subTitle,const QImage& avatar,QString jid,QString role)
{
    //向一个分组里添加一个好友，也就是向二级表项里插入一个三级表项
    QTreeWidgetItem *item11=new QTreeWidgetItem(grp);
    ui->friendTree->setItemWidget(item11,0,createItem(jid,mainTitle,avatar,subTitle,true,true));
    return item11;
}

QTreeWidgetItem *MainWindow::addFriendtoGroupAtTop(QTreeWidgetItem *grp,QString mainTitle,QString subTitle,const QImage& avatar,QString jid)
{
    //和上面函数功能一样，但是上面是在末尾插，这个在顶部插
    QTreeWidgetItem *item11=new QTreeWidgetItem(grp,nullptr);
    QWidget *now=createItem(jid,mainTitle,avatar,subTitle,true,true);
    ui->friendTree->setItemWidget(item11,0,now);

    return item11;
}

QTreeWidgetItem *MainWindow::removeFriendOrGroup(QTreeWidgetItem *toSet)
{
    //从列表移除分组或好友
    QTreeWidgetItem *parent=toSet->parent();
    int index;
    if(parent)
    {
        //如果不是一级列表！从父对象中删
        index=parent->indexOfChild(toSet);
        parent->removeChild(toSet);
        //ui->friendTree->removeItemWidget(parent->takeChild(index),0);
    }
    else {
        //是一级列表，从根节点下删
        index=ui->friendTree->indexOfTopLevelItem(toSet);
        qDebug()<<parent<<"   "<<index;
        ui->friendTree->takeTopLevelItem(index);
    }
    return toSet;
}


QTreeWidgetItem *MainWindow::setFriendToTop(QTreeWidgetItem *toSet,QString mainTitle,QString subTitle,const QImage& avatar,QTreeWidgetItem *grp,QString jid)
{
    //把某个好友放到顶部去，就是删了再加
    if(grp==nullptr)
        grp=toSet->parent();
    removeFriendOrGroup(toSet);
    return addFriendtoGroupAtTop(grp,mainTitle,subTitle,avatar,jid);
}


QWidget *MainWindow::createItem(QString jid,QString mainTitle, const QImage& icon, QString subTitle, bool ifVIP=false, bool ifOnline=false)
{
    //本函数用于向一个已经存在的表项里添加控件
    QWidget *myItem=new QWidget(this);
    QLabel *mainLabel=new QLabel(this);
    QLabel *subLabel=new QLabel(this);
    QLabel *iconLabel=new QLabel(this);
    QPixmap avatar= QPixmap::fromImage(icon);
    QLabel *jidLabel=new QLabel(jid,this);
    QLabel *nickName=new QLabel(mainTitle,this);
    jidLabel->hide();
    nickName->hide();
    QPixmap avatarAfter=avatar.scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    //分别代表一个消息条中的用户名、个性签名和头像，addr存头像地址，隐藏起来，发射信号用
    mainLabel->setText(mainTitle);
    subLabel->setText(subTitle);
    iconLabel->setPixmap(avatarAfter);

    mainLabel->setStyleSheet(QString("font:bold 12pt \"微软雅黑\";padding:5pt;"));
    subLabel->setStyleSheet(QString(""));
    //其余内容都是设置布局
    QVBoxLayout *vLayout=new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addWidget(mainLabel);
    vLayout->addWidget(subLabel);
    vLayout->addWidget(nickName);
    vLayout->addWidget(jidLabel);
    vLayout->addStretch();
    vLayout->setSpacing(5);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setMargin(3);

    QHBoxLayout *hLayout=new QHBoxLayout();
    hLayout->addWidget(iconLabel);
    hLayout->addLayout(vLayout);
    hLayout->addStretch();
    hLayout->addStretch();
    hLayout->setContentsMargins(0,0,0,0);

    hLayout->setMargin(10);
    myItem->setLayout(hLayout);

    return myItem;
}



//创建群组面板里的控件
QWidget *MainWindow::createRoomItem(QString mainTitle, QString iconAddr)
{
    //本函数用于向一个已经存在的表项里添加控件
    QWidget *myItem=new QWidget(this);
    QLabel *mainLabel=new QLabel(this);
    QLabel *iconLabel=new QLabel(this);
    QPixmap *avatar=new QPixmap(iconAddr);
    QLabel *addr=new QLabel(iconAddr,this);
    addr->hide();
    QPixmap avatarAfter=avatar->scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    //分别代表群组名称和头像，addr存头像地址，隐藏起来，发射信号用
    mainLabel->setText(mainTitle);
    iconLabel->setPixmap(avatarAfter);

    mainLabel->setStyleSheet(QString("font:bold 12pt \"微软雅黑\";padding:5pt;"));
    //其余内容都是设置布局
    QVBoxLayout *vLayout=new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addWidget(mainLabel);
    vLayout->addWidget(addr);
    vLayout->addStretch();
    vLayout->setSpacing(5);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setMargin(3);

    QHBoxLayout *hLayout=new QHBoxLayout();
    hLayout->addWidget(iconLabel);
    hLayout->addLayout(vLayout);
    hLayout->addStretch();
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setMargin(10);

    myItem->setLayout(hLayout);


    return myItem;
}

//
void MainWindow::windowclosed()
{
    client->disconnectFromServer();
    this->close();
    QApplication::exit();
}


//主面板跳转页面
//跳转至消息页面
void MainWindow::on_pushButton_clicked()
{
    ui->switchPanel->setCurrentIndex(0);
}
//跳转至群组页面
void MainWindow::on_pushButton_2_clicked()
{
    ui->switchPanel->setCurrentIndex(2);
}
//跳转至好友页面
void MainWindow::on_pushButton_3_clicked()
{
    ui->switchPanel->setCurrentIndex(1);
    updateAllFriends();
    on_roomReceived(myBookMarkManager->bookmarks());
}

void MainWindow::on_roomTree_itemClicked(QTreeWidgetItem *item, int column)
{

}

void MainWindow::on_friendTree_itemClicked(QTreeWidgetItem *item, int column)
{

}

void MainWindow::on_messageTree_itemClicked(QTreeWidgetItem *item, int column)
{

}

void MainWindow::on_messageTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    //双击了消息面板！
    QWidget *now=ui->messageTree->itemWidget(item,0);
    QList<QLabel *> labelList = now->findChildren<QLabel *>();
    // labelList:[4]->bareJid [2]->signature

    auto bareJID = labelList[4]->text();
    auto nickName = labelList[1]->text();


    //要找到用户点击了哪个面板，从其中的label里面找到用户名等信息，发射信号
    emit msgClicked(labelList[1]->text());
    //不仅要发射信号，还要用获得的用户名等信息，创建一个聊天框
    //qDebug()<<"                                   "<<labelList<<labelList[0]->text()<<labelList[1]->text()<<labelList[2]->text()<<labelList[3]->text()<<labelList[4]->text();

    //聊天框建立
    //ChatDialog::getChatDialog(labelList[4]->text(),ui->nickname->text(),labelList[2]->text(),labelList[1]->text(),avatar);
    popupChatTab(bareJID, nickName);
}

void MainWindow::on_friendTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{    
    //和上面的on_messageTree_itemDoubleClicked类似，不过这个点击的是好友面板
    QWidget *now=ui->friendTree->itemWidget(item,0);
    if(item->parent()->text(0) == "圈子") {
        QList<QLabel *> labelList = now->findChildren<QLabel *>();
        emit roomClicked(labelList[1]->text());

        //服务器名
        QString serverName = "chirsz.cc";
        //聊天室JID
        QString jid = labelList[1]->text() + "@conference." + serverName;

        Group * myGroup = Groups::getMyGroups().getGroup(jid);

        myGroup->obtainMemberList("none");

        popupGroupChatTab(myGroup, labelList[1]->text());
    } else {
        QList<QLabel *> labelList = now->findChildren<QLabel *>();
        auto bareJID = labelList[4]->text();
        auto nickName = labelList[1]->text();

        popupChatTab(bareJID, nickName);
    }
}

void MainWindow::on_roomTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "MainWindow::on_roomTree_itemDoubleClicked starting" << endl;

    //和上面的on_messageTree_itemDoubleClicked类似，不过这个点击的是群组面板
    QWidget *now=ui->roomTree->itemWidget(item,0);

    QList<QLabel *> labelList = now->findChildren<QLabel *>();

    qDebug() << "labelList.size " << labelList.size() << endl;
    qDebug() << "labelList<<labelList[0]->text() " << labelList[0]->text() << endl;
    qDebug() << "labelList<<labelList[1]->text() " << labelList[1]->text() << endl;
    qDebug() << "labelList<<labelList[2]->text() " << labelList[2]->text() << endl;

    emit roomClicked(labelList[1]->text());
    QPixmap avatar;
    avatar.load(labelList[2]->text());

    //服务器名
    QString serverName = "chirsz.cc";
    //聊天室JID
    QString jid = labelList[1]->text() + "@conference." + serverName;

    Group * myGroup = Groups::getMyGroups().getGroup(jid);
    //qDebug() << "MainWindow::on_roomTree_itemDoubleClicked myGroup->getRoomJid() " << myGroup->getRoomJid() << endl;

    myGroup->obtainMemberList("none");

    //GroupChatDialog::getChatDialog(myGroup, ui->nickname->text(), "", labelList[1]->text(), avatar);
    popupGroupChatTab(myGroup, labelList[1]->text());

}

//设置头像
void MainWindow::setAvatar(QPixmap &avatar, int length, int width, int radius)
{
    //首先缩放头像
    QPixmap pixMap= avatar.scaled(width,length, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    //把头像搞成圆形的
    pixMap =  PixmapToRound(pixMap, radius);

    //最后设置
    ui->avatar->setPixmap(pixMap);
}

QPixmap MainWindow::PixmapToRound(const QPixmap &src, int radius)
{
    //用于把头像从方的裁成圆的，radius是裁剪半径
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void MainWindow::setMainTitle(QString string)
{
    //设置主标题
    ui->nickname->setText(string);
}

void MainWindow::setSubTitle(QString string)
{
    //设置副标题
    ui->signature->setText(string);
}

// 用户设置菜单
void MainWindow::on_pushButton_4_clicked()
{
    menu->exec(ui->pushButton_4->mapToGlobal(QPoint(20,20)));
}

// 根据最新的 VCard 更新主窗口上个人资料
void MainWindow::on_clientVCardReceived()
{
    const auto& myCard =  Nowe::myVCard();
    QBuffer buffer;
    buffer.setData(myCard.photo());
    buffer.open(QIODevice::ReadOnly);
    QImageReader avaterReader(&buffer);
    QPixmap avatar = QPixmap::fromImage(avaterReader.read());

    setAvatar(avatar,40,40,20);
    setMainTitle(myCard.fullName());
    setSubTitle(myCard.description());
}

void MainWindow::on_vcardReceived(const QXmppVCardIq& vcard)
{
    const static QImage defaultAvatar(":/images/1.png");
    static int loadedItems = 0;

    if (vcard.from().isEmpty())
        return;

    auto photo = vcard.photo();
    auto bareJid = QXmppUtils::jidToBareJid(vcard.from());

    QImage avatar;
    if (photo.isEmpty()) {
        avatar = defaultAvatar;
    } else {
        QBuffer buffer;
        buffer.setData(photo);
        buffer.open(QIODevice::ReadOnly);
        QImageReader avatarReader(&buffer);
        avatar = avatarReader.read();
    }

    auto rstMng = client->findExtension<QXmppRosterManager>();
    auto item = rstMng->getRosterEntry(bareJid);
    auto resources = rstMng->getResources(bareJid);
    QString res = resources.isEmpty() ? "" : resources[0];
    auto presence = rstMng->getPresence(bareJid,res);

    if (!loadDone) {
        createMessage(item.bareJid(), presence.statusText(), avatar,item.bareJid());
        ++loadedItems;
        if (loadedItems == rstMng->getRosterBareJids().length())
            loadDone = true;
    }


    /*if(item.groups().empty()) {
        grp = grpMng.getGrpAddr("未分组好友",this);
    } else {
        grp = grpMng.getGrpAddr(*(item.groups().begin()),this);
    }*/
    friendGrp = grpMng.getGrpAddr("好友",this);
    //roomGrp = grpMng.getGrpAddr("圈子",this);


    QString role;


    addFriendtoGroup(friendGrp,item.bareJid(),presence.statusText(),avatar,bareJid,role);
}

// 初始化好友列表
void MainWindow::on_rosterReceived()
{
    Nowe::sendBookMarkRequest();
    QMap<QString, QByteArray> jidToAvatar;

    auto rstMng = client->findExtension<QXmppRosterManager>();
    auto vcdMng = client->findExtension<QXmppVCardManager>();

    foreach(const QString& bareJid, rstMng->getRosterBareJids()) {
        vcdMng->requestVCard(bareJid);
    }
}


void MainWindow::on_presenceChanged()
{

}

void MainWindow::addFriend()
{
    //auto rstMng = client->findExtension<QXmppRosterManager>();
    //rstMng->subscribe("b@chirsz.cc");
}



void MainWindow::flushAllFriends()
{
    ui->friendTree->clear();
}

void MainWindow::showAddNewFriendPanel(QString jid)
{
    AddNewFriend *dialog=new AddNewFriend(client,this);
    dialog->show();
    dialog->setLineEditContent(jid);
}

void MainWindow::updateAllFriends()
{
    flushAllFriends();
    grpMng.flush();
    on_rosterReceived();
}

//显示添加好友和群组面板
void MainWindow::displayAddPanel()
{
    AddNewFriend *dialog=new AddNewFriend(client,this);
    dialog->show();
    dialog->setWindowModality(Qt::ApplicationModal);
    updateAllFriends();
}

//显示创建群组面板
void MainWindow::displayCreateRoomPanel()
{
    CreateRoom *dialog=new CreateRoom(this);
    dialog->show();
    dialog->setWindowModality(Qt::ApplicationModal);
    //updateAllFriends();
}

// 添加好友/群组菜单
void MainWindow::on_AddItemBtn_clicked()
{
    addMenu->exec(ui->AddItemBtn->mapToGlobal(QPoint(20,20)));
}

void MainWindow::on_subscriptionReceived(const QString &bareJid)
{
    NotificationPanel *notice=new NotificationPanel(this,client);
    notice->show();
    notice->setJid(bareJid);
    notice->startAnimation();
    updateAllFriends();
}

void MainWindow::on_messageReceived(const QXmppMessage &msg)
{
    username=ui->nickname->text();
    auto senderID=QXmppUtils::jidToBareJid(msg.from());
    auto msgBody=msg.body();
    if((!msg.body().isEmpty())&&(!ifChatExist(senderID)))
    {
    NotificationPanel *notice=new NotificationPanel(this,client);
    notice->setMessageReceiveMode(senderID,msgBody,msg,username);
    //notice->show();
    notice->startAnimation();
    }
}

void MainWindow::on_invitationReceived(const QString &roomJid, const QString &inviter, const QString &reason)
{
    qDebug() << "MainWindow::on_invitationReceived " << endl;

    NotificationPanel * notice=new NotificationPanel(this, client);

    notice->setInvitationReceiveMode(roomJid, inviter, reason);
    notice->show();
    notice->startAnimation();    
}

//在聊天室面板添加一个聊天室
QTreeWidgetItem *MainWindow::addRoom(QString roomName,QString avatarAddr)
{
    //暂时把头像文件写死
    avatarAddr = ":/images/room.png";
    //先添加一个表项到群组列表
    QTreeWidgetItem *item1=new QTreeWidgetItem;
    ui->roomTree->addTopLevelItem(item1);
    //再把这个表项里的控件插入进去
    ui->roomTree->setItemWidget(item1,0,createRoomItem(roomName,avatarAddr));
    return item1;
}


//创建聊天室书签
void MainWindow::createBookMark( QString markName)
{
    //加载已存在的书签
    auto markMsg = client->findExtension<QXmppBookmarkManager>();
    QXmppBookmarkSet markset = markMsg->bookmarks();

    //服务器书签列表
    QList<QXmppBookmarkConference> markList= markset.conferences();
    //服务器名
    QString serverName = "chirsz.cc";
    //聊天室JID
    QString jid=markName+"@conference."+serverName;


    //若该书签已经存在则返回
    foreach(QXmppBookmarkConference mark, markList)
    {
        if(mark.jid() == jid)
            return ;
    }

    //增加书签
    QXmppBookmarkConference * bm = new QXmppBookmarkConference;
    bm->setJid(jid);
    bm->setName(markName);
    bm->setNickName(Nowe::myJidBare());
    bm->setAutoJoin(true);      //设置登陆时自动加入
    markList.append(*bm);
    markset.setConferences(markList);

    markMsg->setBookmarks(markset);

}


//将群组添加到分组
QTreeWidgetItem *MainWindow::addRoomtoGroup(QTreeWidgetItem *grp,QString name,const QString &avatar)
{
    //向一个分组里添加一个好友，也就是向二级表项里插入一个三级表项
    QString avatarAddr = ":/images/room.png";

    QTreeWidgetItem *item11=new QTreeWidgetItem(grp);
    ui->friendTree->setItemWidget(item11,0,createRoomItem(name,avatarAddr));
    return item11;
}



//初始化及更新聊天室书签列表
void MainWindow::on_roomReceived(const QXmppBookmarkSet &bookmarks)
{
    //打印当前书签
    /*foreach(QXmppBookmarkConference c,myBookMarkManager->bookmarks().conferences())
    {
        qDebug()<<"\n\n\n"<<c.jid();
    }*/

    //清空群组面板
    ui->roomTree->clear();



    QXmppMucManager * roomMsg = Nowe::myClient()->findExtension<QXmppMucManager>();


    QString avatarAddr = ":/images/room.png";    //群组列表头像【头像路径在addRoom函数中已写死，在此留出接口方便之后修改】
    QImage ava;
    ava.load(":/images/room.png");

    roomGrp = grpMng.getGrpAddr("圈子",this);

    //更新本地聊天室书签管理
    QList<QXmppBookmarkConference> markList= bookmarks.conferences();
    foreach(QXmppBookmarkConference mark, markList)
    {

        addRoomtoGroup(roomGrp,mark.name(),avatarAddr);

        //addRoom(mark.name(),avatarAddr);
        roomMsg->addRoom(mark.jid());
        Groups::getMyGroups().addGroup(mark.jid());
    }

    //设置群管理
    setMucManager();
}

//处理群组,设置群组管理器
void MainWindow::setMucManager()
{
    auto roomMsg = client->findExtension<QXmppMucManager>();
    auto bookmMsg = client->findExtension<QXmppBookmarkManager>();

    QXmppBookmarkSet bookmarks = bookmMsg->bookmarks();

    QList<QXmppBookmarkConference> markList= bookmarks.conferences();
    foreach(QXmppBookmarkConference mark, markList)
    {
        //createRoom(mark.name());
        QXmppMucRoom* m_pRoom = roomMsg->addRoom(mark.jid());
        m_pRoom->setNickName(mark.nickName());
        m_pRoom->join();
    }
    //createRoom("zff");
}

void MainWindow::popupChatTab(const QString &bareJID, const QString &nickName)
{
    ui->stackedWidget->setCurrentIndex(1);

    int tabs = ui->mainTabs->count();
    for(int i=0; i < tabs; ++i) {
        auto a = dynamic_cast<ChatArea*>(ui->mainTabs->widget(i));
        if (a != nullptr && a->receiverJidBare() == bareJID) {
            ui->mainTabs->setCurrentWidget(a);
            return;
        }
    }

    // 打开新标签页
    auto newPage = new ChatArea(bareJID);
    newPage->setTitle(nickName);
    connect(newPage, &ChatArea::closeBtnClick, [=]() {
        int i = ui->mainTabs->indexOf(newPage);
       ui->mainTabs->removeTab(i);
    });
    int newtabi = ui->mainTabs->addTab(newPage, nickName);
    ui->mainTabs->setCurrentIndex(newtabi);
}

void MainWindow::popupGroupChatTab(Group *grp, const QString &groupName)
{
    ui->stackedWidget->setCurrentIndex(1);

    int tabs = ui->mainTabs->count();
    for(int i=0; i < tabs; ++i) {
        auto a = dynamic_cast<GroupChatArea*>(ui->mainTabs->widget(i));
        if (a != nullptr && a->group() == grp) {
            ui->mainTabs->setCurrentWidget(a);
            return;
        }
    }

    // 打开新标签页
    auto newPage = new GroupChatArea(grp);
    newPage->setTitle(groupName);
    connect(newPage, &GroupChatArea::closeBtnClick, [=]() {
        int i = ui->mainTabs->indexOf(newPage);
        ui->mainTabs->removeTab(i);
    });
    int newtabi = ui->mainTabs->addTab(newPage, groupName);
    ui->mainTabs->setCurrentIndex(newtabi);
}

/*
//创建一个新的群组
void MainWindow::createRoom(QString roomName)
{
    QXmppMucManager* roomMsg = client->findExtension<QXmppMucManager>();
    //服务器名
    QString serverName = "chirsz.cc";
    //聊天室JID
    QString jid=roomName+"@conference."+serverName;

    QList<QXmppMucRoom*> rooms = roomMsg->rooms();

    //查询聊天室JID是否冲突
    foreach(QXmppMucRoom* r, rooms)
    {
        qDebug()<<r->jid();
        if(r->jid() == jid)
        {
            return ;
        }
    }

    //添加群组
    QXmppMucRoom*  m_pRoom = roomMsg->addRoom(jid);

    if(m_pRoom)
    {
        //群名片
        m_pRoom->setNickName("theDip");
        //进入群
        m_pRoom->join();
       // createBookMark(roomName);
    }



}
*/


void MainWindow::on_recommendBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_chatBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_findBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}
