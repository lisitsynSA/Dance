#include "audioplayer.h"

audioplayer::audioplayer(QWidget *parent) :
    QWidget(parent),
    m_AudioOutput(Phonon::MusicCategory)
{
    QSize buttonSize(33, 27);

    playButton = new QPushButton(this);
    play_icon = style()->standardIcon(QStyle::SP_MediaPlay);
    pause_icon = style()->standardIcon(QStyle::SP_MediaPause);
    playButton->setIcon(play_icon);
    backwardButton = new QPushButton(this);
    backwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    forwardButton = new QPushButton(this);
    forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    stopButton = new QPushButton(this);
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    playButton->setEnabled(false);
    backwardButton->setEnabled(false);
    forwardButton->setEnabled(false);
    stopButton->setEnabled(false);

    slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(&m_MediaObject);
    volume = new Phonon::VolumeSlider(&m_AudioOutput);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(8, 8, 8, 8);

    listView = new QListView(this);
    listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setDragDropMode(QAbstractItemView::NoDragDrop);

    vLayout->addWidget(listView);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    name_music = new QLabel(this);
    name_music->setText("Name of Music");
    hLayout->addWidget(name_music);

    finish_box = new QComboBox(this);
    finish_box->addItem("Without repeating");
    finish_box->addItem("With repeating");
    finish_box->addItem("All music");

    hLayout->addWidget(finish_box);
    vLayout->addLayout(hLayout);

    QHBoxLayout *layout = new QHBoxLayout();

    QPalette palette;
    palette.setBrush(QPalette::WindowText, Qt::white);

    backwardButton->setMinimumSize(buttonSize);
    forwardButton->setMinimumSize(buttonSize);
    playButton->setMinimumSize(buttonSize);
    stopButton->setMinimumSize(buttonSize);

    volume->setFixedWidth(120);

    layout->addWidget(backwardButton);
    layout->addWidget(stopButton);
    layout->addWidget(playButton);
    layout->addWidget(forwardButton);

    layout->addStretch();
    layout->addWidget(volume);

    QVBoxLayout *buttonPanelLayout = new QVBoxLayout();
    buttonPanelLayout->addLayout(layout);

    timeLabel = new QLabel(this);
    progressLabel = new QLabel(this);
    QWidget *sliderPanel = new QWidget(this);
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(timeLabel);
    sliderLayout->addWidget(progressLabel);
    sliderLayout->setContentsMargins(0, 0, 0, 0);
    sliderPanel->setLayout(sliderLayout);

    buttonPanelLayout->addWidget(sliderPanel);
    buttonPanelLayout->setContentsMargins(0, 0, 0, 0);
    QWidget *buttonPanelWidget = new QWidget(this);
    buttonPanelWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttonPanelWidget->setLayout(buttonPanelLayout);
    vLayout->addWidget(buttonPanelWidget);

    QHBoxLayout *labelLayout = new QHBoxLayout();

    vLayout->addLayout(labelLayout);
    setLayout(vLayout);

    //SIGNALS

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(play_button()));
    connect(backwardButton, SIGNAL(clicked()),
            this, SLOT(backward_button()));
    connect(forwardButton, SIGNAL(clicked()),
            this, SLOT(forward_button()));
    connect(stopButton, SIGNAL(clicked()),
            this, SLOT(stop_button()));
    connect(listView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(open_music(QModelIndex)));

    connect(&m_MediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(update_time()));
    connect(&m_MediaObject, SIGNAL(tick(qint64)), this, SLOT(update_time()));
    connect(&m_MediaObject, SIGNAL(finished()), this, SLOT(finished()));
    connect(&m_MediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(state_changed(Phonon::State,Phonon::State)));

    //CHECK DIR
    QDir dir = QDir::current();
    if (!dir.exists("music"))
        if(!dir.mkdir("music"))
            QMessageBox::warning(this, tr("Dance class"),
                                 tr("Cannot create directory 'music'."));
    music_dir = QDir(dir.absoluteFilePath("music"));

    m_audioOutputPath = Phonon::createPath(&m_MediaObject, &m_AudioOutput);

    model = new QStringListModel(this);
    listView->setModel(model);
    set_list(current_music);
}

void audioplayer::set_file(const QString &file, bool play)
{
    name_music->setText(file);
    m_MediaObject.setCurrentSource(Phonon::MediaSource("music/" + file));
    if (play)
        m_MediaObject.play();
}

void audioplayer::set_volume(qreal volume)
{
    m_AudioOutput.setVolume(volume);
}

void audioplayer::set_list(QStringList music)
{
    qDebug() << "SET LIST OF MUSIC";
    music_size = music.count();
    if (music.isEmpty())
    {
        playButton->setEnabled(false);
        backwardButton->setEnabled(false);
        forwardButton->setEnabled(false);
        stopButton->setEnabled(false);
    } else {
        playButton->setEnabled(true);
        stopButton->setEnabled(true);
        if (++music.begin() != music.end())
        {
            backwardButton->setEnabled(true);
            forwardButton->setEnabled(true);
        } else {
            backwardButton->setEnabled(false);
            forwardButton->setEnabled(false);
        }
    }
    current_music = music;
    music_index = 0;
    load_index(false);
    model->setStringList(current_music);
}

void audioplayer::play_button()
{
    if (m_MediaObject.state() == Phonon::PlayingState)
        m_MediaObject.pause();
    else
    {
        if (m_MediaObject.currentTime() == m_MediaObject.totalTime())
            m_MediaObject.seek(0);
        m_MediaObject.play();
    }
}

void audioplayer::backward_button()
{
    m_MediaObject.stop();
    if (--music_index == -1)
        music_index = music_size - 1;
    load_index();
}

void audioplayer::forward_button()
{
    m_MediaObject.stop();
    if (++music_index >= music_size)
        music_index = 0;
    load_index();
}

void audioplayer::load_index(bool play)
{
    qDebug() << "LOAD MUSIC FROM INDEX: " << music_index;
    if (current_music.isEmpty())
        return;

    QString music_name = current_music[music_index];
    if (check_music(music_name))
        set_file(music_name, play);
}

void audioplayer::stop_button()
{
    m_MediaObject.stop();
}

void audioplayer::update_time()
{
    long len = m_MediaObject.totalTime();
    long pos = m_MediaObject.currentTime();
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    timeLabel->setText(timeString);

}

void audioplayer::open_music(QModelIndex index)
{
    qDebug() << "OPEN MUSIC:" << index.data();
    if (check_music(index.data().toString()))
    {
        set_file(index.data().toString());
        music_index = index.row();
    }
}

void audioplayer::finished()
{
    switch (finish_box->currentIndex())
    {
    case 1: //with repeating
        load_index();
        break;
    case 2: //all music
        forward_button();
        break;
    case 0: //without repeting
    default:
        break;
    }
}

bool audioplayer::check_music(QString music)
{
    if (music_dir.exists(music))
        return true;
    QMessageBox::warning(this, tr("Loading of the music"),
                         tr("Can't find music '%1'.")
                         .arg(music));
    qDebug() << "Can't find music: " << music;
    return false;
}

void audioplayer::state_changed(Phonon::State new_state, Phonon::State old_state)
{
    Q_UNUSED(old_state);

    switch (new_state) {
        case Phonon::ErrorState:
            if (m_MediaObject.errorType() == Phonon::FatalError) {
                playButton->setEnabled(false);
                backwardButton->setEnabled(false);
                forwardButton->setEnabled(false);
                stopButton->setEnabled(false);
            } else {
                m_MediaObject.pause();
            }
            QMessageBox::warning(this, "Phonon Mediaplayer", m_MediaObject.errorString(), QMessageBox::Close);
            break;

        case Phonon::StoppedState:
        case Phonon::PausedState:
            playButton->setIcon(play_icon);
            if (m_MediaObject.currentSource().type() != Phonon::MediaSource::Invalid){
                playButton->setEnabled(true);
            } else {
                playButton->setEnabled(false);
            }
            break;
        case Phonon::PlayingState:
            playButton->setEnabled(true);
            playButton->setIcon(pause_icon);
            // Fall through
        case Phonon::BufferingState:
            break;
        case Phonon::LoadingState:
            break;
    }
}
