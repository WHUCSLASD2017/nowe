#include "groupentry.h"


GroupEntry::GroupEntry(QString name, MainWindow *window)
{
    grpName=name;
    grpAddr=window->createFriendGroup(grpName);
}

QString GroupEntry::getGrpName() const
{
    return grpName;
}

void GroupEntry::setGrpName(const QString &value)
{
    grpName = value;
}

QTreeWidgetItem *GroupEntry::getGrpAddr() const
{
    return grpAddr;
}

void GroupEntry::setGrpAddr(QTreeWidgetItem *value)
{
    grpAddr = value;
}
