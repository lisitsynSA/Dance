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
    connect(ui->textEdit_full, SIGNAL(textChanged()),
            this, SLOT(check_changes()));
    connect(ui->textEdit_short, SIGNAL(textChanged()),
            this, SLOT(check_changes()));
    connect(ui->textEdit_music, SIGNAL(textChanged()),
            this, SLOT(check_changes()));
    connect(ui->saveButton, SIGNAL(clicked()),
            this, SLOT(save_dance()));
    connect(ui->addButton, SIGNAL(clicked()),
            this, SLOT(add_dance_slot()));
    connect(ui->newButton, SIGNAL(clicked()),
            this, SLOT(new_button()));
    update_dancelist();
    ui->saveButton->setEnabled(false);
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
    while (i != dance_vector->constEnd())
    {
        ui->comboBox->addItem((*i)->get_name());
        ++i;
    }
}

void edit_list_d::add_dance_slot()
{
    if (dance_vector->size())
    {
        accept();
        emit add_dance((*dance_vector)[ui->comboBox->currentIndex()]->get_name());
    }
}

void edit_list_d::check_changes()
{
    int dance = ui->comboBox->currentIndex();
    if ((*dance_vector)[dance]->get_short_description() == ui->textEdit_short->toPlainText() &&
        (*dance_vector)[dance]->get_description() == ui->textEdit_full->toPlainText() &&
        (*dance_vector)[dance]->get_music() == ui->textEdit_music->toPlainText())
        ui->saveButton->setEnabled(false);
    else
        ui->saveButton->setEnabled(true);
}

void edit_list_d::load_dance(int dance)
{
    if ((0 <= dance) && (dance < dance_vector->size()))
    {
        qDebug() << "LOAD DANCE FOR EDITTING: " << dance;
        ui->textEdit_short->setText((*dance_vector)[dance]->get_short_description());
        ui->textEdit_full->setText((*dance_vector)[dance]->get_description());
        ui->textEdit_music->setText((*dance_vector)[dance]->get_music());
    }
}

void edit_list_d::new_button()
{
    accept();
    emit new_dance();
}

void edit_list_d::save_dance()
{
    int dance = ui->comboBox->currentIndex();
    (*dance_vector)[dance]->set_music(ui->textEdit_music->toPlainText());
    (*dance_vector)[dance]->set_description(ui->textEdit_full->toPlainText());
    (*dance_vector)[dance]->set_short_description(ui->textEdit_short->toPlainText());
    ui->saveButton->setEnabled(false);
    emit save_changes();
}
