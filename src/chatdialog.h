#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "nowebasewindow.h"
#include <QDialog>
#include <QTextDocument>
#include <QTextFrame>
#include <QDateTime>
#include <QXmppMessage.h>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void insertOutMessage(QString msg);
    void insertInMessage(QString msg,QDateTime *time=nullptr);
    void setUserName(QString usr);
    void setSignature(QString sgn);
    void setSender(QString sender);
    void setReceiver(QString receiver);
    void setPlainFormat(QFont target,Qt::GlobalColor color);
    void setAvatar(QPixmap &avatar,int length,int width,int radius);
    void setInMsgFormat(QFont target,Qt::GlobalColor color);
    void setOutMsgFormat(QFont target,Qt::GlobalColor color);
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    static QMap<QString,ChatDialog *> openedDialogs;
    QString bareJid;
    static ChatDialog *getChatDialog(QString bareJid, QString username, QString signature, QString receiver, QPixmap avatar);
    static bool ifChatDialogExist(QString jid);
    static void closeChatDialog(ChatDialog *dialog);
    QString getBareJid() const;
    void setBareJid(const QString &value);

private:
    Ui::ChatDialog *ui;
    QTextDocument *document;
    QTextFrame *rootFrame;
    QTextBlockFormat inMsgFormat;
    QTextBlockFormat outMsgFormat;
    QTextCharFormat inMsgCharFormat;
    QTextCharFormat outMsgCharFormat;
    QTextCharFormat plainFormat;
    QString sender;
    QString receiver;
    QString timeFormat;
    QPixmap *avatar;
    QTextCursor save;
    int savepos;

public slots:
    void on_messageReceived(const QXmppMessage &msg);
    void windowclosed() override;

signals:
    void newMessage(QString sender,QString receiver,QDateTime time,QString content);
private slots:
    void on_cancleBtn_clicked();
    void on_sendBtn_clicked();
    void on_messBox_cursorPositionChanged();
};

#endif // CHATDIALOG_H
