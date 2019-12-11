#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QBitmap>
#include <QPainter>
#include <QPixmap>
#include <QDesktopWidget>
#include <QMessageBox>
#include <NoweGlobal.h>
#include <QXmppUtils.h>
#include <QScrollBar>

QMap<QString,ChatDialog *> ChatDialog::openedDialogs;

ChatDialog::ChatDialog(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::ChatDialog)
{
    //做一些默认设置，用户调用下面的函数能覆盖掉
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();

    document=ui->messBox->document();
    rootFrame=document->rootFrame();
    inMsgFormat.setAlignment(Qt::AlignLeft);
    outMsgFormat.setAlignment(Qt::AlignRight);
    inMsgFormat.setTopMargin(5);
    outMsgFormat.setTopMargin(5);
    inMsgCharFormat.setFont(QFont("微软雅黑",9));
    inMsgCharFormat.setForeground(Qt::green);
    outMsgCharFormat.setFont(QFont("微软雅黑",9));
    outMsgCharFormat.setForeground(Qt::blue);
    sender="发送者昵称";
    receiver="接受者昵称";
    timeFormat="yyyy-MM-dd hh:mm:ss";
    plainFormat.setFont(QFont("微软雅黑",10));
    plainFormat.setForeground(Qt::black);

    connect(Nowe::myClient(), &QXmppClient::messageReceived, this, &ChatDialog::on_messageReceived);
    connect(this, &ChatDialog::newMessage, [=](QString sender,QString receiver,QDateTime time,QString content) {
        QXmppMessage msg(sender, receiver, content);
        msg.setType(QXmppMessage::Chat);
        msg.setStamp(time);
        Nowe::myClient()->sendPacket(msg);
    });
    connect(this, &ChatDialog::newPicture, [=](QString sender,QString receiver,QDateTime time,QString content) {
        QXmppMessage msg(sender, receiver, content);
        msg.setType(QXmppMessage::Chat);
        msg.setStamp(time);
        Nowe::myClient()->sendPacket(msg);
    });

    save=ui->messBox->textCursor();
    savepos=ui->messBox->textCursor().position();
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::insertOutMessage(QString msg)
{
    ui->messBox->moveCursor(QTextCursor::End);
    //插入对外发送消息，格式都一样，下同！
    QTextCursor cursor=ui->messBox->textCursor();
    ui->messBox->moveCursor(QTextCursor::End);
    qDebug()<<savepos<<"\n\n\n\n\n\n\n\n\n\n\n\n";
    //cursor.setPosition(QTextCursor::Start);
    //ui->messBox->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertBlock(outMsgFormat);
    ui->messBox->moveCursor(QTextCursor::End);
    //插入文本块儿
    cursor.setCharFormat(outMsgCharFormat);
    ui->messBox->moveCursor(QTextCursor::End);
    //设置字符模式
    cursor.insertText(sender);
    cursor.insertText("  ");
    cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    ui->messBox->moveCursor(QTextCursor::End);
    cursor.insertText(msg);
    cursor.insertText("\n");
    save=ui->messBox->textCursor();
    savepos=ui->messBox->textCursor().position();
    scrollBarAdjust();
}

void ChatDialog::insertInMessage(QString msg,QDateTime *time)
{
    ui->messBox->moveCursor(QTextCursor::End);
    //插入接受消息，同上
    QTextCursor cursor=ui->messBox->textCursor();
    //cursor.setPosition(QTextCursor::End);
    //ui->messBox->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertBlock(inMsgFormat);
    cursor.setCharFormat(inMsgCharFormat);
    cursor.insertText(receiver);
    cursor.insertText("  ");
    if(time==nullptr)
        //如果没有传入时间，就用系统当前时间
        cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
    else
        cursor.insertText(time->toString(timeFormat));
    cursor.insertText("\n");
    cursor.setCharFormat(plainFormat);
    cursor.insertText(msg);
    //cursor.insertText("\n");
    scrollBarAdjust();
}

void ChatDialog::insertOutPicture(QImage *image)
{
    ui->messBox->moveCursor(QTextCursor::End);
    QTextCursor cursor=ui->messBox->textCursor();
    ui->messBox->moveCursor(QTextCursor::End);
    cursor.insertBlock(outMsgFormat);
    ui->messBox->moveCursor(QTextCursor::End);
    cursor.insertImage(*image);
    cursor.insertText("\n");
    save=ui->messBox->textCursor();
    savepos=ui->messBox->textCursor().position();
    scrollBarAdjust();
}

void ChatDialog::insertInPicture(QImage *image,QDateTime *time)
{
    ui->messBox->moveCursor(QTextCursor::End);
    QTextCursor cursor=ui->messBox->textCursor();
    cursor.insertBlock(inMsgFormat);
    cursor.setCharFormat(inMsgCharFormat);
    cursor.insertText(receiver);
    cursor.insertText("  ");
    if(time==nullptr)
        //如果没有传入时间，就用系统当前时间
        cursor.insertText(QDateTime::currentDateTime().toString(timeFormat));
    else
        cursor.insertText(time->toString(timeFormat));
    cursor.insertText("\n");

    cursor.insertImage(*image);
    cursor.insertText("\n");
    scrollBarAdjust();
}

void ChatDialog::setUserName(QString usr)
{
    //设置用户名，显示在每次发消息的消息标题上
    sender=usr;
    ui->useTitle->setText(usr);

}

void ChatDialog::setSignature(QString sgn)
{
    //设置标题上的个性签名
    ui->signature->setText(sgn);
}

void ChatDialog::setSender(QString sender)
{
    //设置
    this->sender=sender;
}

void ChatDialog::setReceiver(QString receiver)
{
    //设置接受者，显示在接受消息标题上
    this->receiver=receiver;
    ui->useTitle->setText(receiver);
}

void ChatDialog::setPlainFormat(QFont target,Qt::GlobalColor color)
{
    //设置消息正文格式
    plainFormat.setFont(target);
    plainFormat.setForeground(color);
}

void ChatDialog::setAvatar(QPixmap &avatar, int length, int width, int radius)
{
    //设置头像
    QPixmap pixMap= avatar.scaled(width,length, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //搞成圆形
    pixMap =  PixmapToRound(pixMap, radius);
    ui->avaterLabel->setPixmap(pixMap);

}

void ChatDialog::setInMsgFormat(QFont target,Qt::GlobalColor color)
{
    //设置接受消息的格式
    inMsgCharFormat.setFont(target);
    inMsgCharFormat.setForeground(color);
}

void ChatDialog::setOutMsgFormat(QFont target,Qt::GlobalColor color)
{
    //设置发送消息的格式
    outMsgCharFormat.setFont(target);
    outMsgCharFormat.setForeground(color);
}

void ChatDialog::on_messageReceived(const QXmppMessage &msg)
{
    if (QXmppUtils::jidToBareJid(msg.from()) == bareJid)
    {
        auto time = msg.stamp();

        if(msg.body().mid(0,3)=="URL")
        {
            QString url_string=msg.body().mid(4,-1);
            QUrl url(url_string);
            qDebug()<<"QUrl="<<url;

            QNetworkRequest request;
            request.setUrl(url);

            //网络载体
            QNetworkAccessManager *networkAccessManager2=new QNetworkAccessManager(this);
            //网络载体的响应接收信号，与响应接收槽绑定
            connect(networkAccessManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoadPicture(QNetworkReply*)));
            networkAccessManager2->get(request);
        }
        else
        {
            insertInMessage(msg.body(), &time);
        }
    }
}

void ChatDialog::windowclosed()
{
    closeChatDialog(this);
    close();
}

QPixmap ChatDialog::PixmapToRound(const QPixmap &src, int radius)
{
    //图片变圆的
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

QString ChatDialog::getBareJid() const
{
    return bareJid;
}

void ChatDialog::setBareJid(const QString &value)
{
    bareJid = value;
}

void ChatDialog::scrollBarAdjust()
{
    QScrollBar *scrollBar=ui->messBox->verticalScrollBar();
    if (scrollBar)
    {
        scrollBar->setSliderPosition(scrollBar->maximum());
    }
}

void ChatDialog::on_sendBtn_clicked()
{
    //点击发送按钮后发射信号，清空文本区
    QString text=ui->contentBox->toPlainText();
    if(text!="")
    {
        qDebug()<<"Send Text is not Empty!";
        qDebug()<<text;
        insertOutMessage(text);
        emit newMessage(sender,receiver,QDateTime::currentDateTime(),text);
    }

    if(!strFilePath.isEmpty())
    {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"smfile\"; filename=\"image.png\""));

        QFile *file = new QFile(strFilePath);
        if(!file->open(QIODevice::ReadOnly))
        {
            qDebug()<<"Open file failed!";
        }
        else
        {
            imagePart.setBodyDevice(file);
            multiPart->append(imagePart);
        }

        QNetworkRequest request;
        request.setUrl(QUrl("https://sm.ms/api/upload"));
        //网络载体
        QNetworkAccessManager *networkAccessManager1=new QNetworkAccessManager(this);
        //网络载体的响应接收信号，与响应接收槽绑定
        connect(networkAccessManager1, SIGNAL(finished(QNetworkReply*)), this, SLOT(Generatelink(QNetworkReply*)));
        networkAccessManager1->post(request,multiPart);
        insertOutPicture(image);
    }

    //clear
    ui->contentBox->clear();
    picUrl="";
    strFilePath="";
}

void ChatDialog::on_cancleBtn_clicked()
{
    closeChatDialog(this);
    close();
}

void ChatDialog::on_messBox_cursorPositionChanged()
{
    //ui->messBox->moveCursor(QTextCursor::End);
}

// 获取聊天窗口的指针，如果不存在，则打开一个新的聊天窗口
ChatDialog *ChatDialog::getChatDialog(QString bareJid,QString username,QString signature,QString receiver,QPixmap avatar)
{

    QMap<QString,ChatDialog *>::const_iterator i= openedDialogs.find(bareJid);
    if(i==openedDialogs.end())
    {
        ChatDialog *chat=new ChatDialog();
        chat->setUserName(username);
        chat->setBareJid(bareJid);
        chat->setSignature(signature);
        chat->setReceiver(receiver);
        chat->setAvatar(avatar,80,80,45);
        chat->show();
        openedDialogs.insert(bareJid,chat);
        return chat;
    }
    else {
        return i.value();
    }
}

bool ChatDialog::ifChatDialogExist(QString jid)
{
    QMap<QString,ChatDialog *>::const_iterator i= openedDialogs.find(jid);
    return !(i==openedDialogs.end());

}

void ChatDialog::closeChatDialog(ChatDialog *dialog)
{
    QString jid=openedDialogs.key(dialog);
    QMap<QString,ChatDialog *>::iterator i= openedDialogs.find(jid);
    openedDialogs.erase(i);
    qDebug()<<"             erase!";
}

void ChatDialog::on_photoBtn_clicked()
{
    strFilePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择上传图片"), "./", tr("Image files(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm *.jpeg);;All files (*.*)"));
    if (strFilePath.isEmpty())
    {
        return;
    }
    image=new QImage();
    image->load(strFilePath);
    image->scaled(40,30,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    ui->contentBox->moveCursor(QTextCursor::End);
    QTextCursor cursor=ui->contentBox->textCursor();
    cursor.insertImage(*image);

}

void ChatDialog::Generatelink(QNetworkReply* reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "statusCode:" << statusCode;
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray allData = reply->readAll();
        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError)
        {
            qDebug() << "json error!";
            return;
        }
        QJsonObject rootObj = jsonDoc.object();

        QString content;
        if(rootObj.contains("data"))
        {
            QJsonObject subObj = rootObj.value("data").toObject();
            picUrl=subObj["url"].toString();
            content.append("URL=");
            content.append(picUrl);
            emit newPicture(sender,receiver,QDateTime::currentDateTime(),content);
            qDebug()<<"answer:"<<picUrl;
        }
        else
        {
            picUrl=rootObj["images"].toString();
            content.append("URL=");
            content.append(picUrl);
            emit newPicture(sender,receiver,QDateTime::currentDateTime(),content);
            qDebug()<<"answer:"<<picUrl;
        }
    }
    else
    {
        qDebug() << "NetworkReply Error!";
    }

    reply->deleteLater();
}

void ChatDialog::LoadPicture(QNetworkReply *reply)
{
    qDebug() << "reply :" << reply<< endl;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data_bytes = reply->readAll();

        QImage *image_in=new QImage();
        image_in->loadFromData(data_bytes);
        image_in->scaled(40,30,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        insertInPicture(image_in,nullptr);
    }
    reply->deleteLater();

}

