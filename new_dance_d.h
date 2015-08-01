#ifndef NEW_DANCE_D_H
#define NEW_DANCE_D_H

#include <QDialog>
#include "dance_t.h"
#include "ui_new_dance_d.h"

class new_dance_d : public QDialog, public Ui::new_dance_d
{
    Q_OBJECT

public:
    new_dance_d(QWidget *parent = 0);
signals:
    void send_dance(dance_t*);
private slots:
    void create_dance();
};

#endif // NEW_DANCE_D_H
