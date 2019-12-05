#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QXmppClient.h>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::LoginWindow *ui;
    QXmppClient *const client;
    QPoint last;

    //DataBase myDatabase;

public slots:
    void Login();
    void Regist();
    void windowclosed();
    void windowmin();
    void loginSucceed();
};

#endif // LOGINWINDOW_H
