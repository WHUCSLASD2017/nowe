#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "NoweGlobal.h"
#include "registwindow.h"
#include <QRegExpValidator>
#include <QMessageBox>
#include <QXmppVCardManager.h>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

LoginWindow::LoginWindow(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::LoginWindow),
    client(Nowe::myClient())
{
    ui->setupUi(this);

    NoweBaseWindow::initNoweStyle();

    connect(ui->loginBtn,&QPushButton::clicked,this,&LoginWindow::Login);
    connect(ui->registBtn,&QPushButton::clicked,this,&LoginWindow::Regist);

    // 密码
    QRegExp passwd_re("^.{,16}$");
    QRegExpValidator *validator_passwd = new QRegExpValidator(passwd_re, this);

//    // JID
//    QRegExp jid_re(R"(^((?!\s).){1,10}@((?!\s).){1,10}\.((?!\s).){1,10}$)");
//    QRegExpValidator *validator_jid = new QRegExpValidator(jid_re, this);


    // ui->jidLnEdt->setValidator(validator_jid);
    ui->passwdLnEdt->setValidator(validator_passwd);

    //连接serve成功会发射connected()，失败发射disconnected()、error()
    connect(client, &QXmppClient::connected, this, &LoginWindow::loginSucceed);
    connect(client, &QXmppClient::disconnected, [=]() {
        QMessageBox::critical(this, tr("登陆失败"), tr("与服务器断开连接"));
    });
    connect(client, &QXmppClient::error, this, [=]() {
        QMessageBox::critical(this, tr("登陆失败"), tr("登陆过程中出错"));
    });

    QGraphicsOpacityEffect * m_pGraphicsOpacityEffect1=new QGraphicsOpacityEffect(ui->ingLabel);
    m_pGraphicsOpacityEffect1->setOpacity(0);
    ui->ingLabel->setGraphicsEffect(m_pGraphicsOpacityEffect1);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::closeEvent(QCloseEvent *event)
{
    if(m_eventLoop != nullptr) {
        m_eventLoop->exit();
    }
    QWidget::closeEvent(event);
}

int LoginWindow::exec()
{
    this->show();
    m_eventLoop=new QEventLoop(this);
    m_eventLoop->exec();
    return m_result;
}

void LoginWindow::accept()
{
    m_result = QDialog::Accepted;
    this->close();
}

void LoginWindow::reject()
{
    m_result = QDialog::Rejected;
    this->close();
}

// 登录
void LoginWindow::Login()
{
    ui->colorBar->setVisible(true);
    ui->colorBar_2->setVisible(false);
    QString jid = ui->jidLnEdt->text();
    QString passwd = ui->passwdLnEdt->text();

    QXmppBookmarkManager *mybook = new QXmppBookmarkManager;
    client->addExtension(mybook);

    QPropertyAnimation *anime=new QPropertyAnimation(ui->loginLabel,"geometry");
    anime->setDuration(1000);
    anime->setStartValue(QRect(75,23,61,61));
    anime->setEndValue(QRect(-70,23,61,61));
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->start();

    QPropertyAnimation *anime2=new QPropertyAnimation(ui->ingLabel,"geometry");
    anime2->setDuration(1000);
    anime2->setStartValue(QRect(15,22,61,61));
    anime2->setEndValue(QRect(-180,22,61,61));
    anime2->setEasingCurve(QEasingCurve::OutCubic);
    anime2->start();

    QGraphicsOpacityEffect * m_pGraphicsOpacityEffect1=new QGraphicsOpacityEffect(ui->ingLabel);
    m_pGraphicsOpacityEffect1->setOpacity(0);
    ui->ingLabel->setGraphicsEffect(m_pGraphicsOpacityEffect1);

    QPropertyAnimation *anime3=new QPropertyAnimation(m_pGraphicsOpacityEffect1,"opacity");
    anime3->setDuration(200);
    anime3->setStartValue(0);
    anime3->setEndValue(1);
    anime3->setEasingCurve(QEasingCurve::OutCubic);
    anime3->start();

    QPropertyAnimation *anime5=new QPropertyAnimation(ui->mainFrame,"geometry");
    anime5->setDuration(1000);
    anime5->setStartValue(QRect(20,100,451,211));
    anime5->setEndValue(QRect(20,100,451,211));
    anime5->setKeyValueAt(0.0001,QRect(20,500,451,211));
    anime5->setKeyValueAt(0.9,QRect(20,500,451,211));
    anime5->setEasingCurve(QEasingCurve::InCubic);
    anime5->start();

    QPropertyAnimation *anime4=new QPropertyAnimation(ui->colorBar,"geometry");
    anime4->setDuration(1);
    anime4->setStartValue(QRect(0,0,500,91));
    anime4->setEndValue(QRect(0,0,500,322));
    anime4->setEasingCurve(QEasingCurve::OutCubic);
    anime4->start();

    QEventLoop eventloop2;
    QTimer::singleShot(200, &eventloop2, SLOT(quit())); //wait 2s
    eventloop2.exec();

    QPropertyAnimation *anime50=new QPropertyAnimation(ui->colorBar,"geometry");
    anime50->setDuration(500);
    anime50->setStartValue(QRect(0,0,500,322));
    anime50->setKeyValueAt(0.5,QRect(-400,0,1000,322));
    anime50->setEndValue(QRect(0,0,500,322));
    anime50->setEasingCurve(QEasingCurve::Linear);
    anime50->setLoopCount(10);
    anime50->start();

    QPropertyAnimation *animation4=new QPropertyAnimation(this,"geometry");
    animation4->setDuration(200);
    int x=frameGeometry().x();
    int y=frameGeometry().y();
    animation4->setStartValue(QRect(x,y,500,400));
    animation4->setKeyValueAt(0.25,QRect(x+30,y+30,500,400));
    animation4->setKeyValueAt(0.5,QRect(x,y+30,500,400));
    animation4->setKeyValueAt(0.75,QRect(x+30,y,500,400));
    animation4->setEndValue(QRect(x,y,500,400));
    animation4->setEasingCurve(QEasingCurve::OutBounce);
    animation4->setLoopCount(10);
    //animation4->start();

    QEventLoop eventloop;
    QTimer::singleShot(700, &eventloop, SLOT(quit())); //wait 2s
    eventloop.exec();

    QXmppConfiguration config;
    config.setJid(jid);
    config.setPassword(passwd);
    config.setPort(5222);
    config.setIgnoreSslErrors(true);
    client->connectToServer(config);

    ui->colorBar->setVisible(false);
    ui->colorBar_2->setVisible(true);
}

// 跳转注册界面
void LoginWindow::Regist()
{
    auto r = new RegistWindow(this);
    r->setAttribute(Qt::WA_DeleteOnClose);
    r->show();
}

// 向 exec() 调用处返回 QDialog::Reject
void LoginWindow::windowclosed()
{
    this->reject();
}

// 登录成功
void LoginWindow::loginSucceed()
{
    // 请求服务器发送 VCard，获取个人资料
    client->findExtension<QXmppVCardManager>()->requestClientVCard();

    // 关闭 login 窗口并在 exec 的执行处返回 QDialog::Accepted
    this->accept();
}


