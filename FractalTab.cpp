#include "FractalTab.h"
#include "MainWindow.h"
#include <vector>

FractalTab::FractalTab(QWidget *parent, MainWindow *win) : QWidget(parent) {
    this->win = win;
    mainLayout = new QFormLayout;

    QStringList fractalTypes = {"Mandelbrot", "Tricorn", "Burning Ship", "Perpendicular Mandelbrot", "Perpendicular Burning Ship", "Celtic Mandelbrot", "Celtic Mandelbar", "Perpendicular Celtic", "Buffalo", "Perpendicular Buffalo", "Mandelship", "Mandelbrot Heart", "Buffalo Heart", "Lamda Mandelbrot / Logistic Map", "Custom"};
    fractalType = new QComboBox;
    fractalType->addItems(fractalTypes);
    QHBoxLayout *FTLayout = new QHBoxLayout;
    FTLayout->addWidget(fractalType);
    connect(fractalType, QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){ changeFractal(index); });

    auto Power = new QDoubleSpinBox;
    Power->setSingleStep(0.01);
    Power->setDecimals(10);
    Power->setValue(2);
    QLabel *PowerL = new QLabel;
    QHBoxLayout *PowerLayout = new QHBoxLayout;
    PowerLayout->addWidget(Power);
    connect(Power, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double value){ changePower(value); });

    auto StartingP = new QHBoxLayout;
    auto StartRe = new QLineEdit;
    StartRe->setValidator(new QDoubleValidator);
    StartRe->setPlaceholderText("Real");
    StartRe->setText("0.0");
    auto StartIm = new QLineEdit;
    StartIm->setValidator(new QDoubleValidator);
    StartIm->setPlaceholderText("Imag");
    StartIm->setText("0.0");
    StartingP->addWidget(StartRe);
    StartingP->addWidget(StartIm);
    auto SPLayout = new QHBoxLayout;
    SPLayout->addLayout(StartingP);

    connect(StartRe, QOverload<const QString &>::of(&QLineEdit::textChanged), [=](const QString &value){changeStartP(value.toDouble(), win->glWidget->StartY);});
    connect(StartIm, QOverload<const QString &>::of(&QLineEdit::textChanged), [=](const QString &value){changeStartP(win->glWidget->StartX, value.toDouble());});

    auto iterations = new QLineEdit;
    iterations->setValidator(new QIntValidator);
    iterations->setText(QString::number(200));
    QHBoxLayout *itrLayout = new QHBoxLayout;
    itrLayout->addWidget(iterations);
    connect(iterations, QOverload<const QString &>::of(&QLineEdit::textChanged), [=](const QString &value){changeIterations(value.toInt());});

    Zoom = new QLineEdit;
    Zoom->setValidator(new QDoubleValidator);
    QHBoxLayout *zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(Zoom);

    auto juliaTrigger = new QCheckBox;
    juliaTrigger->setChecked(false);
    QHBoxLayout *JTLayout = new QHBoxLayout;
    JTLayout->addWidget(juliaTrigger);
    connect(juliaTrigger, QOverload<bool>::of(&QCheckBox::clicked), [=](bool value){setJulia(value);});

    auto timer = new QTimer;
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&FractalTab::info));
    timer->start();

    mainLayout->addRow("Fractal", FTLayout);
    mainLayout->addRow("Power", PowerLayout);
    mainLayout->addRow("Seed", SPLayout);
    mainLayout->addRow("Iterations", itrLayout);
    mainLayout->addRow("Zoom", zoomLayout);
    mainLayout->addRow("Julia Set", JTLayout);
    setLayout(mainLayout);
}

void FractalTab::changeFractal(int index) {
    win->glWidget->curFractal = index;
    win->glWidget->update();
    win->glWidget->glUniform1i(win->glWidget->glGetUniformLocation(win->glWidget->program, "FRACTAL_TYPE"), win->glWidget->curFractal);
}

void FractalTab::changePower(double value) {
    win->glWidget->update();
    win->glWidget->power = value;
    win->glWidget->glUniform1f(win->glWidget->glGetUniformLocation(win->glWidget->program, "POWER"), (float)(win->glWidget->power));
}

void FractalTab::changeStartP(double re, double im) {
    win->glWidget->update();
    win->glWidget->StartX = re;
    win->glWidget->StartY = im;
    win->glWidget->glUniform2d(win->glWidget->glGetUniformLocation(win->glWidget->program, "Start"), win->glWidget->StartX, win->glWidget->StartY);
}

void FractalTab::setJulia(bool value) {
    win->glWidget->update();
    win->glWidget->isJulia = value;

    if (value) {
        win->glWidget->StartX = -1.0;
        win->glWidget->StartY = -0.5;
    } else {
        win->glWidget->StartX = 0.0;
        win->glWidget->StartY = 0.0;
    }

    win->glWidget->glUniform1f(win->glWidget->glGetUniformLocation(win->glWidget->program, "juliaEnabled"), win->glWidget->isJulia);
}

void FractalTab::changeIterations(int value) {
    win->glWidget->itr = value;
    win->glWidget->glUniform1i(win->glWidget->glGetUniformLocation(win->glWidget->program, "itr"), win->glWidget->itr);
    win->glWidget->update();
}

void FractalTab::info() {
    Zoom->setText(QString::number(win->glWidget->zoom));
}