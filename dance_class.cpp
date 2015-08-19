#include <QtGui>
#include "dance_class.h"
#include "dance_list.h"

dance_class::dance_class(dance_list *init_dancelist, QWidget *parent) :
    QWidget(parent)
{
    showSpeed = true;
    showRepeating = true;
    showTime = true;

    dancelist = init_dancelist;
    add_dance_dialog = 0;
    current_date_modified = false;

    model = new QStringListModel(this);

    listView = new listview_t(this);
    listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setDragDropMode(QAbstractItemView::InternalMove);
    listView->setModel(model);
    connect(listView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(set_music(QModelIndex)));
    connect(listView, SIGNAL(drop_signal(int,QList<int>*)),
            this, SLOT(drop_event(int,QList<int>*)));

    calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true);
    calendar->setFirstDayOfWeek(Qt::Monday);
    connect(calendar, SIGNAL(clicked(QDate)),
            this, SLOT(changed_date(QDate)));
    current_date = calendar->selectedDate();
    standart = calendar->dateTextFormat(current_date);
    underline = standart;
    underline.setFontPointSize(15);
    underline.setForeground(Qt::magenta);

    music  = new QTextBrowser(this);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(calendar);
    rightLayout->addWidget(new QLabel("Music:", this));
    rightLayout->addWidget(music);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(listView);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
}

void dance_class::add_button()
{
    qDebug() << "ADD";
    if (!add_dance_dialog)
    {
        add_dance_dialog = new add_dance_d(dancelist->get_dance_vector(), this);
        connect(add_dance_dialog, SIGNAL(add_dance(QString)),
                this, SLOT(add_dance(QString)));
        connect(add_dance_dialog, SIGNAL(accepted()),
                this, SIGNAL(modified_class()));
        connect(dancelist, SIGNAL(modified_list()),
                add_dance_dialog, SLOT(update_dancelist()));
    }

    add_dance_dialog->show();
    add_dance_dialog->raise();
    add_dance_dialog->activateWindow();
}

void dance_class::add_dance(QString dance)
{
    current_class.push_back(dance);
    current_date_modified = true;
    model->setStringList(current_class);
}

void dance_class::del_button()
{//TODO: deleting many dances
    QList<int>* list = listView->get_selectedIndexes();
    qDebug() << "DEL: " << *list;
    QList<int>::const_iterator it;
    for (it = list->constBegin(); it != list->constEnd(); it++)
        current_class.removeAt(*it);
    list->clear();
    delete list;
    current_date_modified = true;
    model->setStringList(current_class);
}

void dance_class::load_button()
{
    qDebug() << "LOAD TO BUFFER";
    QClipboard *clipboard = QApplication::clipboard();
    QString buffer = current_date.toString("dd.MM.yy (dddd)") + ":\n";
    QStringList::const_iterator it;
    for (it = current_class.constBegin(); it != current_class.constEnd(); it++)
        buffer = buffer + "\n" + (*it).toLocal8Bit().constData();
    clipboard->setText(buffer);
    emit status_bar(tr("Loaded to the clipboard"));
}

void dance_class::drop_event(int row, QList<int>* list)
{
    qDebug() << "DROP EVENT: " << row << *list;
    QStringList temp;
    QList<int>::const_iterator it;
    for (it = list->constBegin(); it != list->constEnd(); it++)
    {
        temp.push_front(current_class[*it]);
        current_class.removeAt(*it);
        if (*it < row)
            row--;
    }

    QStringList::const_iterator string_it;
    for (string_it = temp.constBegin();
         string_it != temp.constEnd();string_it++)
        current_class.insert(row, (*string_it));
    list->clear();
    delete list;
    current_date_modified = true;
    model->setStringList(current_class);
}

bool dance_class::read_mainFile(const QString &fileName)
{
    qDebug() << "READ CLASS MAINFILE";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Dance class"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    quint32 magic;
    in >> magic;
    if (magic != MagicNumber) {
        QMessageBox::warning(this, tr("Dance class"),
                             tr("The file is not a Dance class file."));
        return false;
    }

    QDate* temp = NULL;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    clear();
    while (!in.atEnd()) {
        temp = new QDate;
        in >> *temp;
        all_classes.push_back(temp);

    }
    set_date_format(all_classes, underline);
    class_path = QFileInfo(fileName).absolutePath();
    changed_date(current_date);
    QApplication::restoreOverrideCursor();
    return true;
}

void dance_class::clear()//TODO: delete: all_classes, current_class, current_date
{
    set_date_format(all_classes, standart);
    all_classes.clear();
}

void dance_class::set_date_format(QVector<QDate*> dates, QTextCharFormat format)
{
    for (int i = 0; i < dates.size(); i++)
        calendar->setDateTextFormat(*(dates[i]), format);
}

bool dance_class::write_mainFile(const QString &fileName)
{
    qDebug() << "WRITE CLASS MAINFILE";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Dance class"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint32(MagicNumber);
    QVector<QDate*>::const_iterator it = all_classes.begin();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while (it != all_classes.end())
    {
        out << **it;
        it++;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool dance_class::readFile(const QString &fileName)
{
    qDebug() << "READ CLASS FILE";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Dance class"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    quint32 magic;
    in >> magic;
    if (magic != MagicNumber) {
        QMessageBox::warning(this, tr("Dance class"),
                             tr("The file is not a Dance class file."));
        return false;
    }

    QString temp;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while (!in.atEnd()) {
        in >> temp;
        current_class.push_back(temp);
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool dance_class::writeFile(const QString &fileName)
{
    qDebug() << "WRITE CLASS FILE";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Dance class"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint32(MagicNumber);
    QStringList::const_iterator it = current_class.begin();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while (it != current_class.end())
    {
        out << *it;
        it++;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool dance_class::removeFile(const QString &fileName)
{
    qDebug() << "REMOVE CLASS FILE";
    QFile file(fileName);
    if (!file.remove()) {
        QMessageBox::warning(this, tr("Dance class"),
                             tr("Cannot remove file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    return true;
}

void dance_class::changed_date(QDate date)
{
    save_current_date();
    current_date = date;
    current_date_modified = false;
    current_class.clear();
    if (find_date(date))
        readFile(class_path + '/' + date.toString("dd_MM_yyyy"));
    qDebug() << "CHANGED DATE" << date;
    model->setStringList(current_class);
    music->setText("");
    emit modified_date();
}

bool dance_class::find_date(QDate date)//TODO: Optimize search
{
    for (int i = 0; i < all_classes.size(); i++)
        if (*(all_classes[i]) == date)
            return true;
    return false;
}

bool dance_class::save_current_date()
{
    if (current_date_modified)
    {
        if (current_class.empty())
        {
            bool result = false;
            if (calendar->dateTextFormat(current_date) == underline)
                result = removeFile(class_path + '/' + current_date.toString("dd_MM_yyyy"));
            delete_date(current_date);
            return result;
        } else {
            add_date(current_date);
            return writeFile(class_path + '/' + current_date.toString("dd_MM_yyyy"));
        }
    }
    return true;
}

void dance_class::add_date(QDate date)
{
    if (calendar->dateTextFormat(date) != underline)
    {
        calendar->setDateTextFormat(date, underline);
        QDate* temp = new QDate;
        *temp = date;
        all_classes.push_back(temp);
    }
}

void dance_class::delete_date(QDate date)
{
    if (calendar->dateTextFormat(date) == underline)
    {
        calendar->setDateTextFormat(date, standart);
        for (int i = 0; i < all_classes.size(); i++)
            if (*(all_classes[i]) == date)
                all_classes.remove(i);
    }
}

void dance_class::set_music(QModelIndex index)
{
    qDebug() << "SET MUSIC:" << index.data();
    music->setText((dancelist->get_dance(index.data().toString()))->get_music());
}
