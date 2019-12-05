#ifndef NOWEBASEWINDOW_H
#define NOWEBASEWINDOW_H

#include <QWidget>

class NoweBaseWindow : public QWidget
{
    Q_OBJECT
public:
    explicit NoweBaseWindow(QWidget *parent = nullptr);

    void initNoweStyle();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QPoint last;

public slots:
    virtual void windowclosed();
    virtual void windowmin();
};

#endif // NOWEBASEWINDOW_H
