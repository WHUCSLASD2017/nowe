#include "DragScrollArea.h"
#include <QEvent>
#include <QMouseEvent>
#include <QScrollBar>

DragScrollArea::DragScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

DragScrollArea::~DragScrollArea()
{

}

bool DragScrollArea::event(QEvent* event)
{
    if (widget())	//没有滚动窗口的时候，不处理
    {
        static QPoint point;
        if (QEvent::MouseButtonPress == event->type())
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            point = mouseEvent->pos();
        }
        else if (QEvent::MouseMove == event->type())
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (Qt::LeftButton & mouseEvent->buttons())
            {
                const QPoint& offset = mouseEvent->pos() - point;
                horizontalScrollBar()->setValue(horizontalScrollBar()->value() - offset.x()*horizontalScrollBar()->maximum() / widget()->width());
                verticalScrollBar()->setValue(verticalScrollBar()->value() - offset.y()*verticalScrollBar()->maximum() / widget()->height());
                point = mouseEvent->pos();
            }
        }
    }
    return QScrollArea::event(event);
}
