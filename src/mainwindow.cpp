#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatdialog.h"
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

MainWindow::MainWindow(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::MainWindow),
    client(Nowe::myClient())
{
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();

    //设置面板上面三个按钮的图标
    QPixmap icon1(":/images/b.png");
    ui->pushButton->setIcon(icon1);

    QPixmap icon2(":/images/a.png");
    ui->pushButton_3->setIcon(icon2);

    QPixmap icon3(":/images/c.png");
    ui->pushButton_2->setIcon(icon3);

    //设置下面按钮的图标
    QPixmap icon4(":/images/more.png");
    ui->pushButton_4->setIcon(icon4);
    //ui->pushButton->setFixedSize(icon1.size());

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

    client->logger()->setLoggingType(QXmppLogger::StdoutLogging);

    // 当服务器发送 VCard 时更新主窗口上个人资料
    connect(client->findExtension<QXmppVCardManager>(), &QXmppVCardManager::clientVCardReceived,
            this, &MainWindow::on_clientVCardReceived);

    connect(client->findExtension<QXmppRosterManager>(), &QXmppRosterManager::rosterReceived,
            this, &MainWindow::on_rosterReceived);

    connect(client->findExtension<QXmppRosterManager>(), &QXmppRosterManager::presenceChanged,
            this, &MainWindow::on_presenceChanged);

    connect(client->findExtension<QXmppRosterManager>(), &QXmppRosterManager::subscriptionReceived,
            this, &MainWindow::on_subscriptionReceived);

    loadDone=false;

    //这一部分测试用的，试着添加一部分内容
//    QTreeWidgetItem *a=createFriendGroup("123发");
//    QTreeWidgetItem *b=createFriendGroup("1235");
//    QTreeWidgetItem *c=createFriendGroup("1236775");
//    QTreeWidgetItem *d=createFriendGroup("1235656");
//    QTreeWidgetItem *e=addFriendtoGroup(a,"1","dfgd",":/images/1.png");
//    QTreeWidgetItem *f=addFriendtoGroup(a,"2","dfgd",":/images/1.png");
//    setFriendToTop(f,"new","new",":/images/4.png",a);

    //createMessage("5555","66666",":/images/3.png");
    addRoom("11","22");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMenu()
{
    //设置左下角弹出菜单的
    menu=new QMenu(this);
    QAction *setProfile=new QAction("设置个人资料",this);
    QAction *setAvatar=new QAction("设置头像",this);
    //把建立的动作添加到菜单
    menu->addAction(setProfile);
    menu->addSeparator();
    menu->addAction(setAvatar);

    connect(setProfile,&QAction::triggered,this,&MainWindow::displayProfilePanel);
    connect(setAvatar,&QAction::triggered,this,&MainWindow::displayAvatarChangePanel);


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
QTreeWidgetItem *MainWindow::createMessage(QString mainTitle,QString subTitle,QString avatarAddr)
{
    //先添加一个表项到消息列表
    QTreeWidgetItem *item1=new QTreeWidgetItem;
    ui->messageTree->addTopLevelItem(item1);
    //再把这个表项里的控件插入进去，下面都是这样
    ui->messageTree->setItemWidget(item1,0,createItem(mainTitle,avatarAddr,subTitle,true,true));
    return item1;
}

//在群组面板添加一个群组
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

QTreeWidgetItem *MainWindow::addFriendtoGroup(QTreeWidgetItem *grp,QString mainTitle,QString subTitle,QString avatarAddr)
{
    //向一个分组里添加一个好友，也就是向二级表项里插入一个三级表项
    QTreeWidgetItem *item11=new QTreeWidgetItem(grp);
    ui->friendTree->setItemWidget(item11,0,createItem(mainTitle,avatarAddr,subTitle,true,true));
    return item11;
}

QTreeWidgetItem *MainWindow::addFriendtoGroupAtTop(QTreeWidgetItem *grp,QString mainTitle,QString subTitle,QString avatarAddr)
{
    //和上面函数功能一样，但是上面是在末尾插，这个在顶部插
    QTreeWidgetItem *item11=new QTreeWidgetItem(grp,nullptr);
    QWidget *now=createItem(mainTitle,avatarAddr,subTitle,true,true);
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


QTreeWidgetItem *MainWindow::setFriendToTop(QTreeWidgetItem *toSet,QString mainTitle,QString subTitle,QString avatarAddr,QTreeWidgetItem *grp)
{
    //把某个好友放到顶部去，就是删了再加
    if(grp==nullptr)
        grp=toSet->parent();
    removeFriendOrGroup(toSet);
    return addFriendtoGroupAtTop(grp,mainTitle,subTitle,avatarAddr);
}


QWidget *MainWindow::createItem(QString mainTitle, QString iconAddr, QString subTitle, bool ifVIP=false, bool ifOnline=false)
{
    //本函数用于向一个已经存在的表项里添加控件
    QWidget *myItem=new QWidget(this);
    QLabel *mainLabel=new QLabel(this);
    QLabel *subLabel=new QLabel(this);
    QLabel *iconLabel=new QLabel(this);
    QPixmap *avatar=new QPixmap(iconAddr);
    QLabel *addr=new QLabel(iconAddr,this);
    addr->hide();
    QPixmap avatarAfter=avatar->scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

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
    vLayout->addWidget(addr);
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
    //要找到用户点击了哪个面板，从其中的label里面找到用户名等信息，发射信号
    emit msgClicked(labelList[1]->text());

    ChatDialog *chat=new ChatDialog();
    //不仅要发射信号，还要用获得的用户名等信息，创建一个聊天框
    //qDebug()<<labelList<<labelList[0]->text()<<labelList[1]->text()<<labelList[2]->text()<<labelList[3]->text();
    chat->setUserName(labelList[1]->text());
    chat->setSignature(labelList[2]->text());
    chat->setReceiver(labelList[1]->text());
    QPixmap avatar;
    avatar.load(labelList[3]->text());
    chat->setAvatar(avatar,80,80,45);
    chat->show();
    //聊天框建立
}

void MainWindow::on_friendTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    //和上面的on_messageTree_itemDoubleClicked类似，不过这个点击的是好友面板
    QWidget *now=ui->friendTree->itemWidget(item,0);
    if(item->parent())
    {
        QList<QLabel *> labelList = now->findChildren<QLabel *>();

        emit friendClicked(labelList[1]->text());
        ChatDialog *chat=new ChatDialog();
        //qDebug()<<labelList<<labelList[0]->text()<<labelList[1]->text()<<labelList[2]->text()<<labelList[3]->text();
        chat->setUserName(labelList[1]->text());
        chat->setSignature(labelList[2]->text());
        chat->setReceiver(labelList[1]->text());
        QPixmap avatar;
        avatar.load(labelList[3]->text());
        chat->setAvatar(avatar,80,80,45);
        chat->show();
    }
}

//设置头像
void MainWindow::setAvatar(QPixmap &avatar, int length, int width, int radius)
{
    //首先缩放头像
    QPixmap pixMap= avatar.scaled(width,length, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

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

void MainWindow::on_pushButton_4_clicked()
{
    //这个按钮时左下角的菜单
    QPoint pos;
    //菜单在540像素位置弹出
    pos.setY(540);
    menu->exec(this->mapToGlobal(pos));
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

    setAvatar(avatar,80,80,45);
    setMainTitle(myCard.fullName());
    setSubTitle(myCard.description());
}

// 初始化好友列表
void MainWindow::on_rosterReceived()
{
    loadDone=false;
    auto rstMng = client->findExtension<QXmppRosterManager>();
    foreach(const QString& bareJid, rstMng->getRosterBareJids()) {
       // qDebug()<<"■■■■■■■■■■■■■■■"<<"item!";
        auto item = rstMng->getRosterEntry(bareJid);
       // qDebug()<<"■■■■■■■■■■■■■■■"<<"res!";
        auto resources = rstMng->getResources(bareJid);
        QString res = resources.isEmpty() ? "" : resources[0];
       // qDebug()<<"■■■■■■■■■■■■■■■"<<"pres!";
        auto presence = rstMng->getPresence(bareJid,res);
       // qDebug()<<"■■■■■■■■■■■■■■■"<<"create!";
        createMessage(item.bareJid(), presence.statusText(), ":/images/1.png");
        //qDebug()<<"■■■■■■■■■■■■■■■"<<"add!";
        qDebug()<<item.groups()<<"\n\n\n\n\n\n\n\n\n\n\n\n";
        if(item.groups().empty())
            addFriendtoGroup(grpMng.getGrpAddr("单向好友",this),item.bareJid(),presence.statusText(),":/images/1.png");
        else
            addFriendtoGroup(grpMng.getGrpAddr(*(item.groups().begin()),this),item.bareJid(),presence.statusText(),":/images/1.png");

    }
    loadDone=true;
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

void MainWindow::updateAllFriends()
{
    flushAllFriends();
    grpMng.flush();
    on_rosterReceived();
}


void MainWindow::on_AddItemBtn_clicked()
{
    AddNewFriend *dialog=new AddNewFriend(client,this);
    dialog->show();
    dialog->setWindowModality(Qt::ApplicationModal);
    updateAllFriends();
}

void MainWindow::on_subscriptionReceived(const QString &bareJid)
{
    qDebug()<<"aaaaaaaaaaaaaaaaaaaaa"<<bareJid;
}

