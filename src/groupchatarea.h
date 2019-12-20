#ifndef GROUPCHATAREA_H
#define GROUPCHATAREA_H

#include <QWidget>
#include <QXmppMessage.h>
#include <QTextDocument>
#include <QTextFrame>
#include <QStandardItemModel>

#include "group.h"
#include "invitefriend.h"

namespace Ui {
class GroupChatArea;
}

class GroupChatArea : public QWidget
{
    Q_OBJECT

public:
    GroupChatArea(Group * myGroup, QWidget *parent = nullptr);
    ~GroupChatArea() override;
public:
    void setPlainFormat(const QFont& target, const QColor& color);
    void setInMsgFormat(const QFont& target, const QColor& color);
    void setOutMsgFormat(const QFont& target, const QColor& color);
    void scrollBarAdjust();
    Group * group() const;

public slots:
    void on_messageReceived(const QXmppMessage &msg);
    void insertOutMessage(const QString& msg, const QDateTime& time = QDateTime::currentDateTime());
    void insertInMessage(const QString& from, const QString& msg, const QDateTime& time);
    void setTitle(const QString& newTitle);
    void renderMemberList();
    void someoneOnline(const QString& jid);
    void someoneOffline(const QString& jid);

signals:
    void newMessageSend(QString fromJID,QString toJID,QDateTime time,QString content);
    void closeBtnClick();

private slots:
    void on_sendBtn_clicked();
    void on_invite_clicked();

private:
    Ui::GroupChatArea *ui;
    Group * thisGroup;

    QStandardItemModel * model;
    QStandardItem * activeItems;
    QStandardItem * inActiveItems;

    QTextCursor savedCursorPos;

    QTextBlockFormat inMsgFormat;
    QTextBlockFormat outMsgFormat;
    QTextCharFormat inMsgCharFormat;
    QTextCharFormat outMsgCharFormat;
    QTextCharFormat plainFormat;
    QString timeFormat;
};

#endif // GROUPCHATAREA_H
