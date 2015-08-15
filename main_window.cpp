#include <QtGui>

#include "main_window.h"
#include "dance_class.h"
#include "dance_list.h"

MainWindow::MainWindow()
{
    dancelist = new dance_list(this);
    danceclass = new dance_class(dancelist, this);
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

void MainWindow::newFile()
{
    if (okToContinue()) {
        danceclass->clear();
        setCurrentFile("");
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

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Save Dance class"), ".",
                               tr("Dance class files (*.dc)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Dance Class"),
            tr("<h2>Dance Class 1.1</h2>"
               "<p>Copyright &copy; 2015 Lisitsyn Sergey(KIM MIPT)."
               "<p>Dance Class is a small application that "
               "helps to plan and track training."));
}

void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

void MainWindow::updateStatusBar()
{
    dateLabel->setText(danceclass->get_current_date().toString("dd.MM.yy (ddd)"));
}

void MainWindow::danceclassModified()
{
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing spreadsheet file"));
    connect(openAction, SIGNAL(triggered()),
            this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the spreadsheet to disk"));
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setStatusTip(tr("Save the spreadsheet under a new "
                                  "name"));
    connect(saveAsAction, SIGNAL(triggered()),
            this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

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
            danceclass, SLOT(add_button()));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/images/minus.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete the current selection's "
                                  "contents"));
    connect(deleteAction, SIGNAL(triggered()),
            danceclass, SLOT(del_button()));

    listEditAction = new QAction(tr("&Edit"), this);
    listEditAction->setIcon(QIcon(":/images/edit.png"));
    listEditAction->setStatusTip(tr("Edit the dictionary of the dance"));
    connect(listEditAction, SIGNAL(triggered()),
            dancelist, SLOT(list_edit_button()));

    listFindAction = new QAction(tr("&Find"), this);
    listFindAction->setIcon(QIcon(":/images/find.png"));
    listFindAction->setShortcut(QKeySequence::Find);
    listFindAction->setStatusTip(tr("Find a dance in the dictionary"));
    connect(listFindAction, SIGNAL(triggered()),
            dancelist, SLOT(list_find_button()));

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
    dance_classMenu->addAction(newAction);
    dance_classMenu->addAction(openAction);
    dance_classMenu->addAction(saveAction);
    dance_classMenu->addAction(saveAsAction);
    separatorAction = dance_classMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        dance_classMenu->addAction(recentFileActions[i]);
    dance_classMenu->addSeparator();
    dance_classMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(loadAction);
    editMenu->addAction(addAction);
    editMenu->addAction(deleteAction);

    edit_listSubMenu = editMenu->addMenu(tr("&Dictionary of dance"));
    edit_listSubMenu->addAction(listEditAction);
    edit_listSubMenu->addAction(listFindAction);
    edit_listSubMenu->addAction(listNewAction);


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
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(loadAction);
    editToolBar->addAction(addAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(listEditAction);
    editToolBar->addAction(listFindAction);
    editToolBar->addAction(listNewAction);
}

void MainWindow::createStatusBar()
{
    dateLabel = new QLabel("00.00.00 (AA)",this);
    dateLabel->setAlignment(Qt::AlignHCenter);
    dateLabel->setMinimumSize(dateLabel->sizeHint());

    statusBar()->addWidget(dateLabel);

    connect(danceclass, SIGNAL(modified_class()),
            this, SLOT(danceclassModified()));
    connect(danceclass, SIGNAL(modified_date()),
            this, SLOT(updateStatusBar()));

    updateStatusBar();
}

void MainWindow::readSettings()
{
    QSettings settings("LiSA", "Dance class");

    restoreGeometry(settings.value("geometry").toByteArray());

    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

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
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showSpeed", showSpeedAction->isChecked());
    settings.setValue("showRepeating", showRepeatingAction->isChecked());
    settings.setValue("showTime", showTimeAction->isChecked());
}

bool MainWindow::okToContinue()
{
    danceclass->save_current_date();
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
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Dance class")));
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < recentFiles.count()) {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
