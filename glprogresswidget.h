#ifndef GLPROGRESSWIDGET_H
#define GLPROGRESSWIDGET_H

#include <QGLWidget>
#include <QVector>
#include "mathalgorithms.h"

class glProgressWidget : public QGLWidget
{
public:
    glProgressWidget();
    ~glProgressWidget();

protected:

    //default widget gl functions
    void initializeGL(); //called once before drawing happens
    void paintGL(); //called during draw call back
    void resizeGL(int width, int height); //called when widget size changes
    void mousePressEvent(QMouseEvent *event); //when mouse is pressed inside the area
};

#endif // GLPROGRESSWIDGET_H
