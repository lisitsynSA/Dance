#include <QDebug>
#include <QPushButton>
#include <QMessageBox>

#include "new_music_d.h"
#include "ui_new_music_d.h"

new_music_d::new_music_d(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_music_d)
{
    ui->setupUi(this);

    //CHECK DIR
    QDir dir = QDir::current();
    if (!dir.exists("music"))
        if(!dir.mkdir("music"))
            QMessageBox::warning(this, tr("Dance class"),
                                 tr("Cannot create directory 'music'."));

    model = new QFileSystemModel;
    model->setRootPath(dir.absoluteFilePath("music"));
    update_tree();

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(check_choice(QModelIndex)));
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(add_choice(QModelIndex)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(create_music()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

new_music_d::~new_music_d()
{
    delete ui;
}
void new_music_d::update_tree()
{
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(QDir::current().absoluteFilePath("music")));
}

void new_music_d::check_choice(QModelIndex choice)
{
    qDebug() << choice.data();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                    choice.data().toString().contains(".mp3"));
}

void new_music_d::add_choice(QModelIndex choice)
{
    if (choice.data().toString().contains(".mp3"))
    {
        emit add_music(tr(ui->treeView->currentIndex().data().toString().toLocal8Bit().data()));
        accept();
    }
}

void new_music_d::create_music()
{
    emit add_music(tr(ui->treeView->currentIndex().data().toString().toLocal8Bit().data()));
    accept();
}
