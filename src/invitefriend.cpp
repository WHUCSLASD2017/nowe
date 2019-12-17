#include "invitefriend.h"
#include "ui_invitefriend.h"
#include "NoweGlobal.h"

InviteFriend::InviteFriend(Group * myGroup, QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::InviteFriend)
{
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();
    this->client=Nowe::myClient();
    thisGroup = myGroup;
}

InviteFriend::~InviteFriend()
{
    delete ui;
}

void InviteFriend::on_cancelBtn_clicked()
{
    close();
}

void InviteFriend::on_inviteBtn_clicked()
{
    QString jid = ui->jidEdt->text();
    if(jid == ""){
        QMessageBox::critical(this,"输入非法","请输入好友jid！");
    }
    else {
        inviteFriend(jid);
    }
}


//邀请好友
void InviteFriend::inviteFriend(QString jid)
{
    thisGroup->inviteNewMember(jid);
    close();

}
