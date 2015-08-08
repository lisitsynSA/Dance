#include <QtGui>

#include "new_dance_d.h"
#include "ui_new_dance_d.h"

new_dance_d::new_dance_d(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_dance_d)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(create_dance()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

new_dance_d::~new_dance_d()
{
    delete ui;
}

void new_dance_d::create_dance()
{
    dance_t* result = new dance_t(ui->lineEdit->text(),
                                  ui->textEdit_short->toPlainText(),
                                  ui->textEdit_full->toPlainText(),
                                  ui->textEdit_music->toPlainText());
    emit send_dance(result);
    emit accept();
    ui->lineEdit->setText("");
    ui->textEdit_short->setText("");
    ui->textEdit_full->setText("");
    ui->textEdit_music->setText("");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void new_dance_d::on_lineEdit_textChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                !(ui->lineEdit->text() == ""));
}
