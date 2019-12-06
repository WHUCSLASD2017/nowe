#include "notificationpanel.h"
#include "ui_notificationpanel.h"

NotificationPanel::NotificationPanel(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::NotificationPanel)
{
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();
}

NotificationPanel::~NotificationPanel()
{
    delete ui;
}
