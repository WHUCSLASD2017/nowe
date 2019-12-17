#ifndef GROUPCHATDIALOG_H
#define GROUPCHATDIALOG_H

#include <QDialog>
#include <QTextDocument>
#include <QTextFrame>
#include <QDateTime>
#include <QXmppMessage.h>
#include <QStandardItemModel>
#include <QStandardItem>

#include "nowebasewindow.h"
#include "ui_groupchatdialog.h"
#include "group.h"
#include "invitefriend.h"

namespace Ui {
class GroupChatDialog;
}

class GroupChatDialog : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit GroupChatDialog(Group * myGroup, QWidget *parent = nullptr);
    ~GroupChatDialog();

    void insertOutMessage(QString msg);
    void insertInMessage(QString from, QString msg, QDateTime *time=nullptr);
    void setUserName(QString usr);
    void setSignature(QString sgn);
    void setSender(QString sender);
    void setRoom(QString roomJid);
    void setPlainFormat(QFont target,Qt::GlobalColor color);
    void setAvatar(QPixmap &avatar,int length,int width,int radius);
    void setInMsgFormat(QFont target,Qt::GlobalColor color);
    void setOutMsgFormat(QFont target,Qt::GlobalColor color);
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    static QMap<QString,GroupChatDialog *> openedGroupChatDialogs;
    QString bareJid;
    static GroupChatDialog * getChatDialog(Group * myGroup, QString username, QString signature, QString roomName, QPixmap avatar);
    static bool ifChatDialogExist(QString jid);
    static void closeChatDialog(GroupChatDialog *dialog);
    QString getBareJid() const;
    void setBareJid(const QString &value);
    void scrollBarAdjust();

private:
    Ui::GroupChatDialog *ui;

    Group * thisGroup;

    QStandardItemModel * model;
    QStandardItem * activeItems;
    QStandardItem * inActiveItems;

    QTextDocument *document;
    QTextFrame *rootFrame;
    QTextBlockFormat inMsgFormat;
    QTextBlockFormat outMsgFormat;
    QTextCharFormat inMsgCharFormat;
    QTextCharFormat outMsgCharFormat;
    QTextCharFormat plainFormat;
    QString sender;
    QString timeFormat;
    QPixmap *avatar;
    QTextCursor save;
    int savepos;

public slots:
    void on_messageReceived(const QXmppMessage &msg);
    void windowclosed() override;
    void renderMemberList();
    void someoneOnline(QString jid);
    void someoneOffline(QString jid);

signals:
    void newMessage(QString sender,QString roomJid,QDateTime time,QString content);

private slots:
    void on_cancleBtn_clicked();
    void on_sendBtn_clicked();
    void on_messBox_cursorPositionChanged();

    void on_invite_clicked();
};

#endif // GROUPCHATDIALOG_H
