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

    name_music = new QLabel(this);
    name_music->setText("Name of Music");
    vLayout->addWidget(name_music);

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

    m_audioOutputPath = Phonon::createPath(&m_MediaObject, &m_AudioOutput);

    model = new QStringListModel(this);
    listView->setModel(model);


    current_music.push_back("black horse.mp3");
    current_music.push_back("Sappy.mp3");
    model->setStringList(current_music);

    playButton->setEnabled(true);
    backwardButton->setEnabled(true);
    forwardButton->setEnabled(true);
    stopButton->setEnabled(true);
}

void audioplayer::set_file(const QString &file)
{
    name_music->setText(file);
    m_MediaObject.setCurrentSource(Phonon::MediaSource("music/" + file));
    m_MediaObject.play();
}

void audioplayer::set_volume(qreal volume)
{
    m_AudioOutput.setVolume(volume);
}

void audioplayer::get_list(QStringList* music)
{

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

}

void audioplayer::forward_button()
{

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
        set_file(index.data().toString());
}

void audioplayer::finished()
{

}

bool audioplayer::check_music(QString music)
{
    return true;
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
