#ifndef NEW_MUSIC_D_H
#define NEW_MUSIC_D_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class new_music_d;
}

class new_music_d : public QDialog
{
    Q_OBJECT
    
public:
    explicit new_music_d(QWidget *parent = 0);
    ~new_music_d();
    void update_tree();
private slots:
    void check_choice(QModelIndex choice);
    void add_choice(QModelIndex choice);
    void create_music();
signals:
    void add_music(QString music);

private:
    Ui::new_music_d *ui;
    QFileSystemModel *model;
};

#endif // NEW_MUSIC_D_H
