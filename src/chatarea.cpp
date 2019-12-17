#include "chatarea.h"
#include "ui_chatarea.h"

ChatArea::ChatArea(const QString& bareJID, const QString& nickName, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ChatArea),
    toBareJID(bareJID)
{
    ui->setupUi(this);
    ui->nickNameTitle->setText(nickName);
}

ChatArea::~ChatArea()
{
    delete ui;
}
