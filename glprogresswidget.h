#ifndef GLPROGRESSWIDGET_H
#define GLPROGRESSWIDGET_H

#include <QGLWidget>
#include <QVector>
#include "mathalgorithms.h"

class glProgressWidget : public QGLWidget
{
    struct ConstraintMatch
    {
        MathAlgorithms::Vertex vertexInMesh;
        MathAlgorithms::Vertex vertexInTexture;
    };


public:
    glProgressWidget();
    ~glProgressWidget();

    void addConstraintMatchAddVertexInMesh(MathAlgorithms::Vertex aVertex);
    void addConstraintMatchAddVertexInTexture(MathAlgorithms::Vertex aVertex);

protected:

    //default widget gl functions
    void initializeGL(); //called once before drawing happens
    void paintGL(); //called during draw call back
    void resizeGL(int width, int height); //called when widget size changes
    void mousePressEvent(QMouseEvent *event); //when mouse is pressed inside the area
    QVector<ConstraintMatch> ConstraintMatches;
};

#endif // GLPROGRESSWIDGET_H
