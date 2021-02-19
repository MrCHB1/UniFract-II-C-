#include "GradientTab.h"
#include <cstdlib>

GradientTab::GradientTab(QWidget *parent, MainWindow *win) : QWidget(parent) {
    this->win = win;
    mainLayout = new QFormLayout;

    QHBoxLayout *col1 = new QHBoxLayout;
    QHBoxLayout *col2 = new QHBoxLayout;
    QHBoxLayout *col3 = new QHBoxLayout;
    QHBoxLayout *col4 = new QHBoxLayout;
    QHBoxLayout *col5 = new QHBoxLayout;

    auto R1 = new QSlider(Qt::Horizontal);
    R1->setMaximum(100);
    R1->setMinimum(0);
    auto G1 = new QSlider(Qt::Horizontal);
    G1->setMaximum(100);
    G1->setMinimum(0);
    auto B1 = new QSlider(Qt::Horizontal);
    B1->setMaximum(100);
    B1->setMinimum(0);
    R1->setValue(win->glWidget->col1R);
    G1->setValue(win->glWidget->col1G);
    B1->setValue(win->glWidget->col1B);
    col1->addWidget(R1);
    col1->addWidget(G1);
    col1->addWidget(B1);
    connect(R1, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{value, win->glWidget->col1G, win->glWidget->col1B}, new int[3]{win->glWidget->col2R, win->glWidget->col2G, win->glWidget->col2B}, new int[3]{win->glWidget->col3R, win->glWidget->col3G, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});
    connect(G1, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, value, win->glWidget->col1B}, new int[3]{win->glWidget->col2R, win->glWidget->col2G, win->glWidget->col2B}, new int[3]{win->glWidget->col3R, win->glWidget->col3G, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});
    connect(B1, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, win->glWidget->col1G, value}, new int[3]{win->glWidget->col2R, win->glWidget->col2G, win->glWidget->col2B}, new int[3]{win->glWidget->col3R, win->glWidget->col3G, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});

    auto R2 = new QSlider(Qt::Horizontal);
    R2->setMaximum(100);
    R2->setMinimum(0);
    auto G2 = new QSlider(Qt::Horizontal);
    G2->setMaximum(100);
    G2->setMinimum(0);
    auto B2 = new QSlider(Qt::Horizontal);
    B2->setMaximum(100);
    B2->setMinimum(0);
    R2->setValue(win->glWidget->col2R);
    G2->setValue(win->glWidget->col2G);
    B2->setValue(win->glWidget->col2B);
    col2->addWidget(R2);
    col2->addWidget(G2);
    col2->addWidget(B2);
    connect(R2, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, win->glWidget->col1G, win->glWidget->col1B}, new int[3]{value, win->glWidget->col2G, win->glWidget->col2B}, new int[3]{win->glWidget->col3R, win->glWidget->col3G, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});
    connect(G2, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, win->glWidget->col1G, win->glWidget->col1B}, new int[3]{win->glWidget->col2R, value, win->glWidget->col2B}, new int[3]{win->glWidget->col3R, win->glWidget->col3G, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});
    connect(B2, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, win->glWidget->col1G, win->glWidget->col1B}, new int[3]{win->glWidget->col2R, win->glWidget->col2G, value}, new int[3]{win->glWidget->col3R, win->glWidget->col3G, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});

    auto R3 = new QSlider(Qt::Horizontal);
    R3->setMaximum(100);
    R3->setMinimum(0);
    auto G3 = new QSlider(Qt::Horizontal);
    G3->setMaximum(100);
    G3->setMinimum(0);
    auto B3 = new QSlider(Qt::Horizontal);
    B3->setMaximum(100);
    B3->setMinimum(0);
    R3->setValue(win->glWidget->col3R);
    G3->setValue(win->glWidget->col3G);
    B3->setValue(win->glWidget->col3B);
    col3->addWidget(R3);
    col3->addWidget(G3);
    col3->addWidget(B3);
    connect(R3, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, win->glWidget->col1G, win->glWidget->col1B}, new int[3]{win->glWidget->col2R, win->glWidget->col2G, win->glWidget->col2B}, new int[3]{value, win->glWidget->col3G, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});
    connect(G3, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, win->glWidget->col1G, win->glWidget->col1B}, new int[3]{win->glWidget->col2R, win->glWidget->col2G, win->glWidget->col2B}, new int[3]{win->glWidget->col3R, value, win->glWidget->col3B}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});
    connect(B3, QOverload<int>::of(&QSlider::valueChanged), [=](int value){editGradient(new int[3]{win->glWidget->col1R, win->glWidget->col1G, win->glWidget->col1B}, new int[3]{win->glWidget->col2R, win->glWidget->col2G, win->glWidget->col2B}, new int[3]{win->glWidget->col3R, win->glWidget->col3G, value}, new int[3]{win->glWidget->col4R, win->glWidget->col4G, win->glWidget->col4B}, new int[3]{win->glWidget->col5R, win->glWidget->col5G, win->glWidget->col5B}, new int[3]{win->glWidget->inColorR, win->glWidget->inColorG, win->glWidget->inColorB});});

    auto randCol = new QPushButton;
    randCol->setText("Random Colors");
    QHBoxLayout *randColLayout = new QHBoxLayout;
    randColLayout->addWidget(randCol);
    connect(randCol, QOverload<bool>::of(&QPushButton::clicked), [=](bool value){ randomColors(); });

    mainLayout->addRow("Color 1", col1);
    mainLayout->addRow("Color 2", col2);
    mainLayout->addRow("Color 3", col3);
    mainLayout->addRow(randColLayout);

    setLayout(mainLayout);
}

void GradientTab::editGradient(int col1[3], int col2[3], int col3[3], int col4[3], int col5[3], int inColor[3]) {
    win->glWidget->update();
    win->glWidget->col1R = col1[0];
    win->glWidget->col1G = col1[1];
    win->glWidget->col1B = col1[2];
    win->glWidget->col2R = col2[0];
    win->glWidget->col2G = col2[1];
    win->glWidget->col2B = col2[2];
    win->glWidget->col3R = col3[0];
    win->glWidget->col3G = col3[1];
    win->glWidget->col3B = col3[2];

    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col1"), (float)win->glWidget->col1R/100.0f, (float)win->glWidget->col1G/100.0f, (float)win->glWidget->col1B/100.0f);
    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col2"), (float)win->glWidget->col2R/100.0f, (float)win->glWidget->col2G/100.0f, (float)win->glWidget->col2B/100.0f);
    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col3"), (float)win->glWidget->col3R/100.0f, (float)win->glWidget->col3G/100.0f, (float)win->glWidget->col3B/100.0f);
}

void GradientTab::randomColors() {
    win->glWidget->update();
    win->glWidget->col1R = (float)(rand() % 101);
    win->glWidget->col1G = (float)(rand() % 101);
    win->glWidget->col1B = (float)(rand() % 101);
    win->glWidget->col2R = (float)(rand() % 101);
    win->glWidget->col2G = (float)(rand() % 101);
    win->glWidget->col2B = (float)(rand() % 101);
    win->glWidget->col3R = (float)(rand() % 101);
    win->glWidget->col3G = (float)(rand() % 101);
    win->glWidget->col3B = (float)(rand() % 101);
    win->glWidget->col4R = (float)(rand() % 101);
    win->glWidget->col4G = (float)(rand() % 101);
    win->glWidget->col4B = (float)(rand() % 101);
    win->glWidget->col5R = (float)(rand() % 101);
    win->glWidget->col5G = (float)(rand() % 101);
    win->glWidget->col5B = (float)(rand() % 101);

    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col1"), (float)win->glWidget->col1R/100.0f, (float)win->glWidget->col1G/100.0f, (float)win->glWidget->col1B/100.0f);
    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col2"), (float)win->glWidget->col2R/100.0f, (float)win->glWidget->col2G/100.0f, (float)win->glWidget->col2B/100.0f);
    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col3"), (float)win->glWidget->col3R/100.0f, (float)win->glWidget->col3G/100.0f, (float)win->glWidget->col3B/100.0f);
    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col3"), (float)win->glWidget->col4R/100.0f, (float)win->glWidget->col4G/100.0f, (float)win->glWidget->col4B/100.0f);
    win->glWidget->glUniform3f(win->glWidget->glGetUniformLocation(win->glWidget->program, "col3"), (float)win->glWidget->col5R/100.0f, (float)win->glWidget->col5G/100.0f, (float)win->glWidget->col5B/100.0f);
}