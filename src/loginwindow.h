#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "nowebasewindow.h"
#include <QDialog>
#include <QXmppClient.h>
#include <QEventLoop>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::LoginWindow *ui;
    QXmppClient *const client;
    QEventLoop *m_eventLoop;
    int m_result = QDialog::Rejected;

public slots:
    int exec();
    void accept();
    void reject();
    void Login();
    void Regist();
    void windowclosed();
    void loginSucceed();
};

#endif // LOGINWINDOW_H
