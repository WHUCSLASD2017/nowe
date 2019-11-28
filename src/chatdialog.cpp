#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QBitmap>
#include <QPainter>
#include <QToolButton>
#include <QMouseEvent>
#include<QToolButton>
#include<QPixmap>
#include<QStyle>
#include <QDesktopWidget>
#include<QMessageBox>

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
                //做一些默认设置，用户调用下面的函数能覆盖掉
    ui->setupUi(this);
    document=ui->messBox->document();
    rootFrame=document->rootFrame();
    inMsgFormat.setAlignment(Qt::AlignLeft);
    outMsgFormat.setAlignment(Qt::AlignRight);
    inMsgFormat.setTopMargin(5);
    outMsgFormat.setTopMargin(5);
    inMsgCharFormat.setFont(QFont("微软雅黑",9));
    inMsgCharFormat.setForeground(Qt::green);
    outMsgCharFormat.setFont(QFont("微软雅黑",9));
    outMsgCharFormat.setForeground(Qt::blue);
    sender="发送者昵称";
    receiver="接受者昵称";
    timeFormat="yyyy-MM-dd hh:mm:ss";
    plainFormat.setFont(QFont("微软雅黑",10));
    plainFormat.setForeground(Qt::black);

                    //设置按钮1，默认是表情按钮
    QPixmap button1;
    button1.load(":/images/1.png");
    button1.scaled(20,20);
    ui->button1->setPixmap(button1);
    ui->button1->setScaledContents(true);

                    //设置按钮2，图片按钮
    QPixmap button2;
    button2.load(":/images/2.png");
    button2.scaled(20,20);
    ui->button2->setPixmap(button2);
    ui->button2->setScaledContents(true);

                //不显示标题
    setWindowFlags(Qt::FramelessWindowHint);

    //Show in the center
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);


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


    connect(closeButton, SIGNAL(clicked()), this, SLOT(windowclosed()) );
    connect(minButton, SIGNAL(clicked()), this, SLOT(windowmin()));

}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::insertOutMessage(QString msg)
{
                    //插入对外发送消息，格式都一样，下同！
      QTextCursor cursor=ui->messBox->textCursor();
      cursor.insertBlock(outMsgFormat);
                      //插入文本块儿
      cursor.setCharFormat(outMsgCharFormat);
                      //设置字符模式
      cursor.insertText(sender);
      cursor.insertText("  ");
      cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
      cursor.insertText("\n");
      cursor.setCharFormat(plainFormat);
      cursor.insertText(msg);
      //cursor.insertText("\n");
}

void ChatDialog::insertInMessage(QString msg,QDateTime *time)
{
                    //插入接受消息，同上
    QTextCursor cursor=ui->messBox->textCursor();
    cursor.insertBlock(inMsgFormat);
    cursor.setCharFormat(inMsgCharFormat);
    cursor.insertText(receiver);
    cursor.insertText("  ");
    if(time==nullptr)
                        //如果没有传入时间，就用系统当前时间
    cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
    else
        cursor.insertText(time->toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    cursor.insertText(msg);
    //cursor.insertText("\n");
}

void ChatDialog::setUserName(QString usr)
{
                    //设置用户名，显示在每次发消息的消息标题上
    sender=usr;
    ui->useTitle->setText(usr);

}

void ChatDialog::setSignature(QString sgn)
{
                    //设置标题上的个性签名
    ui->signature->setText(sgn);
}

void ChatDialog::setSender(QString sender)
{
                    //设置
    this->sender=sender;
}

void ChatDialog::setReceiver(QString receiver)
{
                    //设置接受者，显示在接受消息标题上
    this->receiver=receiver;
    ui->useTitle->setText(receiver);
}

void ChatDialog::setPlainFormat(QFont target,Qt::GlobalColor color)
{
                    //设置消息正文格式
    plainFormat.setFont(target);
    plainFormat.setForeground(color);
}

void ChatDialog::setAvatar(QPixmap &avatar, int length, int width, int radius)
{
                    //设置头像
    QPixmap pixMap= avatar.scaled(width,length, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    //搞成圆形
    pixMap =  PixmapToRound(pixMap, radius);
    ui->avaterLabel->setPixmap(pixMap);

}

void ChatDialog::setInMsgFormat(QFont target,Qt::GlobalColor color)
{
                    //设置接受消息的格式
    inMsgCharFormat.setFont(target);
    inMsgCharFormat.setForeground(color);
}

void ChatDialog::setOutMsgFormat(QFont target,Qt::GlobalColor color)
{
                    //设置发送消息的格式
    outMsgCharFormat.setFont(target);
    outMsgCharFormat.setForeground(color);
}



QPixmap ChatDialog::PixmapToRound(const QPixmap &src, int radius)
{
                    //图片变圆的
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void ChatDialog::windowclosed()
{
   close();
    //this->close();
}
void ChatDialog::windowmin()
{
    this->showMinimized();
}

void ChatDialog::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void ChatDialog::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}
void ChatDialog::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


void ChatDialog::on_pushButton_clicked()
{
                    //点击发送按钮后发射信号，清空文本区，弹出对话框
   insertOutMessage("QString msg");
   emit newMessage(sender,receiver,QDateTime::currentDateTime(),ui->contentBox->toPlainText());
   //ui->contentBox->setText("");
   //QMessageBox::information(this,"提示","消息已发送。",QMessageBox::Yes);

}

void ChatDialog::on_pushButton_2_clicked()
{
    close();
}
