#ifndef REGISTWINDOW_H
#define REGISTWINDOW_H

#include "nowebasewindow.h"
#include <QXmppClient.h>

namespace Ui {
class RegistWindow;
}

class RegistWindow : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit RegistWindow(QWidget *parent = nullptr);
    ~RegistWindow();

private:
    Ui::RegistWindow *ui;
    QXmppClient *client;

public slots:
    void Reset();
    void Regist();

private slots:
    void iqReceived(const QXmppIq&);
};

#endif // REGISTWINDOW_H
