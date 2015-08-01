#ifndef DANCE_T_H
#define DANCE_T_H

class QString;
class QDataStream;

class dance_t
{
public:
    dance_t();
    dance_t(QString init_name,
            QString init_short_description,
            QString init_description,
            QString init_music);
    ~dance_t();

    QString get_name() const { return *name; }
    QString get_short_description() const { return *short_description; }
    QString get_description() const { return *description; }
    QString get_music() const { return *music; }

    friend QDataStream& operator << (QDataStream& stream, dance_t &op);
    friend QDataStream& operator << (QDataStream& stream, const dance_t &op);
    friend QDataStream& operator >> (QDataStream& stream, dance_t &op);

private:
    QString* name;
    QString* short_description;
    QString* description;
    QString* music;


};

#endif // DANCE_T_H
