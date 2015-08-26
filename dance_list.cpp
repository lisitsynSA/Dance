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
    //TODO: qsort(dance_vector)
}

void dance_list::make_fast_order()
{
    for (int i = 0; i < dance_vector.count(); i++)
    {
        fast_find_order[(dance_vector[i])->get_name()] = dance_vector[i];
        fast_number_order[(dance_vector[i])->get_name()] = i;
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
