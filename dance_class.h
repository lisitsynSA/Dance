#ifndef DANCE_CLASS_H
#define DANCE_CLASS_H

#include <QWidget>
#include <QDebug>
#include <QDate>
#include <QTextCharFormat>
//#include <add_dialog.h>
//#include <del_dialog.h>

class QCalendarWidget;
class QListView;
class QStringListModel;
class dance_list;


class dance_class : public QWidget
{
    Q_OBJECT

public:
    explicit dance_class(dance_list *init_dancelist, QWidget *parent = 0);

    void clear(){}//delete: all_classes, current_class, current_date
    bool read_mainFile(const QString &fileName);
    bool write_mainFile(const QString &fileName);
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    bool get_showSpeed(){ return showSpeed; }
    bool get_showRepeating(){ return showRepeating; }
    bool get_showTime(){ return showTime; }
    QDate get_current_date(){ return current_date;}

signals:
    void modified_class();
    void modified_date();

public slots:
    void add_in_lesson(){ qDebug() << "ADD"; }
    void del_in_lesson(){ qDebug() << "DEL"; }
    void load_to_buffer(){ qDebug() << "LOAD"; }
    void set_showSpeed(bool value){  qDebug() << "SHOW SPEED"; showSpeed = value; }
    void set_showRepeating(bool value){  qDebug() << "SHOW REPEATING"; showRepeating = value; }
    void set_showTime(bool value){  qDebug() << "SHOW TIME"; showTime = value; }

private slots:
    void changed_date(QDate date);

private:
    enum { MagicNumber = 0x7FFF883};
    QCalendarWidget *calendar;
    QTextCharFormat standart;
    QTextCharFormat underline;
    QListView *listView;
    QStringListModel *model;
    QList<QDate*> all_classes;
    QStringList current_class;
    QDate current_date;

    dance_list *dancelist;

    bool showSpeed;
    bool showRepeating;
    bool showTime;
};

#endif // DANCE_CLASS_H
