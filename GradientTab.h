#ifndef GRADIENTTAB_H
#define GRADIENTTAB_H

#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include "MainWindow.h"

class MainWindow;

class GradientTab : public QWidget {
    Q_OBJECT
public:
    explicit GradientTab(QWidget *parent = nullptr, MainWindow *win = nullptr);
private:
    MainWindow *win;
    QFormLayout *mainLayout;
    void editGradient(int col1[3], int col2[3], int col3[3], int col4[3], int col5[3], int inColor[3]);
    void randomColors();
};

#endif