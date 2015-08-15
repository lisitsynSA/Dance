#ifndef DANCE_CLASS_H
#define DANCE_CLASS_H

#include <QWidget>
#include <QDebug>
#include <QDate>
#include <QVector>
#include <QTextCharFormat>
#include <QTextBrowser>
#include <QListView>
#include <add_dance_d.h>
//#include <del_dialog.h>

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
    bool get_showSpeed(){ return showSpeed; }
    bool get_showRepeating(){ return showRepeating; }
    bool get_showTime(){ return showTime; }
    QDate get_current_date(){ return current_date;}

signals:
    void modified_class();
    void modified_date();

public slots:
    void add_button();
    void del_button(){ qDebug() << "DEL"; }
    void load_button(){ qDebug() << "LOAD TO BUFFER"; }
    void set_showSpeed(bool value){  qDebug() << "SHOW SPEED"; showSpeed = value; }
    void set_showRepeating(bool value){  qDebug() << "SHOW REPEATING"; showRepeating = value; }
    void set_showTime(bool value){  qDebug() << "SHOW TIME"; showTime = value; }
    void set_music(QModelIndex index);
private slots:
    void changed_date(QDate date);
    void add_dance(QString dance);
private:
    enum { MagicNumber = 0x7FFF883};
    QCalendarWidget *calendar;
    QTextBrowser * music;
    QTextCharFormat standart;
    QTextCharFormat underline;

    QListView *listView;
    QStringListModel *model;
    QStringList current_class;

    QVector<QDate*> all_classes;
    QString class_path;
    QDate current_date;
    bool find_date(QDate date);
    void add_date(QDate date);
    void set_date_format(QVector<QDate*> dates, QTextCharFormat format);
    bool current_date_modified;

    dance_list *dancelist;
    add_dance_d *add_dance_dialog;

    bool showSpeed;
    bool showRepeating;
    bool showTime;
};

#endif // DANCE_CLASS_H
