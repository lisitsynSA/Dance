#include <QtGui>

#include "new_dance_d.h"
#include "ui_new_dance_d.h"

new_dance_d::new_dance_d(QVector<dance_t*>* init_dance_vector, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_dance_d),
    dance_vector(init_dance_vector),
    new_music(0)
{
    ui->setupUi(this);
    music_model = new QStringListModel(this);
    ui->listView_music->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView_music->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView_music->setDragDropMode(QAbstractItemView::NoDragDrop);
    ui->listView_music->setModel(music_model);
    music_model->setStringList(current_music);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->add_musicButton, SIGNAL(clicked()),
            this, SLOT(add_music_button()));
    connect(ui->delete_musicButton, SIGNAL(clicked()),
            this, SLOT(delete_music_button()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(create_dance()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

new_dance_d::~new_dance_d()
{
    delete ui;
}

void new_dance_d::create_dance()
{
    if (!find_dance(ui->lineEdit->text()))
    {
        dance_t* result = new dance_t(ui->lineEdit->text(),
                                      ui->textEdit_short->toPlainText(),
                                      ui->textEdit_full->toPlainText(),
                                      current_music);
        emit send_dance(result);
        emit accept();
        ui->lineEdit->setText("");
        ui->textEdit_short->setText("");
        ui->textEdit_full->setText("");
        current_music.clear();
        music_model->setStringList(current_music);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        QMessageBox::warning(this, tr("Creating of the dance"),
                             tr("Dance '%1' has already exist.")
                             .arg(ui->lineEdit->text()));
    }
}

bool new_dance_d::find_dance(QString name)//TODO:optimize search
{
    for (int i = 0; i < dance_vector->count(); i++)
        if (name == (*dance_vector)[i]->get_name())
            return true;
    return false;
}

void new_dance_d::on_lineEdit_textChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                !(ui->lineEdit->text() == ""));
}

void new_dance_d::add_music_button()
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

void new_dance_d::add_music(QString music)
{
    qDebug() << "ADD MUSIC:" << music;
    if (!current_music.contains(music))
    {
        current_music.push_back(music);
        current_music.sort();
        music_model->setStringList(current_music);
    }
}

void new_dance_d::delete_music_button()
{
    qDebug() << "DELETE MUSIC";
    if (current_music.isEmpty())
        return;
    current_music.removeOne(ui->listView_music->currentIndex().data().toString());
    music_model->setStringList(current_music);
}
