#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H
#include <set>
#include <QMap>
#include <QTreeWidgetItem>
#include <QMainWindow>

class GroupManager
{
public:
    GroupManager();
    QMap<QString,QTreeWidgetItem *> groups;
    QTreeWidgetItem *getGrpAddr(QString grpName,QMainWindow *window);

};

#endif // GROUPMANAGER_H
