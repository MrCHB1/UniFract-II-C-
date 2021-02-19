#ifndef FRACTALTAB_H
#define FRACTALTAB_H

#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLFunctions_4_0_Compatibility>
#include "MainWindow.h"

class MainWindow;

class FractalTab : public QWidget {
    Q_OBJECT
public:
    QComboBox *fractalType;
    explicit FractalTab(QWidget *parent = nullptr, MainWindow *win = nullptr);
private:
    QFormLayout *mainLayout;
    MainWindow *win;
    QLineEdit *Zoom;
    void changeFractal(int index);
    void changePower(double value);
    void changeStartP(double re, double im);
    void setJulia(bool value);
    void changeIterations(int value);
    void info();
};

#endif