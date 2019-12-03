#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <QDialog>
#include"ChangeHeaderWnd.h"
#include"NoweGlobal.h"
namespace Ui {
class DataFrame;
}

class DataFrame : public QDialog
{
    Q_OBJECT

public:
    explicit DataFrame(QWidget *parent = nullptr);
    ~DataFrame();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::DataFrame *ui;
    QPoint last;


public slots:
//    void ChangeHeader();
    void windowclosed();
    void windowmin();

private slots:
    void on_ok_clicked();
    void sendVCard();
    void on_clientVCardReceived();
};

#endif // DATAFRAME_H

