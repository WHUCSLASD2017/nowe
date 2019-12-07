#ifndef GROUPCHATDIALOG_H
#define GROUPCHATDIALOG_H

#include <QWidget>

namespace Ui {
class GroupChatDialog;
}

class GroupChatDialog : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChatDialog(QWidget *parent = nullptr);
    ~GroupChatDialog();

private:
    Ui::GroupChatDialog *ui;
};

#endif // GROUPCHATDIALOG_H
