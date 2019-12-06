#include "dataframe.h"
#include "ui_dataframe.h"
#include "NoweGlobal.h"
#include <QToolButton>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QXmppVCardManager.h>
#include <QBuffer>
#include <QImageReader>

DataFrame::DataFrame(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::DataFrame)
{
    //***************************************
    //初始化
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();

    // 每次服务器发送 VCard 时自动更新
    auto myVCardManager = Nowe::myClient()->findExtension<QXmppVCardManager>();
    connect(myVCardManager, &QXmppVCardManager::clientVCardReceived, [=]() {
        updatePanel(Nowe::myVCard());
    });
//    connect(ui->avatarLineedit,SIGNAL(clicked()), this, SLOT(ChangeHeader()));

    // 初始化个人资料面板
    updatePanel(Nowe::myVCard());

    // JID 设置为不可编辑状态
    ui->jid->setText(Nowe::myJid());
    ui->jid->setDisabled(true);
}

DataFrame::~DataFrame()
{
    delete ui;
}

void DataFrame::on_ok_clicked()
{
    if(ui->name->text().isEmpty()||ui->instruction->toPlainText().isEmpty()) {
        ui->stateLabel->setText("不可以为空!");
    } else {
        sendNewVCard();
        ui->stateLabel->setText("修改成功!");
    }
}

// 向服务器发送新 VCard 以更新资料
void DataFrame::sendNewVCard()
{
    // 每次必须发送所有字段，无论是否改动，
    // 所以在原 VCard 的基础上编辑
    auto myVCard =  Nowe::myVCard();
    auto myVCardManager = Nowe::myClient()->findExtension<QXmppVCardManager>();

    myVCard.setFullName(ui->name->text());
    myVCard.setDescription(ui->instruction->toPlainText());

    myVCardManager->setClientVCard(myVCard);
    myVCardManager->requestClientVCard();
}

// 根据传入的 VCard 更新个人资料面板
void DataFrame::updatePanel(const QXmppVCardIq &vcard)
{
    QBuffer buffer;
    buffer.setData(vcard.photo());
    buffer.open(QIODevice::ReadOnly);
    QImageReader avaterReader(&buffer);
    QPixmap avatar = QPixmap::fromImage(avaterReader.read());

    ui->name->setText(vcard.fullName());
    ui->instruction->setText(vcard.description());
    ui->avatar->setPixmap(avatar.scaled(ui->avatar->size()));
}

