#include <QtGui>
#include "dance_list.h"
#include "dance_t.h"

dance_list::dance_list(QWidget *parent) :
    QWidget(parent)
{
    if (!readFile())
        writeFile();
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
    make_fast_find_order();
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
        out << dance_vector[i];

    QApplication::restoreOverrideCursor();
    return true;
}

void dance_list::make_alphabetical_order()
{

}

void dance_list::make_fast_find_order()
{

}

QString dance_list::get_name_dance(int place) const
{
    return alph_order.value(place)->get_name();
}

dance_t* dance_list::get_dance(int place) const
{
    return alph_order.value(place);
}

dance_t* dance_list::get_dance(QString name) const
{
    return fast_find_order.value(name);
}
