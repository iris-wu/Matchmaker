#ifndef GLPROGRESSWIDGET_H
#define GLPROGRESSWIDGET_H

#include <QGLWidget>
#include <QVector>
#include "mathalgorithms.h"
#include "glmeshselectwidget.h"
#define GL_PROGRESSWIDGET_CONSTRAINT_SIZE 2

class glProgressWidget : public QGLWidget
{
    struct ConstraintMatch
    {
        MathAlgorithms::Vertex vertexInMesh;
        MathAlgorithms::Vertex vertexInTexture;
    };

    struct edgeWalker
    {
        glMeshSelectWidget::vertex* startVertex;
        glMeshSelectWidget::vertex* targetVertex;
        QVector<glMeshSelectWidget::edge*> edgesIWalked;
    };

public:
    glProgressWidget();
    ~glProgressWidget();
    void performMatch();

    void addConstraintMatchAddVertexInMesh(MathAlgorithms::Vertex aVertex);
    void addConstraintMatchAddVertexInTexture(MathAlgorithms::Vertex aVertex);

protected:

    //default widget gl functions
    void initializeGL(); //called once before drawing happens
    void paintGL(); //called during draw call back
    void resizeGL(int width, int height); //called when widget size changes
    void mousePressEvent(QMouseEvent *event); //when mouse is pressed inside the area
    void walkToVertex(edgeWalker* walker);
    QVector<ConstraintMatch> ConstraintMatches;
    QVector<glMeshSelectWidget::vertex*>* meshVertices;
    QVector<glMeshSelectWidget::edge*>* meshEdges;
    QVector<glMeshSelectWidget::triangle*>* meshTriangles;
    QVector<edgeWalker*> constraintEdgesWithPoints;
};

#endif // GLPROGRESSWIDGET_H
