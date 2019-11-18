#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include<QTextDocument>
#include<QTextFrame>
#include<QDateTime>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
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
    QPoint last;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

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

public slots:
    void windowclosed();
    void windowmin();

signals:
    void newMessage(QString sender,QString receiver,QDateTime time,QString content);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // CHATDIALOG_H
