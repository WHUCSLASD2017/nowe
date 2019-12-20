#include "chatarea.h"
#include "ui_chatarea.h"
#include <QScrollBar>
#include <QXmppUtils.h>
#include <NoweGlobal.h>

ChatArea::ChatArea(const QString& receiverJidBare, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ChatArea),
    m_receiverJidBare(receiverJidBare)
{
    ui->setupUi(this);

    inMsgFormat.setAlignment(Qt::AlignLeft);
    outMsgFormat.setAlignment(Qt::AlignRight);
    inMsgFormat.setTopMargin(5);
    outMsgFormat.setTopMargin(5);
    inMsgCharFormat.setFont(QFont("微软雅黑",9));
    inMsgCharFormat.setForeground(Qt::green);
    outMsgCharFormat.setFont(QFont("微软雅黑",9));
    outMsgCharFormat.setForeground(Qt::blue);
    timeFormat="yyyy-MM-dd hh:mm:ss";
    plainFormat.setFont(QFont("微软雅黑",10));
    plainFormat.setForeground(Qt::black);

    // 接收消息
    connect(Nowe::myClient(), &QXmppClient::messageReceived, this, &ChatArea::on_messageReceived);

    // 发送消息
    connect(this, &ChatArea::newMessageSend, [=](QString sender,QString receiver,QDateTime time,QString content) {
        QXmppMessage msg(sender, receiver, content);
        msg.setType(QXmppMessage::Chat);
        msg.setStamp(time);
        Nowe::myClient()->sendPacket(msg);
    });

    // 关闭按钮
    connect(ui->cancleBtn, &QPushButton::clicked, this, &ChatArea::closeBtnClick);

    savedCursorPos = ui->hsMsgTxtEdt->textCursor();
}

ChatArea::~ChatArea()
{
    delete ui;
}

void ChatArea::setPlainFormat(const QFont &target, const QColor& color)
{
    //设置消息正文格式
    plainFormat.setFont(target);
    plainFormat.setForeground(color);
}

void ChatArea::setInMsgFormat(const QFont &target, const QColor &color)
{
    //设置接受消息的格式
    inMsgCharFormat.setFont(target);
    inMsgCharFormat.setForeground(color);
}

void ChatArea::setOutMsgFormat(const QFont &target, const QColor &color)
{
    //设置发送消息的格式
    outMsgCharFormat.setFont(target);
    outMsgCharFormat.setForeground(color);
}

void ChatArea::setTitle(const QString &newTitle)
{
    //ui->nickNameTitle->setText(newTitle);
}

void ChatArea::on_sendBtn_clicked()
{
    //点击发送按钮后发射信号，清空文本区
    insertOutMessage(ui->contentBox->toPlainText());
    emit newMessageSend(Nowe::myJidBare(),receiverJidBare(),QDateTime::currentDateTime(),ui->contentBox->toPlainText());
    ui->contentBox->clear();
}

QString ChatArea::receiverJidBare() const
{
    return m_receiverJidBare;
}

void ChatArea::scrollBarAdjust()
{
    QScrollBar *scrollBar=ui->hsMsgTxtEdt->verticalScrollBar();
    if (scrollBar)
    {
        scrollBar->setSliderPosition(scrollBar->maximum());
    }
}

void ChatArea::on_messageReceived(const QXmppMessage &msg)
{
    if (QXmppUtils::jidToBareJid(msg.from()) == m_receiverJidBare) {
        auto time = msg.stamp();
        insertInMessage(msg.body(), time);
    }
}

void ChatArea::insertOutMessage(const QString &msg, const QDateTime &time)
{
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    // 插入对外发送消息，格式都一样，下同！
    QTextCursor cursor=ui->hsMsgTxtEdt->textCursor();
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);

    cursor.insertBlock(outMsgFormat);
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    // 插入文本块儿
    cursor.setCharFormat(outMsgCharFormat);
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    // 设置字符模式
    cursor.insertText(Nowe::myJidBare());
    cursor.insertText("  ");
    cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    cursor.insertText(msg);
    //cursor.insertText("\n");
    savedCursorPos=ui->hsMsgTxtEdt->textCursor();
    scrollBarAdjust();
}

void ChatArea::insertInMessage(const QString &msg, const QDateTime &time)
{
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    //插入接受消息，同上
    QTextCursor cursor=ui->hsMsgTxtEdt->textCursor();
    //cursor.setPosition(QTextCursor::End);
    //ui->hsMsgTxtEdt->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertBlock(inMsgFormat);
    cursor.setCharFormat(inMsgCharFormat);
    cursor.insertText(receiverJidBare());
    cursor.insertText("  ");
    cursor.insertText(time.toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    cursor.insertText(msg);
    //cursor.insertText("\n");
    scrollBarAdjust();
}

