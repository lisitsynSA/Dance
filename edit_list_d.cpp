#include "edit_list_d.h"
#include "ui_edit_list_d.h"

edit_list_d::edit_list_d(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_list_d)
{
    ui->setupUi(this);
}

edit_list_d::~edit_list_d()
{
    delete ui;
}
