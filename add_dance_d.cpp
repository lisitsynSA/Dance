#include "add_dance_d.h"
#include "ui_add_dance_d.h"

add_dance_d::add_dance_d(QVector<dance_t *> *init_dance_vector, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_dance_d),
    dance_vector(init_dance_vector)
{
    ui->setupUi(this);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(load_dance(int)));
    connect(this, SIGNAL(accepted()),
            this, SLOT(save_dance()));
    update_dancelist();
    load_dance(0);
}

add_dance_d::~add_dance_d()
{
    delete ui;
}

void add_dance_d::update_dancelist()
{
    ui->comboBox->clear();
    QVector<dance_t*>::const_iterator i = dance_vector->constBegin();
    while (i != dance_vector->constEnd())
    {
        ui->comboBox->addItem((*i)->get_name());
        ++i;
    }
}

void add_dance_d::load_dance(int dance)
{
    if ((0 <= dance) && (dance < dance_vector->size()))
    {
        qDebug() << "LOAD DANCE FOR ADDING: " << dance;
        ui->textBrowser->setText((*dance_vector)[dance]->get_short_description());
    }
}

void add_dance_d::save_dance()
{
    emit add_dance((*dance_vector)[ui->comboBox->currentIndex()]->get_name());
}
