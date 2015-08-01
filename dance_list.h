#ifndef DANCE_LIST_H
#define DANCE_LIST_H

#include <QWidget>
#include <QDebug>

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

    bool readFile();
    void make_alphabetical_order();
};

#endif // DANCE_LIST_H
