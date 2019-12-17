#include "groups.h"

Groups::Groups()
{

}

Groups::~Groups()
{
    QMap<QString, Group *>::iterator it;
    for (it = groups.begin(); it != groups.end(); it++) {
        delete it.value();
        it.value() = nullptr;
    }

    groups.clear();
}

Groups & Groups::getMyGroups()
{
    static Groups myGroups;
    return myGroups;
}

bool Groups::addGroup(QString roomJid)
{
    if (groups.contains(roomJid)) {
        qDebug() << "depulicated roomJid!" << endl;
        return false;
    }

    Group * group = new Group(roomJid);
    groups[roomJid] = group;
    return true;
}

bool Groups::removeGroup(QString roomJid)
{
    if (!groups.contains(roomJid)) {
        qDebug() << "invalid roomJid!" << endl;
        return false;
    }

    groups.remove(roomJid);
}

Group * Groups::getGroup(QString roomJid)
{
    return groups[roomJid];
}
