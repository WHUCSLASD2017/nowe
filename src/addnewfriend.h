#ifndef ADDNEWFRIEND_H
#define ADDNEWFRIEND_H

#include <QDialog>
#include <QXmppClient.h>
#include "nowebasewindow.h"

namespace Ui {
class AddNewFriend;
}

class AddNewFriend : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit AddNewFriend(QXmppClient *client,QWidget *parent = nullptr);
    ~AddNewFriend();
    void setLineEditContent(QString jid);

private:
    Ui::AddNewFriend *ui;
    QPoint last;
    QXmppClient *client;

private slots:
    void on_cancelButton_clicked();
    void on_confirmButton_clicked();
};

#endif // ADDNEWFRIEND_H
