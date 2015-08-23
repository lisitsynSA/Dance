#include <QtGui>

#include "main_window.h"
#include "dance_class.h"
#include "dance_list.h"

MainWindow::MainWindow()
{
    dancelist = new dance_list(this);
    danceclass = new dance_class(dancelist, this);
    connect(danceclass, SIGNAL(status_bar(QString)),
            this, SLOT(msg_statusBar(QString)));
    connect(dancelist, SIGNAL(add_dance_to_lesson(QString)),
            danceclass, SLOT(add_dance(QString)));
    setCentralWidget(danceclass);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();

    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile("");
    loadFile("dc/dances.dc", true);//TODO:delete repeating of recent files
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Dance class"), ".",
                                   tr("Dance class files (*.dc)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Dance Class"),
            tr("<h2>Dance Class 1.1</h2>"
               "<p>Copyright &copy; 2015 Lisitsyn Sergey(KIM MIPT)."
               "<p>Dance Class is a small application that "
               "helps to plan and track training."));
}

void MainWindow::updateStatusBar()
{
    dateLabel->setText(danceclass->get_current_date().toString("dd.MM.yy (ddd)"));
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    loadAction = new QAction(tr("&Load"), this);
    loadAction->setShortcut(QKeySequence::Copy);
    loadAction->setIcon(QIcon(":/images/load.png"));
    loadAction->setStatusTip(tr("Copy current dance class "
                               "to the buffer"));
    connect(loadAction, SIGNAL(triggered()),
            danceclass, SLOT(load_button()));

    addAction = new QAction(tr("&Add"), this);
    addAction->setIcon(QIcon(":/images/plus.png"));
    addAction->setShortcut(tr("Insert"));
    addAction->setStatusTip(tr("Add the new dance "
                               "to the current lesson"));
    connect(addAction, SIGNAL(triggered()),
            dancelist, SLOT(list_edit_button()));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/images/minus.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete the current selection's "
                                  "contents"));
    connect(deleteAction, SIGNAL(triggered()),
            danceclass, SLOT(del_button()));

    listNewAction = new QAction(tr("&New"), this);
    listNewAction->setIcon(QIcon(":/images/new_dance.png"));
    listNewAction->setStatusTip(tr("Create a new dance in the dictionary"));
    connect(listNewAction, SIGNAL(triggered()),
            dancelist, SLOT(list_new_button()));

    showSpeedAction = new QAction(tr("Show &Speed"), this);
    showSpeedAction->setCheckable(true);
    showSpeedAction->setChecked(danceclass->get_showSpeed());
    showSpeedAction->setStatusTip(tr("Show or hide speed flag"));
    connect(showSpeedAction, SIGNAL(toggled(bool)),
            danceclass, SLOT(set_showSpeed(bool)));

    showRepeatingAction = new QAction(tr("Show &Repeating"), this);
    showRepeatingAction->setCheckable(true);
    showRepeatingAction->setChecked(danceclass->get_showRepeating());
    showRepeatingAction->setStatusTip(tr("Show or hide repeating flag"));
    connect(showRepeatingAction, SIGNAL(toggled(bool)),
            danceclass, SLOT(set_showRepeating(bool)));

    showTimeAction = new QAction(tr("Show &Time"), this);
    showTimeAction->setCheckable(true);
    showTimeAction->setChecked(danceclass->get_showTime());
    showTimeAction->setStatusTip(tr("Show or hide time signature flag"));
    connect(showTimeAction, SIGNAL(toggled(bool)),
            danceclass, SLOT(set_showTime(bool)));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    dance_classMenu = menuBar()->addMenu(tr("Dance &class"));
    dance_classMenu->addAction(loadAction);
    dance_classMenu->addAction(addAction);
    dance_classMenu->addAction(deleteAction);
    dance_classMenu->addAction(listNewAction);
    dance_classMenu->addSeparator();
    dance_classMenu->addAction(exitAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showSpeedAction);
    optionsMenu->addAction(showRepeatingAction);
    optionsMenu->addAction(showTimeAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createContextMenu()
{
    danceclass->addAction(loadAction);
    danceclass->addAction(addAction);
    danceclass->addAction(deleteAction);
    danceclass->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(loadAction);
    editToolBar->addAction(addAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(listNewAction);
}

void MainWindow::createStatusBar()
{
    dateLabel = new QLabel("00.00.00 (AA)",this);
    dateLabel->setAlignment(Qt::AlignHCenter);
    dateLabel->setMinimumSize(dateLabel->sizeHint());

    statusBar()->addWidget(dateLabel);
    connect(danceclass, SIGNAL(modified_date()),
            this, SLOT(updateStatusBar()));

    updateStatusBar();
}

void MainWindow::readSettings()
{
    QSettings settings("LiSA", "Dance class");

    restoreGeometry(settings.value("geometry").toByteArray());

    bool showSpeed = settings.value("showSpeed", true).toBool();
    showSpeedAction->setChecked(showSpeed);

    bool showRepeating = settings.value("showRepeating", true).toBool();
    showRepeatingAction->setChecked(showRepeating);

    bool showTime = settings.value("showTime", true).toBool();
    showTimeAction->setChecked(showTime);
}

void MainWindow::writeSettings()
{
    QSettings settings("LiSA", "Dance class");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("showSpeed", showSpeedAction->isChecked());
    settings.setValue("showRepeating", showRepeatingAction->isChecked());
    settings.setValue("showTime", showTimeAction->isChecked());
}

bool MainWindow::okToContinue()
{
    danceclass->save_current_date();
    saveFile(curFile);
    if (dancelist->is_modified())
    {
        int r = QMessageBox::warning(this, tr("Dance class"),
                        tr("The list of the dances has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return dancelist->writeFile();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::loadFile(const QString &fileName, bool first_load)
{
    if (!danceclass->read_mainFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    if (!first_load)
        statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

void MainWindow::msg_statusBar(const QString &msg)
{
    statusBar()->showMessage(msg, 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (!danceclass->write_mainFile(fileName)) {
        statusBar()->showMessage(tr("Saving canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty())
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Dance class")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
