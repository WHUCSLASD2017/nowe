#include "groupchatdialog.h"
#include "ui_groupchatdialog.h"

GroupChatDialog::GroupChatDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupChatDialog)
{
    ui->setupUi(this);
}

GroupChatDialog::~GroupChatDialog()
{
    delete ui;
}
