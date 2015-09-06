#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QWidget>
#include <QListView>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDir>
#include <QMessageBox>
#include <QTime>

#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/seekslider.h>
#include <phonon/volumeslider.h>

class QLabel;
class QStringListModel;

class audioplayer : public QWidget
{
    Q_OBJECT
public:
    audioplayer(QWidget *parent = 0);
    void set_file(const QString &file);
    void set_volume(qreal volume);

public slots:
    void get_list(QStringList* music);
    void play_button();
    void backward_button();
    void forward_button();
    void stop_button();
    void update_time();
    void open_music(QModelIndex index);
    void finished();

    void state_changed(Phonon::State new_state, Phonon::State old_state);
private:
    QListView *listView;
    QStringListModel *model;
    QStringList *current_music;

    QLabel *name_music;
    QIcon play_icon;
    QIcon pause_icon;
    QPushButton *playButton;
    QPushButton *backwardButton;
    QPushButton *forwardButton;
    QPushButton *stopButton;
    Phonon::SeekSlider *slider;
    QLabel *timeLabel;
    QLabel *progressLabel;
    Phonon::VolumeSlider *volume;

    Phonon::MediaObject m_MediaObject;
    Phonon::AudioOutput m_AudioOutput;
    Phonon::Path m_audioOutputPath;
};

#endif // AUDIOPLAYER_H
