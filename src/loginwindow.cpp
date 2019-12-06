#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "NoweGlobal.h"
#include "registwindow.h"
#include <QRegExpValidator>
#include <QMessageBox>
#include <QXmppVCardManager.h>
#include <QCloseEvent>

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
    QString jid = ui->jidLnEdt->text();
    QString passwd = ui->passwdLnEdt->text();

    QXmppConfiguration config;
    config.setJid(jid);
    config.setPassword(passwd);
    config.setPort(5222);
    config.setIgnoreSslErrors(true);
    client->connectToServer(config);
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


