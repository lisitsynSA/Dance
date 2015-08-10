#ifndef ADD_DANCE_D_H
#define ADD_DANCE_D_H

#include <QDialog>
#include <QDebug>
#include "dance_t.h"

namespace Ui {
class add_dance_d;
}

class add_dance_d : public QDialog
{
    Q_OBJECT
    
public:
    explicit add_dance_d(QVector<dance_t*>* init_dance_vector,
                         QWidget *parent = 0);
    ~add_dance_d();

    void update_dancelist();
signals:
    void add_dance(QString);
public slots:
    void load_dance(int dance);
    void save_dance();
private:
    Ui::add_dance_d *ui;
    QVector<dance_t*>* dance_vector;
};

#endif // ADD_DANCE_D_H
