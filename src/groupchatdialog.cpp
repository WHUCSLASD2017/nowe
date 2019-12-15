#include "groupchatdialog.h"

#include <QBitmap>
#include <QPainter>
#include <QPixmap>
#include <QDesktopWidget>
#include <QMessageBox>
#include <NoweGlobal.h>
#include <QXmppUtils.h>
#include <QScrollBar>

QMap<QString,GroupChatDialog *> GroupChatDialog::openedGroupChatDialogs;

GroupChatDialog::GroupChatDialog(Group * myGroup, QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::GroupChatDialog)
{
    //做一些默认设置，用户调用下面的函数能覆盖掉
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();

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

    if (thisGroup != nullptr) {
        thisGroup = myGroup;
        connect(thisGroup->getRoom(), &QXmppMucRoom::messageReceived, this, &GroupChatDialog::on_messageReceived);
        connect(thisGroup, timeToRenderMemberList, this, renderMemberList);
        connect(thisGroup, timeToModifyMemberStatus, this, modifyMemberStatus);
    }

    save=ui->messBox->textCursor();
    savepos=ui->messBox->textCursor().position();
}

GroupChatDialog::~GroupChatDialog()
{
    delete ui;
}

void GroupChatDialog::insertOutMessage(QString msg)
{
    ui->messBox->moveCursor(QTextCursor::End);
    //插入对外发送消息，格式都一样，下同！
    QTextCursor cursor=ui->messBox->textCursor();
    ui->messBox->moveCursor(QTextCursor::End);
    qDebug()<<savepos<<"\n\n\n\n\n\n\n\n\n\n\n\n";
    //cursor.setPosition(QTextCursor::Start);
    //ui->messBox->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertBlock(outMsgFormat);
    ui->messBox->moveCursor(QTextCursor::End);
    //插入文本块儿
    cursor.setCharFormat(outMsgCharFormat);
    ui->messBox->moveCursor(QTextCursor::End);
    //设置字符模式
    cursor.insertText(sender);
    cursor.insertText("  ");
    cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    ui->messBox->moveCursor(QTextCursor::End);
    cursor.insertText(msg);
    //cursor.insertText("\n");
    save=ui->messBox->textCursor();
    savepos=ui->messBox->textCursor().position();
    scrollBarAdjust();
}

void GroupChatDialog::insertInMessage(QString from, QString msg, QDateTime *time)
{
    ui->messBox->moveCursor(QTextCursor::End);
    //插入接受消息，同上
    QTextCursor cursor=ui->messBox->textCursor();
    //cursor.setPosition(QTextCursor::End);
    //ui->messBox->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertBlock(inMsgFormat);
    cursor.setCharFormat(inMsgCharFormat);
    cursor.insertText(from);
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
    scrollBarAdjust();
}

void GroupChatDialog::setUserName(QString usr)
{
    //设置用户名，显示在每次发消息的消息标题上
    sender=usr;
    ui->useTitle->setText(usr);

}

void GroupChatDialog::setSignature(QString sgn)
{
    //设置标题上的个性签名
    ui->signature->setText(sgn);
}

void GroupChatDialog::setSender(QString sender)
{
    //设置
    this->sender=sender;
}

void GroupChatDialog::setRoom(QString roomJid)
{
    //设置群组名，显示在接受消息标题上
    ui->useTitle->setText(QXmppUtils::jidToBareJid(roomJid));
}

void GroupChatDialog::setPlainFormat(QFont target,Qt::GlobalColor color)
{
    //设置消息正文格式
    plainFormat.setFont(target);
    plainFormat.setForeground(color);
}

void GroupChatDialog::setAvatar(QPixmap &avatar, int length, int width, int radius)
{
    //设置头像
    QPixmap pixMap= avatar.scaled(width,length, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //搞成圆形
    pixMap =  PixmapToRound(pixMap, radius);
    ui->avaterLabel->setPixmap(pixMap);

}

void GroupChatDialog::setInMsgFormat(QFont target,Qt::GlobalColor color)
{
    //设置接受消息的格式
    inMsgCharFormat.setFont(target);
    inMsgCharFormat.setForeground(color);
}

void GroupChatDialog::setOutMsgFormat(QFont target,Qt::GlobalColor color)
{
    //设置发送消息的格式
    outMsgCharFormat.setFont(target);
    outMsgCharFormat.setForeground(color);
}

void GroupChatDialog::on_messageReceived(const QXmppMessage &msg)
{
    QString room = msg.from().split("/")[0];
    QString from = msg.from().split("/")[1];

    if (room == thisGroup->getRoomJid()) {
        auto time = msg.stamp();
        insertInMessage(from, msg.body(), &time);
    }
}

void GroupChatDialog::windowclosed()
{
    closeChatDialog(this);
    close();
}

QPixmap GroupChatDialog::PixmapToRound(const QPixmap &src, int radius)
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

QString GroupChatDialog::getBareJid() const
{
    return bareJid;
}

void GroupChatDialog::setBareJid(const QString &value)
{
    bareJid = value;
}

void GroupChatDialog::scrollBarAdjust()
{
    QScrollBar *scrollBar=ui->messBox->verticalScrollBar();
    if (scrollBar)
    {
        scrollBar->setSliderPosition(scrollBar->maximum());
    }
}

void GroupChatDialog::on_sendBtn_clicked()
{
    //点击发送按钮后发射信号，清空文本区
    insertOutMessage(ui->contentBox->toPlainText());
    thisGroup->getRoom()->sendMessage(ui->contentBox->toPlainText());
    ui->contentBox->clear();
}

void GroupChatDialog::on_cancleBtn_clicked()
{
    closeChatDialog(this);
    close();
}

void GroupChatDialog::on_messBox_cursorPositionChanged()
{
    //ui->messBox->moveCursor(QTextCursor::End);
}

// 获取聊天窗口的指针，如果不存在，则打开一个新的聊天窗口
GroupChatDialog *GroupChatDialog::getChatDialog(Group * myGroup, QString username, QString signature, QPixmap avatar)
{
    if (myGroup == nullptr) {
        qDebug() << "null Group" << endl;
        return nullptr;
    }

    QMap<QString,GroupChatDialog *>::const_iterator i = openedGroupChatDialogs.find(myGroup->getRoomJid());
    if (i==openedGroupChatDialogs.end()) {
        GroupChatDialog *chat=new GroupChatDialog(myGroup);
        chat->setUserName(username);
        chat->setBareJid(myGroup->getRoomJid());
        chat->setSignature(signature);
        chat->setAvatar(avatar,80,80,45);
        chat->setRoom(QXmppUtils::jidToBareJid(myGroup->getRoomJid()));
        chat->show();
        openedGroupChatDialogs.insert(bareJid, chat);
        return chat;
    } else {
        return i.value();
    }
}

bool GroupChatDialog::ifChatDialogExist(QString jid)
{
    QMap<QString,GroupChatDialog *>::const_iterator i= openedGroupChatDialogs.find(jid);
    return i != openedGroupChatDialogs.end();

}

void GroupChatDialog::closeChatDialog(GroupChatDialog *dialog)
{
    QString jid = openedGroupChatDialogs.key(dialog);
    QMap<QString,GroupChatDialog *>::iterator i = openedGroupChatDialogs.find(jid);
    openedGroupChatDialogs.erase(i);
    qDebug()<<"             erase!";
}
