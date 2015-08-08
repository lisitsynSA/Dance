#include "edit_list_d.h"
#include "ui_edit_list_d.h"

edit_list_d::edit_list_d(QVector<dance_t*>* init_dance_vector, QWidget* parent):
    QDialog(parent),
    ui(new Ui::edit_list_d),
    dance_vector(init_dance_vector)
{
    ui->setupUi(this);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(load_dance(int)));
    ui->comboBox->setDuplicatesEnabled(false);
    update_dancelist();
    load_dance(0);
}

edit_list_d::~edit_list_d()
{
    delete ui;
}

void edit_list_d::update_dancelist()
{
    ui->comboBox->clear();
    QVector<dance_t*>::const_iterator i = dance_vector->constBegin();
    while (i != dance_vector->constEnd()) {
        ui->comboBox->addItem((*i)->get_name());
        ++i;
    }
}
