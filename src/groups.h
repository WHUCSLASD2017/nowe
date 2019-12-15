#ifndef GROUPS_H
#define GROUPS_H

#include <QObject>

#include "group.h"

class Groups : QObject
{
    Q_OBJECT
public:
    static Group & getMyGroups();
private:
    Groups();
    QMap<QString, Group *> groups;
};

#endif // GROUPS_H
