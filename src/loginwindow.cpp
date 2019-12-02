#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QXmppClient.h>
#include <QXmppVCardManager.h>

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow),
    mainwindow(new MainWindow),
    registwindow(new RegistWindow(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    connect(ui->loginBtn,&QPushButton::clicked,this,&LoginWindow::Login);
    connect(ui->registBtn,&QPushButton::clicked,this,&LoginWindow::Regist);

    // 居中显示
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);

    // 密码
    QRegExp passwd_re("^.{,16}$");
    QRegExpValidator *validator_passwd = new QRegExpValidator(passwd_re, this);

//    // JID
//    QRegExp jid_re(R"(^((?!\s).){1,10}@((?!\s).){1,10}\.((?!\s).){1,10}$)");
//    QRegExpValidator *validator_jid = new QRegExpValidator(jid_re, this);


    // ui->jidLnEdt->setValidator(validator_jid);
    ui->passwdLnEdt->setValidator(validator_passwd);


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

    connect(closeButton, &QToolButton::clicked, this, &LoginWindow::windowclosed);
    connect(minButton, &QToolButton::clicked, this, &LoginWindow::windowmin);

    //连接serve成功会发射connected()，失败发射disconnected()、error()
    connect(mainwindow->client, &QXmppClient::connected, this, &LoginWindow::loginSucceed);
    connect(mainwindow->client, &QXmppClient::disconnected, this, &LoginWindow::loginFail);
    connect(mainwindow->client, &QXmppClient::error, this, &LoginWindow::loginError);

}

LoginWindow::~LoginWindow()
{
    delete ui;
}

// 登录
void LoginWindow::Login()
{
    QString jid = ui->jidLnEdt->text();
    QString passwd = ui->passwdLnEdt->text();

    QXmppConfiguration config;
    config.setJid(jid);
    config.setPassword(passwd);
    config.setPort(5222);
    config.setIgnoreSslErrors(true);
    mainwindow->client->connectToServer(config);
}

// 跳转注册界面
void LoginWindow::Regist()
{
    registwindow->show();
}

// 鼠标拖动移动窗口位置
void LoginWindow::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


//
void LoginWindow::windowclosed()
{
    QApplication::exit();
    //this->close();
}
void LoginWindow::windowmin()
{
    this->showMinimized();
}

// 登录成功
void LoginWindow::loginSucceed()
{
    mainwindow->client->findExtension<QXmppVCardManager>()->requestClientVCard();
    mainwindow->show();
    this->hide();
}

void LoginWindow::loginFail()
{
    QMessageBox::critical(this, tr("登陆失败"), tr("登陆失败"));
}

void LoginWindow::loginError(QXmppClient::Error e)
{
    qDebug() << e;
}


