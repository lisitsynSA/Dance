#include <QtGui>

#include "new_dance_d.h"

new_dance_d::new_dance_d(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(create_dance()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void new_dance_d::create_dance()
{
    dance_t* result = new dance_t();
    emit send_dance(result);
    emit accept();
}

void new_dance_d::on_lineEdit_textChanged()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            lineEdit->hasAcceptableInput());
}
