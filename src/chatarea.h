#ifndef CHATAREA_H
#define CHATAREA_H

#include <QWidget>
#include <QXmppMessage.h>
#include <QTextDocument>
#include <QTextFrame>

namespace Ui {
class ChatArea;
}

class ChatArea : public QWidget
{
    Q_OBJECT

public:
    ChatArea(const QString& receiverJidBare, QWidget *parent = nullptr);
    ~ChatArea();
public:
    void setPlainFormat(const QFont& target, const QColor& color);
    void setInMsgFormat(const QFont& target, const QColor& color);
    void setOutMsgFormat(const QFont& target, const QColor& color);
    QString receiverJidBare() const;
    void scrollBarAdjust();

public slots:
    void on_messageReceived(const QXmppMessage &msg);
    void insertOutMessage(const QString& msg, const QDateTime& time = QDateTime::currentDateTime());
    void insertInMessage(const QString& msg, const QDateTime& time);
    void setTitle(const QString& newTitle);

signals:
    void newMessageSend(QString fromJID,QString toJID,QDateTime time,QString content);
    void closeBtnClick();

private slots:
    void on_sendBtn_clicked();

private:
    Ui::ChatArea *ui;
    QString m_receiverJidBare;
    QTextCursor savedCursorPos;

    QTextBlockFormat inMsgFormat;
    QTextBlockFormat outMsgFormat;
    QTextCharFormat inMsgCharFormat;
    QTextCharFormat outMsgCharFormat;
    QTextCharFormat plainFormat;
    QString timeFormat;
};

#endif // CHATAREA_H
