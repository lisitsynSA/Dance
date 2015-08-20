#include "find_d.h"
#include "ui_find_d.h"

find_d::find_d(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::find_d)
{
    ui->setupUi(this);
}

find_d::~find_d()
{
    delete ui;
}
