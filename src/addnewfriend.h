#ifndef ADDNEWFRIEND_H
#define ADDNEWFRIEND_H

#include <QDialog>

namespace Ui {
class AddNewFriend;
}

class AddNewFriend : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewFriend(QWidget *parent = nullptr);
    ~AddNewFriend();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    Ui::AddNewFriend *ui;
    QPoint last;

public slots:
    void windowclosed();
    void windowmin();
};

#endif // ADDNEWFRIEND_H
