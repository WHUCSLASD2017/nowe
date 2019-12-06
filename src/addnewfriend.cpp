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
#include "nowebasewindow.h"

AddNewFriend::AddNewFriend(QXmppClient *client,QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::AddNewFriend)
{
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();

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
