#include <QtGui>
#include "dance_list.h"
#include "dance_t.h"

dance_list::dance_list(QWidget *parent) :
    QWidget(parent)
{
    if (!readFile())
        writeFile();
    new_dance_dialog = 0;
    edit_list_dialog = 0;
    modified = false;
}

dance_list::~dance_list()
{
    qDeleteAll(dance_vector);
    dance_vector.clear();
}

bool dance_list::readFile()
{
    qDebug() << "READ LIST FILE";
    QFile file("dance_list");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Dance list"),
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
        QMessageBox::warning(this, tr("Dance list"),
                             tr("The file is not a Dance list file."));
        return false;
    }

    dance_t* temp = NULL;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    while (!in.atEnd()) {
        temp = new dance_t;
        in >> *temp;
        dance_vector.push_back(temp);
    }

    make_alphabetical_order();
    make_fast_order();
    QApplication::restoreOverrideCursor();
    return true;
}

bool dance_list::writeFile()
{
    qDebug() << "WRITE LIST FILE";
    QFile file("dance_list");
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Dance list"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint32(MagicNumber);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = 0; i < dance_vector.count(); i++)
        out << *(dance_vector[i]);

    QApplication::restoreOverrideCursor();
    return true;
}

void dance_list::make_alphabetical_order()
{
    if (dance_vector.count() > 1)
    {
        qDebug() << "QSORT OF LIST";
        vector_qsort(0, dance_vector.count() - 1);
    }
}

void dance_list::vector_qsort(int first, int last)
{
    int i = first, j = last;
    QString x = dance_vector[(first + last) / 2]->get_name();
    do {
        while (QString::localeAwareCompare(dance_vector[i]->get_name(), x) < 0) i++;
        while (QString::localeAwareCompare(dance_vector[j]->get_name(), x) > 0) j--;

        if(i <= j) {
            if (i < j)
            {
                dance_t* temp = dance_vector[i];
                dance_vector[i] = dance_vector[j];
                dance_vector[j] = temp;
            }
            i++;
            j--;
        }
    } while (i <= j);
    if (i < last)
        vector_qsort(i, last);
    if (first < j)
        vector_qsort(first, j);
}

QString dance_list::dump()
{
    QString dump = "DANCE LIST: ";
    for (int i = 0; i < dance_vector.count(); i++)
        dump += " " + dance_vector[i]->get_name();
    return dump;
}

void dance_list::make_fast_order()
{
    fast_find_order.clear();
    fast_number_order.clear();
    for (int i = 0; i < dance_vector.count(); i++)
    {
        fast_find_order[(dance_vector[i])->get_name()] = dance_vector[i];
        fast_number_order[(dance_vector[i])->get_name()] = i;
    }
}

void dance_list::list_delete(int dance)
{
    if (QMessageBox::warning(this, tr("Dance class"),
                             tr("The dance must be deleted from all classes.\n"
                                "The existing of dance in classes will create wrong empty dance.\n"
                                "Do you want to delete dance?"),
                             QMessageBox::Yes | QMessageBox::No)
            == QMessageBox::Yes)
    {
        dance_vector.remove(dance);
        make_alphabetical_order();
        make_fast_order();
        modified_list_slot();
        list_edit_button();
    }
}

void dance_list::add_dance(dance_t* dance)
{
    dance_vector.push_back(dance);
    make_alphabetical_order();
    make_fast_order();
    modified_list_slot();
}

void dance_list::list_edit_button()
{
    qDebug() << "EDIT";
    if (!edit_list_dialog)
    {
        edit_list_dialog = new edit_list_d(&dance_vector, this);
        connect(edit_list_dialog, SIGNAL(save_changes()),
                this, SLOT(modified_list_slot()));
        connect(edit_list_dialog, SIGNAL(add_dance(QString)),
                this, SIGNAL(add_dance_to_lesson(QString)));
        connect(edit_list_dialog, SIGNAL(new_dance()),
                this, SLOT(list_new_button()));
        connect(edit_list_dialog, SIGNAL(delete_dance(int)),
                this, SLOT(list_delete(int)));
    }
    if (modified)
        edit_list_dialog->update_dancelist();

    edit_list_dialog->show();
    edit_list_dialog->raise();
    edit_list_dialog->activateWindow();
}

void dance_list::open_dance(QString dance)
{
    list_edit_button();
    edit_list_dialog->load_dance(get_dance_number(dance));
}

void dance_list::list_new_button()
{
    qDebug() << "NEW";
    if (!new_dance_dialog)
    {
        new_dance_dialog = new new_dance_d(&dance_vector, this);
        connect(new_dance_dialog, SIGNAL(send_dance(dance_t*)),
                this, SLOT(list_add(dance_t*)));
    }

    new_dance_dialog->show();
    new_dance_dialog->raise();
    new_dance_dialog->activateWindow();
}

void dance_list::modified_list_slot()
{
    qDebug() << "LIST IS MODIFIED";
    modified = true;
}

QString dance_list::get_name_dance(int place) const
{
    return dance_vector[place]->get_name();
}

dance_t* dance_list::get_dance(int place) const
{
    return dance_vector[place];
}

dance_t* dance_list::get_dance(QString name) const
{
    return fast_find_order[name];
}

int dance_list::get_dance_number(QString name) const
{
    return fast_number_order[name];
}

int dance_list::get_dance_number(dance_t* dance) const
{
    return get_dance_number(dance->get_name());
}

bool dance_list::find_name(QString name)
{
    if (fast_find_order.find(name) == fast_find_order.end())
        return false;
    return true;
}
