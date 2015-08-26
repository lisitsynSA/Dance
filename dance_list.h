#ifndef DANCE_LIST_H
#define DANCE_LIST_H

#include <QDebug>
#include "new_dance_d.h"
#include "edit_list_d.h"

class QWidget;
class dance_t;

class dance_list : public QWidget
// dance_list = dictionary of dances
{
    Q_OBJECT
public:
    explicit dance_list(QWidget *parent = 0);
    ~dance_list();

    bool writeFile();
    QVector<dance_t*>* get_dance_vector() {return &dance_vector;}
    QString get_name_dance(int place) const;
    dance_t* get_dance(int place) const;
    dance_t* get_dance(QString name) const;
    int get_dance_number(QString name) const;
    int get_dance_number(dance_t* dance) const;
    int get_size() const{ return dance_vector.size();}
    bool find_name(QString name);
    bool is_modified() const{ return modified;}

signals:
    void add_dance_to_lesson(QString);

public slots:
    void list_edit_button();
    void list_new_button();
    void list_add(dance_t * dance) {add_dance(dance);
                                    qDebug() << "NEW DANCE";}
    void list_delete(int dance);
    void modified_list_slot();
    void open_dance(QString dance);
private:
    enum { MagicNumber = 0x7FFC883};
    QVector<dance_t*> dance_vector;
    QMap<QString, dance_t*> fast_find_order;
    QMap<QString, int> fast_number_order;
    bool modified;

    new_dance_d* new_dance_dialog;
    edit_list_d* edit_list_dialog;
    void add_dance(dance_t* dance);
    void delete_dance();
    bool readFile();
    void make_alphabetical_order();
    void make_fast_order();
    void vector_qsort(int first, int last);
    QString dump();
};

#endif // DANCE_LIST_H
