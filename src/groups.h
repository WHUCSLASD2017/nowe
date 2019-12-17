#ifndef GROUPS_H
#define GROUPS_H

#include "group.h"

class Groups
{
public:
    ~Groups();
    static Groups & getMyGroups();
    bool addGroup(QString roomJid);
    bool removeGroup(QString roomJid);
    Group * getGroup(QString roomJid);

private:
    Groups();
    QMap<QString, Group *> groups;
};

#endif // GROUPS_H
