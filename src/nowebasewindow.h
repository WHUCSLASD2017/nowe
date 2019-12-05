#ifndef NOWEBASEWINDOW_H
#define NOWEBASEWINDOW_H

// Nowe 项目中统一风格的基类，取消系统标题栏、使用自定义风格的最小化和关闭按钮，允许使用鼠标拖动窗口。
// 注意：请在 setUp(ui) 后调用 NoweBaseWindow::initNoweStyle() 以实现它的功能。

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
