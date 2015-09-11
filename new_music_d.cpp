#include <QDebug>
#include <QPushButton>

#include "new_music_d.h"
#include "ui_new_music_d.h"

new_music_d::new_music_d(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_music_d)
{
    ui->setupUi(this);

    model = new QFileSystemModel;
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
    qDebug() << QDir::currentPath();
    model->setRootPath(QDir::currentPath());
    ui->treeView->setModel(model);
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
        emit add_music(ui->treeView->currentIndex().data().toString());
        accept();
    }
}

void new_music_d::create_music()
{
    emit add_music(ui->treeView->currentIndex().data().toString());
    accept();
}
