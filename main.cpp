#include <QApplication>
#include <QTextCodec>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Dacne class");
    app.setOrganizationName("KIM MIPT");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
