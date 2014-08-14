#include "glprogresswidget.h"
#include "QMouseEvent"
#include "qimage.h"
#include "qbuffer.h"
#include "qbytearray.h"
#include "mainwindow.h"

glProgressWidget::glProgressWidget()
{

}

glProgressWidget::~glProgressWidget()
{
    //nothing owned

}

void glProgressWidget::initializeGL()
{
    //setup OpenGl
    //glEnable(GL_TEXTURE_2D); //enable textures
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void glProgressWidget::paintGL()
{
    //glClear(GL_COLOR_BUFFER_BIT);
}

void glProgressWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glProgressWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    //redraw glWidget
    updateGL();
}

void glProgressWidget::addConstraintMatchAddVertexInMesh(MathAlgorithms::Vertex aVertex)
{
    glProgressWidget::ConstraintMatch newMatch;
    newMatch.vertexInMesh = aVertex;
    ConstraintMatches.append(newMatch);

    //change flags for widgets
    MainWindow::globalInstance->glMeshWidget->SetEnableConstraint(false);
    MainWindow::globalInstance->glTextureWidget->SetEnableConstraint(true);
    MainWindow::globalInstance->matchAction->setEnabled(false); //since we still need another vertex for texture
}

void glProgressWidget::addConstraintMatchAddVertexInTexture(MathAlgorithms::Vertex aVertex)
{
    ConstraintMatches[ConstraintMatches.size() - 1].vertexInTexture = aVertex;

    //change flags for widgets
    MainWindow::globalInstance->glMeshWidget->SetEnableConstraint(true);
    MainWindow::globalInstance->glTextureWidget->SetEnableConstraint(false);
    MainWindow::globalInstance->matchAction->setEnabled(true); //since we have a full set of pairs now
}
