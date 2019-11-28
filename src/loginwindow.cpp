#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&LoginWindow::Login);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&LoginWindow::Regist);

    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);

    //密码
    QRegExp code("^[a-zA-Z0-9~!@#$%^&*()_+-=;:,./<>?`]{6,16}$");
    QRegExpValidator *validator_code = new QRegExpValidator(code, this);

    //名称
    QRegExp nam("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]{1,10}$");
    QRegExpValidator *validator_nam = new QRegExpValidator(nam, this);


    ui->lineEdit_3->setValidator(validator_nam);
    ui->lineEdit_4->setValidator(validator_code);


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

LoginWindow::~LoginWindow()
{
    delete ui;
}

//登录验证并跳转
void LoginWindow::Login()
{
/*
    QString ACCOUNT = ui->lineEdit->text();
    QString CODE = ui->lineEdit_2->text();

    QSqlQuery sql_query(myDatabase.database);
    QString sele_sql = "select CODE from MANAGER_INFORMATION where MANAGER_INFORMATION.ACCOUNT = '"+ACCOUNT+"'";

    sql_query.exec(sele_sql);
    sql_query.next();

    bool ok;
    QString N_CODE = sql_query.value(0).toString();
    if(ACCOUNT=="" || CODE =="" )
    {
        QMessageBox::warning(this,"", u8"请输入用户名或密码！");
        ui->lineEdit_2->setText("");
    }
    else {

        if(N_CODE ==  CODE)
        {
            this->close();
            mainwindow.show();
        }
        else
        {
            QMessageBox::warning(this,"", u8"用户名名或密码错误！");
            ui->lineEdit_2->setText("");

        }
    }
*/

}



//跳转注册界面
void LoginWindow::Regist()
{
    registwindow.show();
    this->close();
}

//Mouse Move
void LoginWindow::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void LoginWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}
void LoginWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


//
void LoginWindow::windowclosed()
{
    QApplication::exit();
    //this->close();
}
void LoginWindow::windowmin()
{
    this->showMinimized();
}


