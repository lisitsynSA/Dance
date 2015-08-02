#ifndef DANCE_LIST_H
#define DANCE_LIST_H

#include <QWidget>
#include <QDebug>
#include "new_dance_d.h"
//#include <edit_dialog.h>
//#include <find_dialog.h>

class dance_t;

class dance_list : public QWidget
// dance_list = dictionary of dances
{
    Q_OBJECT
public:
    explicit dance_list(QWidget *parent = 0);
    ~dance_list();

    bool writeFile();
    QString get_name_dance(int place) const;
    dance_t* get_dance(int place) const;
    dance_t* get_dance(QString name) const;

signals:
    void modified_list();

public slots:
    void list_edit(){ qDebug() << "EDIT"; }
    void list_find(){ qDebug() << "FIND"; }
    void list_new();
    void list_add(dance_t * dance) {add_dance(dance);
                                    qDebug() << "New dance";}
private:
    enum { MagicNumber = 0x7FFC883};
    QVector<dance_t*> dance_vector;
    QMap<int, dance_t*> alph_order;
    QMap<QString, dance_t*> fast_find_order;

    new_dance_d* new_dance_dialog;
    void add_dance(dance_t* dance);
    bool readFile();
    void make_alphabetical_order();
    void make_fast_find_order();
};

#endif // DANCE_LIST_H
