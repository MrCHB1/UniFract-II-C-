#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include "FractalTab.h"
#include "RenderTab.h"
#include "GradientTab.h"
#include "GLWidget.h"

class MainWindow : public QWidget {
public:
    GLWidget *glWidget;
    MainWindow(QWidget *parent = nullptr);
    void initUI();
private:
    QTabWidget *tabWidget;
    void exportImgs();
};

#endif