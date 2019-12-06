#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H
#include <QMap>
#include <QTreeWidgetItem>
#include <QMainWindow>

class GroupManager
{
public:
    GroupManager();
    QMap<QString,QTreeWidgetItem *> groups;
    QTreeWidgetItem *getGrpAddr(QString grpName,QWidget *window);

    void flush();
};

#endif // GROUPMANAGER_H
