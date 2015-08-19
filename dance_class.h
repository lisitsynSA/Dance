#ifndef DANCE_CLASS_H
#define DANCE_CLASS_H

#include <QDebug>
#include <QDate>
#include <QVector>
#include <QTextCharFormat>
#include "listview_t.h"
#include "add_dance_d.h"

class QWidget;
class QTextBrowser;
class QCalendarWidget;
class QStringListModel;
class dance_list;

class dance_class : public QWidget
{
    Q_OBJECT

public:
    explicit dance_class(dance_list *init_dancelist, QWidget *parent = 0);

    void clear();
    bool read_mainFile(const QString &fileName);
    bool write_mainFile(const QString &fileName);
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    bool removeFile(const QString &fileName);
    bool get_showSpeed(){ return showSpeed; }
    bool get_showRepeating(){ return showRepeating; }
    bool get_showTime(){ return showTime; }
    QDate get_current_date(){ return current_date;}
    bool save_current_date();

signals:
    void modified_class();
    void modified_date();
    void status_bar(QString);

public slots:
    void add_button();
    void del_button();
    void load_button();
    void set_showSpeed(bool value){  qDebug() << "SHOW SPEED"; showSpeed = value; }
    void set_showRepeating(bool value){  qDebug() << "SHOW REPEATING"; showRepeating = value; }
    void set_showTime(bool value){  qDebug() << "SHOW TIME"; showTime = value; }
    void set_music(QModelIndex index);
private slots:
    void changed_date(QDate date);
    void add_dance(QString dance);
    void drop_event(int row, QList<int>* list);
private:
    enum { MagicNumber = 0x7FFF883};
    QCalendarWidget *calendar;
    QTextBrowser * music;
    QTextCharFormat standart;
    QTextCharFormat underline;

    listview_t *listView;
    QStringListModel *model;
    QStringList current_class;

    QVector<QDate*> all_classes;
    QString class_path;
    QDate current_date;
    bool find_date(QDate date);
    void add_date(QDate date);
    void delete_date(QDate date);
    void set_date_format(QVector<QDate*> dates, QTextCharFormat format);
    bool current_date_modified;

    dance_list *dancelist;
    add_dance_d *add_dance_dialog;

    bool showSpeed;
    bool showRepeating;
    bool showTime;
};

#endif // DANCE_CLASS_H
