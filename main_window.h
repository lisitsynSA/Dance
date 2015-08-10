#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class dance_class;
class dance_list;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void openRecentFile();
    void updateStatusBar();
    void danceclassModified();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool okToContinue_list();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    dance_class *danceclass;
    dance_list *dancelist;
    QLabel *dateLabel;
    QStringList recentFiles;
    QString curFile;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QMenu *dance_classMenu;
    QMenu *editMenu;
    QMenu *edit_listSubMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *loadAction;
    QAction *addAction;
    QAction *deleteAction;
    QAction *listEditAction;
    QAction *listFindAction;
    QAction *listNewAction;
    QAction *showSpeedAction;
    QAction *showRepeatingAction;
    QAction *showTimeAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif
