#include "add_dance_d.h"
#include "ui_add_dance_d.h"

add_dance_d::add_dance_d(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_dance_d)
{
    ui->setupUi(this);
}

add_dance_d::~add_dance_d()
{
    delete ui;
}
