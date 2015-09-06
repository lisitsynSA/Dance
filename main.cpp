#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Dacne class");
    app.setOrganizationName("KIM MIPT");
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
