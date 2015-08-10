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
    explicit edit_list_d(QVector<dance_t*>* init_dance_vector,
                         QWidget *parent = 0);
    ~edit_list_d();

    void update_dancelist();
    bool get_modified() const { return modified;}
    bool set_modified(bool init_modified) { return modified = init_modified;}
signals:
    void save_changes();
public slots:
    void load_dance(int dance);
    void save_dance();
    void list_changed(){qDebug() << "####"; modified = true;}//TODO: split useful and useless calls of list_changed()
private:
    bool modified;
    Ui::edit_list_d *ui;
    QVector<dance_t*>* dance_vector;

};

#endif // EDIT_LIST_D_H
