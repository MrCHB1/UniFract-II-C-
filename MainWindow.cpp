#include "MainWindow.h"
#include <QTabWidget>
#include <stdio.h>

#define WIDTH 800

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    this->resize(WIDTH+20,490);
    this->setWindowTitle(tr("UniFract II (C++)"));
    this->initUI();
    glWidget = new GLWidget(this);
    glWidget->resize(550,420);
    glWidget->move(5, 65);
    this->show();
}

void MainWindow::initUI() {
    auto tabs = new QTabWidget(this);
    tabs->setCurrentIndex(0);
    tabs->move(560,65);
    tabs->resize(255,420);
    tabs->addTab(new FractalTab(this, this), tr("Fractal"));
    tabs->addTab(new RenderTab(this, this), tr("Rendering"));
    tabs->addTab(new GradientTab(this, this), tr("Gradient"));

    // Main Menu

    QMenuBar *mainMenu = new QMenuBar(this);
    auto fileMenu = mainMenu->addMenu("&File");
    auto editMenu = mainMenu->addMenu("&Edit");
    auto exportImgSeries = fileMenu->addAction("Export Image Series");
    connect(exportImgSeries, QOverload<bool>::of(&QAction::triggered), [=](bool value){exportImgs();});

    // Tool Bar

    auto openCfg = new QPushButton(this);
    openCfg->setText("Open Config");
    openCfg->move(5, 30);

    auto saveCfg = new QPushButton(this);
    saveCfg->setText("Save Config");
    saveCfg->move(openCfg->width()+10, 30);
}

void MainWindow::exportImgs() {
    int frames = 0;
    while (glWidget->zoom >= 100.0) {
        auto image = glWidget->grabFrameBuffer();
        char buffer[50];
        sprintf(buffer, "fr_%04d.png", frames);
        image.save(buffer);
        glWidget->glUniform1d(glWidget->glGetUniformLocation(glWidget->program, "zoom"), glWidget->zoom);

        glWidget->zoom /= 1.02;
        frames += 1;
        glWidget->update();
    }
}