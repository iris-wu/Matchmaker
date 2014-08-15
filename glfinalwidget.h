#ifndef GLFINALWIDGET_H
#define GLFINALWIDGET_H

#include <QGLWidget>
#include <QVector>
#include "mathalgorithms.h"
#include "glmeshselectwidget.h"

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

private:

    GLfloat Sign(const glMeshSelectWidget::vertex& p1, const glMeshSelectWidget::vertex& p2, const glMeshSelectWidget::vertex& p3);
    bool PointInTriangle(const glMeshSelectWidget::vertex& pt, const glMeshSelectWidget::vertex& v1, const glMeshSelectWidget::vertex& v2, const glMeshSelectWidget::vertex& v3);

    void Reposition();

    QVector<glMeshSelectWidget::vertex> m_qVertices;
    QVector<glMeshSelectWidget::edge> m_qEdges;
    QVector<glMeshSelectWidget::triangle> m_qTriangles;

    QVector<glMeshSelectWidget::triangle> m_constriants;
};

#endif // GLFINALWIDGET_H
