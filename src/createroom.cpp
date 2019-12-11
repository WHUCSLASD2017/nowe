#include "createroom.h"
#include "ui_createroom.h"
#include "NoweGlobal.h"

CreateRoom::CreateRoom(QWidget *parent) :
    NoweBaseWindow(parent),
    ui(new Ui::CreateRoom)
{
    ui->setupUi(this);
    NoweBaseWindow::initNoweStyle();
    this->client=Nowe::myClient();

}

CreateRoom::~CreateRoom()
{
    delete ui;
}

void CreateRoom::on_cancelBtn_clicked()
{
    close();
}

void CreateRoom::on_createBtn_clicked()
{
    QString  roomName = ui->nameLnEdt->text();
    if(roomName == ""){
        QMessageBox::critical(this,"输入非法","请输入群名称！");
    }
    else {
        createRoom(roomName);
    }
}


//创建聊天室
void CreateRoom::createRoom(QString roomName)
{
    QXmppMucManager* roomMsg = client->findExtension<QXmppMucManager>();

    //服务器名
    QString serverName = "chirsz.cc";
    //聊天室JID
    QString jid=roomName+"@conference."+serverName;

    QList<QXmppMucRoom*> rooms = roomMsg->rooms();

    //查询聊天室JID是否冲突
    foreach(QXmppMucRoom* r, rooms)
    {
        qDebug()<<r->jid();
        if(r->jid() == jid)
        {
            QMessageBox::critical(this,"创建失败","该群聊已存在！");
            return ;
        }
    }

    //添加群组
    QXmppMucRoom*  m_pRoom = roomMsg->addRoom(jid);

    if(m_pRoom)
    {
        //群名片：必须设置，否则将创建失败
        m_pRoom->setNickName(Nowe::myJidBare());

        //进入群:此命令必须在设置房间属性之前，只有加入房间后才能设置属性
        m_pRoom->join();

        //设置房间属性，使群成为长久群
        QXmppDataForm form(QXmppDataForm::Submit);
        QList<QXmppDataForm::Field> fields;
        {
           QXmppDataForm::Field field(QXmppDataForm::Field::HiddenField);
           field.setKey("FORM_TYPE");
           field.setValue("http://jabber.org/protocol/muc#roomconfig");
           fields.append(field);
        }
        QXmppDataForm::Field field;
        field.setKey("muc#roomconfig_roomname");
        field.setValue(roomName);
        fields.append(field);

        //field.setKey("muc#roomconfig_subject");
        //field.setValue(roomSubject.text());
        //fields.append(field);

        //field.setKey("muc#roomconfig_roomdesc");
        //field.setValue(roomDesc.text());
        fields.append(field);
        {
           QXmppDataForm::Field field(QXmppDataForm::Field::BooleanField);
           field.setKey("muc#roomconfig_persistentroom");
           field.setValue(true);
           fields.append(field);
        }

        form.setFields(fields);
        //The dataform ends here.
        m_pRoom->setConfiguration(form);

        Nowe::createBookMark(roomName);
    }


    close();

}


