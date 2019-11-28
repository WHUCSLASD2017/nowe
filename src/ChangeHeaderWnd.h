/*
* Description: 切换头像窗口
* Author: 公子开明 KaiMing Prince
* Detail: 从本地上传图片，进行放缩，切割等操作，选择合适范围作为头像
* Class： ChangeHeaderWnd
* Implement: 本类实现了一个具有简单操作的切换头像窗口，布局仿造QQ8.9.6上的切换头像窗口
*/
#ifndef _CHANGE_HEADER_WND_H__
#define _CHANGE_HEADER_WND_H__
#include <QWidget>
#include "ui_ChangeHeaderWnd.h"
#include"DragScrollArea.h"
#include"mainwindow.h"
#include <QFileDialog>
#include <QPainter>


class ChangeHeaderWnd : public QWidget
{
	Q_OBJECT

public:
	ChangeHeaderWnd(QWidget *parent = Q_NULLPTR);
	~ChangeHeaderWnd();

	void LoadHeader(const QPixmap& pixmap);
	void RotateHeader(bool bClock);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event);

signals:
	void updateHeader(const QPixmap& header);

protected slots:
	void onUpload();
	void onScaledChanged(int nValue);
	void onOk();
	void onCancel();

private:
	Ui::ChangeHeaderWnd ui;
	QWidget*			m_pDivWidget;			//选择头像区域的遮罩层
	QPixmap				m_sourceHeader;			//上传的原始头像文件
    QPoint last;

};
#endif //_CHANGE_HEADER_WND_H__
