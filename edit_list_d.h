#ifndef EDIT_LIST_D_H
#define EDIT_LIST_D_H

#include <QDebug>
#include <QDialog>
#include <QVector>
#include <QStringListModel>
#include "dance_t.h"
//#include "new_music_d.h"

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
signals:
    void add_dance(QString);
    void save_changes(int dance);
    void new_dance();
    void delete_dance(int);
public slots:
    void add_button();
    void delete_button();
    void check_changes();
    void load_dance(int dance);
    void new_button();
    void save_dance();
    void add_music_button();
    void delete_music_button();
private:
    Ui::edit_list_d *ui;
    QVector<dance_t*>* dance_vector;

    //QListView ui->listView_music;
    QStringListModel *music_model;
    QStringList current_music;
};

#endif // EDIT_LIST_D_H
