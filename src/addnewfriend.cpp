#include "addnewfriend.h"
#include "ui_addnewfriend.h"
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

AddNewFriend::AddNewFriend(QXmppClient *client,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewFriend)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);

    //Show in the center
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);


    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);


    //获取最小化、关闭按钮图标
    QPixmap minPix= style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);


    //设置最小化、关闭按钮图标
    minButton->setIcon(minPix);
    closeButton->setIcon(closePix);

    //设置最小化、关闭按钮在界面的位置

    minButton->setGeometry(this->width()-46,5,20,20);
    closeButton->setGeometry(this->width()-25,5,20,20);


    //设置鼠标移至按钮上的提示信息

    minButton->setToolTip(tr("最小化"));

    closeButton->setToolTip(tr("关闭"));

    //设置最小化、关闭按钮的样式
    minButton->setStyleSheet("background:none;border:none");
    closeButton->setStyleSheet("background:none;border:none");


    connect(closeButton, SIGNAL(clicked()), this, SLOT(windowclosed()) );
    connect(minButton, SIGNAL(clicked()), this, SLOT(windowmin()));

    this->client=client;
}

AddNewFriend::~AddNewFriend()
{
    delete ui;
}

void AddNewFriend::windowclosed()
{

    close();
    //this->close();
}
void AddNewFriend::windowmin()
{
    this->showMinimized();
}

void AddNewFriend::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void AddNewFriend::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void AddNewFriend::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
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
    }
}
