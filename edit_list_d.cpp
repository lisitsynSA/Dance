#include "edit_list_d.h"
#include "ui_edit_list_d.h"

edit_list_d::edit_list_d(QVector<dance_t*>* init_dance_vector, QWidget* parent):
    QDialog(parent),
    ui(new Ui::edit_list_d),
    dance_vector(init_dance_vector),
    new_music(0)
{
    ui->setupUi(this);
    music_model = new QStringListModel(this);
    ui->listView_music->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView_music->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView_music->setDragDropMode(QAbstractItemView::NoDragDrop);
    ui->listView_music->setModel(music_model);

    ui->addButton->setDefault(true);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(load_dance(int)));
    connect(ui->textEdit_full, SIGNAL(textChanged()),
            this, SLOT(check_changes()));
    connect(ui->textEdit_short, SIGNAL(textChanged()),
            this, SLOT(check_changes()));
    connect(ui->add_musicButton, SIGNAL(clicked()),
            this, SLOT(add_music_button()));
    connect(ui->delete_musicButton, SIGNAL(clicked()),
            this, SLOT(delete_music_button()));
    connect(ui->saveButton, SIGNAL(clicked()),
            this, SLOT(save_dance()));
    connect(ui->addButton, SIGNAL(clicked()),
            this, SLOT(add_button()));
    connect(ui->newButton, SIGNAL(clicked()),
            this, SLOT(new_button()));
    connect(ui->deleteButton, SIGNAL(clicked()),
            this, SLOT(delete_button()));
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

void edit_list_d::add_button()
{
    if (dance_vector->size())
    {
        accept();
        emit add_dance((*dance_vector)[ui->comboBox->currentIndex()]->get_name());
    }
}

void edit_list_d::delete_button()
{
    if (dance_vector->size())
    {
        //accept();
        emit delete_dance(ui->comboBox->currentIndex());
    }
}

void edit_list_d::check_changes()
{
    int dance = ui->comboBox->currentIndex();
    if ((*dance_vector)[dance]->get_short_description() == ui->textEdit_short->toPlainText() &&
        (*dance_vector)[dance]->get_description() == ui->textEdit_full->toPlainText() &&
        (*dance_vector)[dance]->get_music() == current_music)
        ui->saveButton->setEnabled(false);
    else
        ui->saveButton->setEnabled(true);
}

void edit_list_d::load_dance(int dance)
{
    if ((0 <= dance) && (dance < dance_vector->size()))
    {
        qDebug() << "LOAD DANCE FOR EDITTING: " << dance;
        ui->comboBox->setCurrentIndex(dance);
        ui->textEdit_short->setText((*dance_vector)[dance]->get_short_description());
        ui->textEdit_full->setText((*dance_vector)[dance]->get_description());
        current_music.clear();
        current_music =(*dance_vector)[dance]->get_music();
        music_model->setStringList(current_music);
        check_changes();
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
    (*dance_vector)[dance]->set_music(current_music);
    (*dance_vector)[dance]->set_description(ui->textEdit_full->toPlainText());
    (*dance_vector)[dance]->set_short_description(ui->textEdit_short->toPlainText());
    ui->saveButton->setEnabled(false);
    emit save_changes(dance);
}

void edit_list_d::add_music_button()
{
    qDebug() << "NEW MUSIC";
    if (!new_music)
    {
     new_music = new new_music_d(this);
     connect(new_music, SIGNAL(add_music(QString)),
             this, SLOT(add_music(QString)));
    }
    new_music->update_tree();
    new_music->show();
    new_music->raise();
    new_music->activateWindow();
}

void edit_list_d::add_music(QString music)
{
    qDebug() << "ADD MUSIC:" << music;
    if (!current_music.contains(music))
    {
        current_music.push_back(music);
        current_music.sort();
        music_model->setStringList(current_music);
        check_changes();
    }
}

void edit_list_d::delete_music_button()
{
    qDebug() << "DELETE MUSIC";
    if (current_music.isEmpty())
        return;
    current_music.removeOne(ui->listView_music->currentIndex().data().toString());
    music_model->setStringList(current_music);
    check_changes();
}
