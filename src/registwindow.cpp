#include "registwindow.h"
#include "ui_registwindow.h"
#include <QXmppRegisterIq.h>
#include <QDesktopWidget>
#include <QToolButton>
#include <QMessageBox>
#include <QMouseEvent>

RegistWindow::RegistWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistWindow),
    client(new QXmppClient(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);

    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);

    setWindowTitle("帐号注册");       //设置窗口名称
    connect(ui->registBtn,&QPushButton::clicked,this,&RegistWindow::Regist);
    connect(ui->resetBtn,&QPushButton::clicked,this,&RegistWindow::Reset);

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

    connect(closeButton, &QToolButton::clicked, this, &RegistWindow::windowclosed);
    connect(minButton, &QToolButton::clicked, this, &RegistWindow::windowmin);

    //如果服务器收到注册的packet会返回iqReceived()信号，获取信号并处理
    connect(client, &QXmppClient::iqReceived, this, &RegistWindow::iqReceived);

    QXmppConfiguration config;

    config.setHost("chirsz.cc");
    config.setIgnoreSslErrors(true);
    config.setUseSASLAuthentication(false);
    client->connectToServer(config);
}

RegistWindow::~RegistWindow()
{
    delete ui;
}


//清空所有输入框中内容
void RegistWindow::Reset()
{

    ui->nameLnEdt->clear();
    ui->phoneLnEdt->clear();
    ui->idLnEdt->clear();
    ui->passwdLnEdt->clear();
    ui->confirmPasswdLnEdt->clear();

}

//注册账号
void RegistWindow::Regist()
{
    QString id = ui->idLnEdt->text();
    QString passwd = ui->passwdLnEdt->text();
    QString confirm = ui->confirmPasswdLnEdt->text();

    if (passwd.isEmpty() || confirm.isEmpty()) {
        QMessageBox::critical(this, tr("注册信息有误"), tr("请输入密码和确认密码！"));
        return;
    } else if (id.isEmpty()) {
        QMessageBox::critical(this, tr("注册信息有误"), tr("请输入用户id！"));
        return;
    } else if (passwd != confirm) {
        QMessageBox::critical(this, tr("密码不一致！"), tr("密码不一致！"));
        return;
    }

    QXmppRegisterIq iq;
    iq.setType(QXmppIq::Set);
    iq.setTo("chirsz.cc");
    iq.setUsername(id);
    iq.setPassword(passwd);
    client->sendPacket(iq);

}



//Mouse Move
void RegistWindow::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void RegistWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void RegistWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


//
void RegistWindow::windowclosed()
{
    this->close();
}
void RegistWindow::windowmin()
{
    this->showMinimized();
}

void RegistWindow::iqReceived(const QXmppIq &recIq)
{
    if(recIq.type() == QXmppIq::Result)
    {
        QMessageBox::critical(this, tr("注册成功"), tr("注册成功"));
    } else if(recIq.type() == QXmppIq::Error && recIq.error().type() == QXmppIq::Error::Modify)
    {
        QMessageBox::critical(this, tr("注册失败"), tr("注册失败"));
    }
}


