#include "registwindow.h"
#include "ui_registwindow.h"

RegistWindow::RegistWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);

    setWindowTitle("会员管理系统");       //设置窗口名称
    connect(ui->pushButton_3,&QPushButton::clicked,this,&RegistWindow::Regist);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&RegistWindow::Reset);


    //11位整数
    QRegExp num("[0-9]{11}$");
    QRegExpValidator *validator_num = new QRegExpValidator(num, this);

    //密码
    QRegExp code("^[a-zA-Z0-9~!@#$%^&*()_+-=;:,./<>?`]{6,16}$");
    QRegExpValidator *validator_code = new QRegExpValidator(code, this);

    //名称
    QRegExp nam("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]{1,10}$");
    QRegExpValidator *validator_nam = new QRegExpValidator(nam, this);


    ui->lineEdit_36->setValidator(validator_num);
    ui->lineEdit_37->setValidator(validator_nam);
    ui->lineEdit_38->setValidator(validator_code);
    ui->lineEdit_39->setValidator(validator_code);


    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);


    //获取最小化、关闭按钮图标
    QPixmap minPix= style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);


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

    connect(closeButton, SIGNAL(clicked()), this, SLOT(windowclosed()) );
    connect(minButton, SIGNAL(clicked()), this, SLOT(windowmin()));

}

RegistWindow::~RegistWindow()
{
    delete ui;
}


//清空所有输入框中内容
void RegistWindow::Reset()
{

    ui->lineEdit_33->setText("");
    ui->lineEdit_36->setText("");
    ui->lineEdit_37->setText("");
    ui->lineEdit_38->setText("");
    ui->lineEdit_39->setText("");

}

//注册账号
void RegistWindow::Regist()
{/*
    //输入信息提示
    if(ui->lineEdit_33->text().isEmpty())
    {
        QMessageBox::warning(this,"", u8"请输入姓名");

    }
    else if (ui->lineEdit_36->text().isEmpty()) {
        QMessageBox::warning(this,"", u8"请输入电话");

    }
    else if (ui->lineEdit_37->text().isEmpty()) {
        QMessageBox::warning(this,"", u8"请输入账户");

    }
    else if (ui->lineEdit_38->text().isEmpty()) {
        QMessageBox::warning(this,"", u8"请输入密码");

    }

    else {

        bool ok;
        //获取输入框中的内容
        QString MANAGER_NAME = ui->lineEdit_33->text();
        int TELEPHON= ui->lineEdit_36->text().toInt(&ok,10);
        QString ACCOUNT = ui->lineEdit_37->text();
        QString CODE = ui->lineEdit_38->text();
        QString reCODE = ui->lineEdit_39->text();


        if(CODE != reCODE)
        {
            QMessageBox::warning(this,"", u8"两次输入的密码不一致！");

        }
        else
        {

            myDatabase.InsertMAN_INFO( ACCOUNT,  CODE,MANAGER_NAME, TELEPHON);


            //清空所有输入
            ui->lineEdit_33->setText("");
            ui->lineEdit_36->setText("");
            ui->lineEdit_37->setText("");
            ui->lineEdit_38->setText("");
            ui->lineEdit_39->setText("");

            QMessageBox::about(this,"", u8"注册成功！");


        }
        this->close();

        mainwindow.show();

    }

*/

}



//Mouse Move
void RegistWindow::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void RegistWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}
void RegistWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


//
void RegistWindow::windowclosed()
{
    QApplication::exit();
    //this->close();
}
void RegistWindow::windowmin()
{
    this->showMinimized();
}


