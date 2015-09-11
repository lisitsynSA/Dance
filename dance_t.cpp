#include <QString>
#include <QStringList>
#include "dance_t.h"

dance_t::dance_t()
{
    name = new QString();
    short_description = new QString();
    description = new QString();
    music = new QStringList();
}

dance_t::dance_t(QString init_name, QString init_short_description, QString init_description, QStringList init_music)
{
    name = new QString(init_name);
    short_description = new QString(init_short_description);
    description = new QString(init_description);
    music = new QStringList(init_music);
}

dance_t::~dance_t()
{
    music->clear();
    delete music;
    delete description;
    delete short_description;
    delete name;
}

QDataStream& operator << (QDataStream& stream, dance_t &op)
{
    stream << *op.name << *op.short_description << *op.description << *op.music;
    return stream;
}

QDataStream& operator << (QDataStream& stream, const dance_t &op)
{
    stream << *op.name << *op.short_description << *op.description << *op.music;
    return stream;
}

QDataStream& operator >> (QDataStream& stream, dance_t &op)
{
    stream >> *op.name >> *op.short_description >> *op.description >> *op.music;
    return stream;
}
