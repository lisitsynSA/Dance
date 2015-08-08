#ifndef EDIT_LIST_D_H
#define EDIT_LIST_D_H

#include <QDialog>
#include <QDebug>
#include <QVector>
#include "dance_t.h"

namespace Ui {
class edit_list_d;
}

class edit_list_d : public QDialog
{
    Q_OBJECT

public:
    explicit edit_list_d(QVector<dance_t*>* init_dance_vector, QWidget *parent = 0);
    ~edit_list_d();

    void update_dancelist();
signals:

public slots:
    void load_dance(int dance){if ((0 <= dance) && (dance < dance_vector->size()))
            qDebug() << "LOAD DANCE FOR EDITTING: " << dance;}
private:
    Ui::edit_list_d *ui;
    QVector<dance_t*>* dance_vector;

};

#endif // EDIT_LIST_D_H
