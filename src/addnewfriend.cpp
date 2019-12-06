#include "addnewfriend.h"
#include "ui_addnewfriend.h"
#include "nowebasewindow.h"
#include <QMessageBox>
#include <QXmppRosterManager.h>

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
    }
}
