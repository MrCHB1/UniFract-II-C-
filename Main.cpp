#include "Quadruple.h"
#include "MainWindow.h"
#include <iostream>

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow win;
    win.show();
    return app.exec();
}