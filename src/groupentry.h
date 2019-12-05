#ifndef GROUPENTRY_H
#define GROUPENTRY_H
#include "mainwindow.h"

class GroupEntry
{
public:
    GroupEntry(QString name,MainWindow *window);
    QTreeWidgetItem *grpAddr;
    QString grpName;
    QTreeWidgetItem *getGrpAddr() const;
    void setGrpAddr(QTreeWidgetItem *value);
    QString getGrpName() const;
    void setGrpName(const QString &value);
};

#endif // GROUPENTRY_H
