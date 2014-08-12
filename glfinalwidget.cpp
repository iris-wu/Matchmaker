#include "glfinalwidget.h"
#include "QMouseEvent"
#include "qimage.h"
#include "qbuffer.h"
#include "qbytearray.h"

glFinalWidget::glFinalWidget()
{

}

glFinalWidget::~glFinalWidget()
{
    //nothing owned

}

void glFinalWidget::initializeGL()
{
    //setup OpenGl
    //glEnable(GL_TEXTURE_2D); //enable textures
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void glFinalWidget::paintGL()
{
    //glClear(GL_COLOR_BUFFER_BIT);
}

void glFinalWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glFinalWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    //redraw glWidget
    updateGL();
}
