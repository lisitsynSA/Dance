#ifndef NEW_DANCE_D_H
#define NEW_DANCE_D_H

#include <QDialog>
#include <QStringListModel>
#include "dance_t.h"
#include "new_music_d.h"

namespace Ui {
class new_dance_d;
}

class new_dance_d : public QDialog
{
    Q_OBJECT

public:
    new_dance_d(QVector<dance_t*>* init_dance_vector, QWidget *parent = 0);
    ~new_dance_d();

signals:
    void send_dance(dance_t*);
private slots:
    void create_dance();
    void on_lineEdit_textChanged();
    void add_music_button();
    void add_music(QString music);
    void delete_music_button();
private:
    bool find_dance(QString name);
    Ui::new_dance_d *ui;
    QVector<dance_t*>* dance_vector;

    //QListView ui->listView_music;
    QStringListModel *music_model;
    QStringList current_music;
    new_music_d *new_music;
};

#endif // NEW_DANCE_D_H
