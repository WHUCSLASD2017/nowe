#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <QDialog>
#include"ChangeHeaderWnd.h"

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
    ChangeHeaderWnd ch;
    QPoint last;


public slots:
    void ChangeHeader();
    void windowclosed();
    void windowmin();

};

#endif // DATAFRAME_H
                      
