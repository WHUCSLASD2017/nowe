/*
* Description: 拖拽滚动区域窗口
* Author: 公子开明 KaiMing Prince
* Detail: 用鼠标左键拖拽进行滚动的窗口
* Class： ChangeHeaderWnd
* Implement: 本类实现了一个隐藏滚动条，用鼠标操作替代滚动效果的滚动区域窗口
*/
#ifndef _DRAG_SCROLL_AREA_H__
#define _DRAG_SCROLL_AREA_H__
#include <QScrollArea>

class DragScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    DragScrollArea(QWidget *parent = Q_NULLPTR);
    ~DragScrollArea();


protected:
    virtual bool event(QEvent* event);


};
#endif //_DRAG_SCROLL_AREA_H__
