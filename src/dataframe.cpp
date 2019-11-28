#include "dataframe.h"
#include "ui_dataframe.h"

DataFrame::DataFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataFrame)
{
    //***************************************
    //初始化
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    //获取最小化、关闭按钮图标
    QPixmap minPix= style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);
    //设置最小化、关闭按钮图标
    minButton->setIcon(minPix);
    closeButton->setIcon(closePix);
    //设置最小化、关闭按钮在界面的位置
    minButton->setGeometry(this->width()-46,5,20,20);
    closeButton->setGeometry(this->width()-25,5,20,20);
    //设置鼠标移至按钮上的提示信息
    minButton->setToolTip(tr("最小化"));
    closeButton->setToolTip(tr("关闭"));
    //设置最小化、关闭按钮的样式
    minButton->setStyleSheet("background:none;border:none");
    closeButton->setStyleSheet("background:none;border:none");


    //信号与槽
    connect(closeButton, SIGNAL(clicked()), this, SLOT(windowclosed()) );
    connect(minButton, SIGNAL(clicked()), this, SLOT(windowmin()));

//    connect(ui->avatarLineedit,SIGNAL(clicked()), this, SLOT(ChangeHeader()));
}

DataFrame::~DataFrame()
{
    delete ui;
}


void DataFrame::setInfo(infoDate info){
    ui->name->setText(info.nickname);
    ui->jid->setText(info.jid);
    ui->jid->setEnabled(false);
    ui->instruction->setText(info.descrition);
    QPixmap pixMap=QPixmap::fromImage(info.avatar);
    ui->avatar->setPixmap(pixMap);
}


void DataFrame::sendInfo(){
    infoDate info;
    info.jid=ui->jid->text();
    info.nickname=ui->name->text();
    info.descrition=ui->instruction->toPlainText();
    emit infoChange(info);
}

void DataFrame::on_ok_clicked()
{
    if(ui->name->text().isEmpty()||ui->instruction->toPlainText().isEmpty()){
        ui->stateLabel->setText("不可以为空!");
    }else{
        sendInfo();
        ui->stateLabel->setText("修改成功!");
        emit update();
    }
}


//鼠标移动更改窗口位置
void DataFrame::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void DataFrame::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}
void DataFrame::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

void DataFrame::windowclosed()
{
    this->close();
}
void DataFrame::windowmin()
{
    this->showMinimized();
}

//**************************************************
//未用
void DataFrame::ChangeHeader()
{
  ch.show();
}

