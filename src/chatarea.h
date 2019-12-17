#ifndef CHATAREA_H
#define CHATAREA_H

#include <QWidget>

namespace Ui {
class ChatArea;
}

class ChatArea : public QWidget
{
    Q_OBJECT

public:
    ChatArea(const QString& bareJID, const QString& nickName, QWidget *parent = nullptr);
    ~ChatArea();

private:
    Ui::ChatArea *ui;
    QString toBareJID;
};

#endif // CHATAREA_H
