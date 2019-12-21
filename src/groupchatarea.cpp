#include "groupchatarea.h"
#include "ui_groupchatarea.h"
#include <QScrollBar>
#include <QXmppUtils.h>
#include <NoweGlobal.h>

GroupChatArea::GroupChatArea(Group * myGroup, QWidget *parent):
    QWidget(parent),
    ui(new Ui::GroupChatArea)
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

    // 发送消息
    connect(this, &GroupChatArea::newMessageSend, [=](QString sender,QString receiver,QDateTime time,QString content) {
        QXmppMessage msg(sender, receiver, content);
        msg.setType(QXmppMessage::Chat);
        msg.setStamp(time);
        Nowe::myClient()->sendPacket(msg);
    });


    if (myGroup != nullptr) {
        thisGroup = myGroup;

        model = new QStandardItemModel(ui->contactTree);

        activeItems = new QStandardItem("在线成员");
        model->appendRow(activeItems);
        inActiveItems = new QStandardItem("下线成员");
        model->appendRow(inActiveItems);

        renderMemberList();

        ui->contactTree->header()->hide();
        ui->contactTree->setModel(model);

        connect(thisGroup->getRoom(), &QXmppMucRoom::messageReceived, this, &GroupChatArea::on_messageReceived);
        connect(thisGroup, &Group::timeToRenderMemberList, this, &GroupChatArea::renderMemberList);
        connect(thisGroup, &Group::memberJoin, this, &GroupChatArea::someoneOnline);
        connect(thisGroup, &Group::memberLeave, this, &GroupChatArea::someoneOffline);
    }

    // 关闭按钮
    connect(ui->cancleBtn, &QPushButton::clicked, this, &GroupChatArea::closeBtnClick);

    savedCursorPos = ui->hsMsgTxtEdt->textCursor();
}

GroupChatArea::~GroupChatArea()
{
    delete ui;
}

void GroupChatArea::setPlainFormat(const QFont &target, const QColor& color)
{
    //设置消息正文格式
    plainFormat.setFont(target);
    plainFormat.setForeground(color);
}

void GroupChatArea::setInMsgFormat(const QFont &target, const QColor &color)
{
    //设置接受消息的格式
    inMsgCharFormat.setFont(target);
    inMsgCharFormat.setForeground(color);
}

void GroupChatArea::setOutMsgFormat(const QFont &target, const QColor &color)
{
    //设置发送消息的格式
    outMsgCharFormat.setFont(target);
    outMsgCharFormat.setForeground(color);
}

Group *GroupChatArea::group() const
{
    return thisGroup;
}

void GroupChatArea::setTitle(const QString &newTitle)
{
    //ui->nickNameTitle->setText(newTitle);
}

void GroupChatArea::renderMemberList()
{
    activeItems->removeRows(0, activeItems->columnCount());
    inActiveItems->removeRows(0, inActiveItems->columnCount());

    for (QString activeMember : thisGroup->getActiveMembers()) {
        qDebug() << "GroupChatDialog::renderMemberList" << activeMember << endl;
        QStandardItem * activeItem = new QStandardItem(QIcon(":/images/1.png"), activeMember.replace(thisGroup->getRoomJid() + "/", "").replace(".chirsz.cc", ""));
        activeItems->appendRow(activeItem);
    }

    for (QString inActiveMember : thisGroup->getInactiveMembers()) {
        qDebug() << "GroupChatDialog::renderMemberList" << inActiveMember << endl;
        QStandardItem * inActiveItem = new QStandardItem(QIcon(":/images/1.png"), inActiveMember.replace(thisGroup->getRoomJid() + "/", "").replace(".chirsz.cc", ""));
        inActiveItems->appendRow(inActiveItem);
    }
}

void GroupChatArea::someoneOnline(const QString &jid)
{
    qDebug() << "GroupChatDialog::someoneOnline " << "jid " << jid << endl;

    QString jidString;
    int row;

    for (row = 0; row < inActiveItems->rowCount(); row++) {
        jidString = inActiveItems->child(row)->text();
        qDebug() << "GroupChatDialog::someoneOnline " << "jidString " << jidString << endl;
        if (jidString == jid) {
            break;
        }
    }

    QString jid1 = jid;

    activeItems->appendRow(new QStandardItem(QIcon(":/images/1.png"), jid1.replace(thisGroup->getRoomJid() + "/", "").replace(".chirsz.cc", "")));
    if (row < inActiveItems->rowCount()) {
        inActiveItems->removeRow(row);
    }
}

void GroupChatArea::someoneOffline(const QString &jid)
{
    qDebug() << "GroupChatDialog::someoneOffline " << "jid " << jid << endl;

    QString jidString;
    int row;

    auto jid1 = jid.split("/")[1];
    for (row = 0; row < activeItems->rowCount(); row++) {
        jidString = activeItems->child(row)->text();
        qDebug() << "GroupChatDialog::someoneOffline " << "jidString " << jidString << endl;
        if (jidString == jid1) {
            break;
        }
    }

    inActiveItems->appendRow(new QStandardItem(QIcon(":/images/1.png"), jid1.replace(thisGroup->getRoomJid() + "/", "").replace(".chirsz.cc", "")));
    if (row < activeItems->rowCount()) {
        activeItems->removeRow(row);
    }
}

void GroupChatArea::on_sendBtn_clicked()
{
    //点击发送按钮后发射信号，清空文本区
    insertOutMessage(ui->contentBox->toPlainText());
    thisGroup->getRoom()->sendMessage(ui->contentBox->toPlainText());
    ui->contentBox->clear();
}

void GroupChatArea::on_invite_clicked()
{
    InviteFriend * inviteDialog = new InviteFriend(thisGroup);
    inviteDialog->show();
}

void GroupChatArea::scrollBarAdjust()
{
    QScrollBar *scrollBar=ui->hsMsgTxtEdt->verticalScrollBar();
    if (scrollBar)
    {
        scrollBar->setSliderPosition(scrollBar->maximum());
    }
}

void GroupChatArea::on_messageReceived(const QXmppMessage &msg)
{
    QString room = QXmppUtils::jidToBareJid(msg.from());
    QString from = QXmppUtils::jidToResource(msg.from());
    QString to = QXmppUtils::jidToBareJid(msg.to());

    if (from != Nowe::myJidBare() && to == Nowe::myJidBare() && room == thisGroup->getRoomJid()) {
        auto time = msg.stamp();
        insertInMessage(from, msg.body(), time);
    }
}

void GroupChatArea::insertOutMessage(const QString &msg, const QDateTime &time)
{
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    //插入对外发送消息，格式都一样，下同！
    QTextCursor cursor=ui->hsMsgTxtEdt->textCursor();
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    //cursor.setPosition(QTextCursor::Start);
    //ui->hsMsgTxtEdt->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertBlock(outMsgFormat);
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    //插入文本块儿
    cursor.setCharFormat(outMsgCharFormat);
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    //设置字符模式
    cursor.insertText(Nowe::myJidBare());
    cursor.insertText("  ");
    cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    cursor.insertText(msg);
    //cursor.insertText("\n");
    savedCursorPos = ui->hsMsgTxtEdt->textCursor();
    scrollBarAdjust();
}

void GroupChatArea::insertInMessage(const QString &from, const QString &msg, const QDateTime &time)
{
    ui->hsMsgTxtEdt->moveCursor(QTextCursor::End);
    //插入接受消息，同上
    QTextCursor cursor=ui->hsMsgTxtEdt->textCursor();
    //cursor.setPosition(QTextCursor::End);
    //ui->messBox->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertBlock(inMsgFormat);
    cursor.setCharFormat(inMsgCharFormat);
    cursor.insertText(from);
    cursor.insertText("  ");
    cursor.insertText(time.toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    cursor.insertText(msg);
    //cursor.insertText("\n");
    scrollBarAdjust();
}

