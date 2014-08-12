#ifndef GLFINALWIDGET_H
#define GLFINALWIDGET_H

#include <QGLWidget>
#include <QVector>
#include "mathalgorithms.h"

class glFinalWidget : public QGLWidget
{
public:
    glFinalWidget();
    ~glFinalWidget();

protected:

    //default widget gl functions
    void initializeGL(); //called once before drawing happens
    void paintGL(); //called during draw call back
    void resizeGL(int width, int height); //called when widget size changes
    void mousePressEvent(QMouseEvent *event); //when mouse is pressed inside the area
};

#endif // GLFINALWIDGET_H
