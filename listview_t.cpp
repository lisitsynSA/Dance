#include "listview_t.h"

QList<int>* listview_t::get_selectedIndexes()
{//descending
    QList<int>* result = new QList<int>;
    QVector<int> temp;
    QModelIndexList list = selectedIndexes();
    QModelIndexList::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); it++)
        temp.push_back(it->row());

    if (!temp.empty())
    {
        for (int j = 0; j < temp.size() - 1; j++)
        {
            for (int i = 0; i < temp.size() - j - 1; i++)
                if (temp[i] > temp[i+1])
                {
                    int tmp = temp[i];
                    temp[i] = temp[i+1];
                    temp[i+1] = tmp;
                }
            result->push_back(temp[temp.size() - j - 1]);
        }
        result->push_back(temp[0]);
    }
    return result;
}

int listview_t::get_row(QPoint pos)
{//we should drop before row
    int size = viewOptions().decorationSize.height();
    return (pos.y() + size/2)/size;
}

void listview_t::dropEvent(QDropEvent * event)
{
    emit drop_signal(get_row(event->pos()), get_selectedIndexes());
}
