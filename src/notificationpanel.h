#ifndef NOTIFICATIONPANEL_H
#define NOTIFICATIONPANEL_H

#include <QWidget>
#include "nowebasewindow.h"

namespace Ui {
class NotificationPanel;
}

class NotificationPanel : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit NotificationPanel(QWidget *parent = nullptr);
    ~NotificationPanel();

private:
    Ui::NotificationPanel *ui;
};

#endif // NOTIFICATIONPANEL_H
