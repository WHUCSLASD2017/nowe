#ifndef ADDNEWFRIEND_H
#define ADDNEWFRIEND_H

#include <QDialog>
#include <QXmppClient.h>

namespace Ui {
class AddNewFriend;
}

class AddNewFriend : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewFriend(QXmppClient *client,QWidget *parent = nullptr);
    ~AddNewFriend();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    Ui::AddNewFriend *ui;
    QPoint last;
    QXmppClient *client;

public slots:
    void windowclosed();
    void windowmin();
private slots:
    void on_cancelButton_clicked();
    void on_confirmButton_clicked();
};

#endif // ADDNEWFRIEND_H
