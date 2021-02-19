#include "RenderTab.h"

RenderTab::RenderTab(QWidget *parent, MainWindow *win) : QWidget(parent) {
    this->win = win;
    mainLayout = new QFormLayout;
    
    auto precision = new QComboBox;
    precision->addItems({"Double", "Quadruple (emulated)"});
    QHBoxLayout *precLayout = new QHBoxLayout;
    precLayout->addWidget(precision);

    auto bailout = new QLineEdit;
    bailout->setValidator(new QDoubleValidator);
    bailout->setText("128.0");
    QHBoxLayout *bailLayout = new QHBoxLayout;
    bailLayout->addWidget(bailout);
    connect(bailout, QOverload<const QString &>::of(&QLineEdit::textChanged), [=](const QString &value){changeBailout(value.toDouble());});

    mainLayout->addRow("Precision", precLayout);
    mainLayout->addRow("Bailout", bailLayout);

    setLayout(mainLayout);
}

void RenderTab::changeBailout(double value) {
    win->glWidget->update();
    win->glWidget->escRad = value;
    win->glWidget->glUniform1d(win->glWidget->glGetUniformLocation(win->glWidget->program, "bailout"), win->glWidget->escRad);
}