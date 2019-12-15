#ifndef GROUP
#define GROUP

#include <QObject>
#include <QXmppClient.h>
#include <QXmppMucManager.h>

#include "NoweGlobal.h"

class Group : QObject
{
    Q_OBJECT
public:
    Group(QString jid);

    bool obtainMemberList(QString affiliation);
    bool obtainInactiveMemberList();

    bool inviteNewMember(QString jid);
    void registerWithRoom();

    inline QXmppMucRoom * getRoom();
    inline QString getRoomJid();
    inline QStringList & getMembers();
    inline QStringList & getActiveMembers();
    inline QStringList & getInactiveMembers();
    inline void setIsRefreshList(bool isRefreshList);

private:
    QXmppMucRoom * room;
    QString roomJid;

    QString affiliation;

    QStringList members;
    QStringList activeMembers;
    QStringList inactiveMembers;

    bool isRefreshList;

    bool receiveMemberList(QXmppElement queryEl);
    bool receiveRegistrationForm();
    bool updateMembership(QXmppElement xEl);

public slots:
    bool on_iqReceived(const QXmppIq &iq);
    void addMember(QString jid);
    void removeMember(QString jid);

signals:
    void renderMemberList();
    void modifyMemberStatus(QString jid);
};

QXmppMucRoom * Group::getRoom()
{
    return room;
}

QString Group::getRoomJid()
{
    return roomJid;
}

QStringList & Group::getMembers()
{
    return members;
}

QStringList & Group::getActiveMembers()
{
    return activeMembers;
}

QStringList & Group::getInactiveMembers()
{
    return inactiveMembers;
}

void Group::setIsRefreshList(bool isRefreshList)
{
    this->isRefreshList = isRefreshList;
}

#endif // GROUP
