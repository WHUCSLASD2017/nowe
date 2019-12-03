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
    QDialog(parent),
    ui(new Ui::DataFrame)
{
    //***************************************
    //初始化
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    //获取最小化、关闭按钮图标
    QPixmap minPix= style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);
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


    //信号与槽
    connect(closeButton, &QToolButton::clicked, this, &DataFrame::windowclosed);
    connect(minButton, &QToolButton::clicked, this, &DataFrame::windowmin);

    auto myVCardManager = Nowe::myClient()->findExtension<QXmppVCardManager>();
    connect(myVCardManager, &QXmppVCardManager::clientVCardReceived, this, &DataFrame::on_clientVCardReceived);
//    connect(ui->avatarLineedit,SIGNAL(clicked()), this, SLOT(ChangeHeader()));

    myVCardManager->requestClientVCard();

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
        sendVCard();
        ui->stateLabel->setText("修改成功!");
    }
}

void DataFrame::sendVCard()
{
    auto myVCard =  Nowe::myVCard();
    auto myVCardManager = Nowe::myClient()->findExtension<QXmppVCardManager>();

    QBuffer avatarData;
    avatarData.open(QIODevice::WriteOnly);
    auto pixmap = ui->avatar->pixmap();
    pixmap->save(&avatarData, "PNG");

    myVCard.setFullName(ui->name->text());
    myVCard.setDescription(ui->instruction->toPlainText());
    myVCard.setPhoto(avatarData.buffer());

    myVCardManager->setClientVCard(myVCard);
    myVCardManager->requestClientVCard();
}

void DataFrame::on_clientVCardReceived()
{
    const auto& myCard =  Nowe::myVCard();
    QBuffer buffer;
    buffer.setData(myCard.photo());
    buffer.open(QIODevice::ReadOnly);
    QImageReader avaterReader(&buffer);
    QPixmap avatar = QPixmap::fromImage(avaterReader.read());

    ui->name->setText(myCard.fullName());
    ui->instruction->setText(myCard.description());
    ui->avatar->setPixmap(avatar);
}


//鼠标移动更改窗口位置
void DataFrame::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void DataFrame::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void DataFrame::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

void DataFrame::windowclosed()
{
    this->close();
}
void DataFrame::windowmin()
{
    this->showMinimized();
}

//**************************************************
//未用
void DataFrame::ChangeHeader()
{
    ch.show();
}

