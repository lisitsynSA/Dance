#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    void about();
    void updateStatusBar();
    void msg_statusBar(const QString&);
    void save();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void prepare_exit();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    dance_class *danceclass;
    dance_list *dancelist;
    QLabel *dateLabel;
    QString curFile;
    QTimer* timer;

    QMenu *dance_classMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QToolBar *editToolBar;
    QAction *exitAction;
    QAction *loadAction;
    QAction *addAction;
    QAction *deleteAction;
    QAction *listNewAction;
    QAction *showSpeedAction;
    QAction *showRepeatingAction;
    QAction *showTimeAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *playAction;
};

#endif
