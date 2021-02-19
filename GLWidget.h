#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLFunctions_4_0_Compatibility>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>
//#include <GL/gl.h>
//#include <GLES3/gl3.h>
//#include <GLES2/gl2.h>
//#include <GLES/gl.h>
#include "Quadruple.h"


class GLWidget : public QGLWidget, public QOpenGLFunctions_4_0_Core {
public:
    unsigned int program;
    int itr = 200;
    double zoom = 100.0;
    double offsetX = 0.0;
    double offsetY = 0.0;
    Quadruple offsetX128 = Quadruple(0.0, 0.0);
    Quadruple offsetY128 = Quadruple(0.0, 0.0);

    double StartX = 0.0;
    double StartY = 0.0;

    int col1R = 10;
    int col1G = 50;
    int col1B = 100;
    int col2R = 70;
    int col2G = 0;
    int col2B = 100;
    int col3R = 89;
    int col3G = 0;
    int col3B = 10;
    int col4R = 100;
    int col4G = 50;
    int col4B = 0;
    int col5R = 100;
    int col5G = 100;
    int col5B = 12;
    int col6R = 100;
    int col6G = 100;
    int col6B = 0;
    int col7R = 50;
    int col7G = 100;
    int col7B = 0;
    int col8R = 0;
    int col8G = 100;
    int col8B = 0;
    int col9R = 0;
    int col9G = 100;
    int col9B = 50;
    int col10R = 0;
    int col10G = 100;
    int col10B = 0;

    int inColorR = 0;
    int inColorG = 0;
    int inColorB = 0;

    double oldX, oldY;
    Quadruple oldX128 = Quadruple(0.0);
    Quadruple oldY128 = Quadruple(0.0);

    double oldStartX, oldStartY;

    bool useQuadPrec = false;
    bool isJulia = false;
    bool perturbationEnabled = false;
    bool smoothColoring = true;

    double escRad = 128.0;

    int curFractal = 0;
    double power = 2.0;

    bool dragging = false;
    bool draggingParam = false;

    GLWidget(QWidget *parent = nullptr);
protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif