#include "groupmanager.h"
#include "mainwindow.h"

GroupManager::GroupManager()
{

}

QTreeWidgetItem *GroupManager::getGrpAddr(QString grpName,QWidget *window)
{
    QMap<QString,QTreeWidgetItem *>::const_iterator i =groups.find(grpName);
    if(i==groups.end())
    {
        QTreeWidgetItem *res=((MainWindow *)window)->createFriendGroup(grpName);
        groups.insert(grpName,res);
        return res;
    }
    else return i.value();
}

void GroupManager::flush()
{
    groups.clear();
}
