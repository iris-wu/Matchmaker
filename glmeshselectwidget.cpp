#include "glmeshselectwidget.h"

glMeshSelectWidget::glMeshSelectWidget()
{
    testShape.vertexA.x = -50;
    testShape.vertexA.y = -50;
    testShape.vertexA.z = 0;

    testShape.vertexB.x = 50;
    testShape.vertexB.y = -50;
    testShape.vertexB.z = 0;

    testShape.vertexC.x = 0;
    testShape.vertexC.y = 50;
    testShape.vertexC.z = 0;
}

glMeshSelectWidget::~glMeshSelectWidget()
{
    //nothing owned
}

void glMeshSelectWidget::initializeGL()
{
    //setup OpenGl
    glEnableClientState(GL_VERTEX_ARRAY);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void glMeshSelectWidget::paintGL()
{
    //use gree to draw lines
    glColor3f(0,1,0);

    glVertexPointer(3, GL_FLOAT, 0, &testShape.vertexA.x);
    glDrawArrays(GL_LINE_LOOP, 0, 3);
}

void glMeshSelectWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glMeshSelectWidget::mousePressEvent(QMouseEvent *event)
{

}
