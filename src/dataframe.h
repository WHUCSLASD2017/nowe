#ifndef DATAFRAME_H
#define DATAFRAME_H

#include "ChangeHeaderWnd.h"
#include "NoweGlobal.h"
#include "nowebasewindow.h"
#include <QDialog>
#include <QXmppVCardIq.h>

namespace Ui {
class DataFrame;
}

class DataFrame : public NoweBaseWindow
{
    Q_OBJECT

public:
    explicit DataFrame(QWidget *parent = nullptr);
    ~DataFrame();

private:
    Ui::DataFrame *ui;

public slots:
    void updatePanel(const QXmppVCardIq&);

private slots:
    void on_ok_clicked();
    void sendNewVCard();
    void on_change_clicked();
};

#endif // DATAFRAME_H

