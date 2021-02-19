#ifndef RENDERTAB_H
#define RENDERTAB_H

#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include "MainWindow.h"

class MainWindow;

class RenderTab : public QWidget {
    Q_OBJECT
public:
    explicit RenderTab(QWidget *parent = nullptr, MainWindow *win = nullptr);
private:
    QFormLayout *mainLayout;
    MainWindow *win;

    void changeBailout(double value);
};

#endif