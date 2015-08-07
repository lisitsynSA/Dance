#ifndef EDIT_LIST_D_H
#define EDIT_LIST_D_H

#include <QDialog>
#include "dance_t.h"
#include <QDebug>
class dance_list;

namespace Ui {
class edit_list_d;
}

class edit_list_d : public QDialog
{
    Q_OBJECT

public:
    explicit edit_list_d(dance_list* init_dancelist, QWidget *parent = 0);
    ~edit_list_d();

    void update_dancelist(){};
signals:

public slots:
    void load_dance(QString dance){qDebug() << "LOAD DANCE FOR EDITTING: " << dance;}
private:
    Ui::edit_list_d *ui;
    dance_list *dancelist;

};

#endif // EDIT_LIST_D_H
