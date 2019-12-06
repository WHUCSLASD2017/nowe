#include "registwindow.h"
#include "ui_registwindow.h"
#include <QXmppRegisterIq.h>
#include <QMessageBox>
#include <QXmppVCardManager.h>
#include <QXmppVCardIq.h>

RegistWindow::RegistWindow(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::RegistWindow),
    client(new QXmppClient(this))
{
    ui->setupUi(this);

    NoweBaseWindow::initNoweStyle();

    setWindowTitle("帐号注册");       //设置窗口名称
    connect(ui->registBtn,&QPushButton::clicked,this,&RegistWindow::Regist);
    connect(ui->resetBtn,&QPushButton::clicked,this,&RegistWindow::Reset);

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

void RegistWindow::iqReceived(const QXmppIq &recIq)
{
    if(recIq.type() == QXmppIq::Result) {
        client->disconnectFromServer();
        QMessageBox::critical(this, tr("注册成功"), tr("注册成功"));

        // 设置其它信息
        QXmppVCardIq iq;
        iq.setFullName(ui->nameLnEdt->text());
        QXmppVCardPhone phone;
        phone.setNumber(ui->phoneLnEdt->text());
        iq.setPhones({phone});

        connect(client, &QXmppClient::connected,[=]() {
            client->findExtension<QXmppVCardManager>()->setClientVCard(iq);
            client->disconnectFromServer();
            this->close();
        });

        auto id = ui->idLnEdt->text();
        auto passwd = ui->passwdLnEdt->text();

        QXmppConfiguration config;
        config.setUser(id);
        config.setDomain("chirsz.cc");
        config.setPassword(passwd);
        config.setPort(5222);
        config.setIgnoreSslErrors(true);
        client->connectToServer(config);
    } else if(recIq.type() == QXmppIq::Error && recIq.error().type() == QXmppIq::Error::Modify) {
        QMessageBox::critical(this, tr("注册失败"), tr("注册失败"));
    }
}


