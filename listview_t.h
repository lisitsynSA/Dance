#ifndef LISTVIEW_T_H
#define LISTVIEW_T_H

#include <QListView>
#include <QDropEvent>
#include <QDebug>

class listview_t : public QListView
{
    Q_OBJECT
public:
    listview_t(QWidget *parent = 0): QListView(parent){}
    QList<int>* get_selectedIndexes();//descending
    int get_row(QPoint pos);
protected:
    void dropEvent(QDropEvent * event);
signals:
    void drop_signal(int row, QList<int>* list);
};

#endif // LISTVIEW_T_H
