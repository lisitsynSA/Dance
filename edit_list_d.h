#ifndef EDIT_LIST_D_H
#define EDIT_LIST_D_H

#include <QDialog>
#include "dance_t.h"

namespace Ui {
class edit_list_d;
}

class edit_list_d : public QDialog
{
    Q_OBJECT

public:
    explicit edit_list_d(QWidget *parent = 0);
    ~edit_list_d();

private:
    Ui::edit_list_d *ui;
};

#endif // EDIT_LIST_D_H
