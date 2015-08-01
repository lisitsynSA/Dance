#ifndef DANCE_LIST_H
#define DANCE_LIST_H

#include <QWidget>
#include <QDebug>
//#include <edit_dialog.h>
//#include <find_dialog.h>
//#include <new_dialog.h>

class dance_t;

class dance_list : public QWidget
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
    void list_new(){ qDebug() << "NEW"; }
private:
    enum { MagicNumber = 0x7FFC883};
    QVector<dance_t*> dance_vector;
    QMap<int, dance_t*> alph_order;
    QMap<QString, dance_t*> fast_find_order;

    bool readFile();
    void make_alphabetical_order();
    void make_fast_find_order();
};

#endif // DANCE_LIST_H
