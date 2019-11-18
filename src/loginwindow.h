#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
//#include "database.h"
#include"registwindow.h"
#include "mainwindow.h"


namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
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
    MainWindow mainwindow;
    RegistWindow registwindow;
    QPoint last;

    //DataBase myDatabase;

public slots:
    void Login();
    void Regist();
    void windowclosed();
    void windowmin();

};

#endif // LOGINWINDOW_H
