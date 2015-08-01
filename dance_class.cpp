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

    model = new QStringListModel(this);
    //model->setStringList(current_dance);

    listView = new QListView(this);
    listView->setModel(model);
    listView->setEditTriggers(QAbstractItemView::DoubleClicked);
    calendar = new QCalendarWidget(this);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(calendar);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(listView);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
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
    while (!in.atEnd()) {
        temp = new QDate;
        in >> *temp;
        all_classes.push_back(temp);
    }
    QApplication::restoreOverrideCursor();
    return true;
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
    QList<QDate*>::const_iterator it = all_classes.begin();
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

    int temp = 0;
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
    QList<int>::const_iterator it = current_class.begin();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while (it != current_class.end())
    {
        out << *it;
        it++;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

void dance_class::get_current_dance(QStringList list) const
{
    QList<int>::const_iterator it = current_class.begin();
    while (it != current_class.end())
    {
        list.push_back(dancelist->get_name_dance(*it));
        it++;
    }
}
