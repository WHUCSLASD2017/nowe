#ifndef REGISTWINDOW_H
#define REGISTWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
//#include "database.h"

namespace Ui {
class RegistWindow;
}

class RegistWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistWindow(QWidget *parent = nullptr);
    ~RegistWindow();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::RegistWindow *ui;
    MainWindow mainwindow;
    //DataBase myDatabase;
    QPoint last;
    QXmppClient *client;


public slots:
    void Reset();
    void Regist();
    void windowclosed();
    void windowmin();

private slots:
    void iqReceived(const QXmppIq&);
};

#endif // REGISTWINDOW_H
