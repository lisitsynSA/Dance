#include "edit_list_d.h"
#include "ui_edit_list_d.h"

edit_list_d::edit_list_d(dance_list *init_dancelist, QWidget* parent):
    QDialog(parent),
    ui(new Ui::edit_list_d),
    dancelist(init_dancelist)
{
    ui->setupUi(this);
}

edit_list_d::~edit_list_d()
{
    delete ui;
}
