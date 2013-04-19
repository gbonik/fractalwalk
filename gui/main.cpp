#include "main_window.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    MainWindow* win = new MainWindow();
    win->show();
    return app.exec();
}

