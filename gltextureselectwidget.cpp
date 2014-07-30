#include "glTextureSelectWidget.h"
#include "qimage.h"
#include "qbuffer.h"
#include "qbytearray.h"

glTextureSelectWidget::glTextureSelectWidget()
{
    textureOfFace.vertexACor.x = 100;
    textureOfFace.vertexACor.y = -100;
    textureOfFace.vertexACor.z = 0;
    textureOfFace.vertexATex.s = 1;
    textureOfFace.vertexATex.t = 0;

    textureOfFace.vertexBCor.x = 100;
    textureOfFace.vertexBCor.y = 100;
    textureOfFace.vertexBCor.z = 0;
    textureOfFace.vertexBTex.s = 1;
    textureOfFace.vertexBTex.t = 1;

    textureOfFace.vertexCCor.x = -100;
    textureOfFace.vertexCCor.y = -100;
    textureOfFace.vertexCCor.z = 0;
    textureOfFace.vertexCTex.s = 0;
    textureOfFace.vertexCTex.t = 0;

    textureOfFace.vertexDCor.x = -100;
    textureOfFace.vertexDCor.y = 100;
    textureOfFace.vertexDCor.z = 0;
    textureOfFace.vertexDTex.s = 0;
    textureOfFace.vertexDTex.t = 1;


}

glTextureSelectWidget::~glTextureSelectWidget()
{
    //nothing owned
}

void glTextureSelectWidget::initializeGL()
{
    //setup OpenGl
    glEnable(GL_TEXTURE_2D); //enable textures
    glEnableClientState(GL_VERTEX_ARRAY); //enable drawArray vertex array
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);//enable drawArray texture array
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //-- Setup texture handling and default texture --//

    //generate TextureIdHandle
    glGenTextures(1, &textureId);

    //load default image
    QImage *default_image = new QImage(":/no_image.png");
    if(default_image->isNull())
    {
        default_image = NULL; //something went wrong with loading the file from the resources
    }
    QImage imageInOpenGLFormat = QGLWidget::convertToGLFormat(*default_image);

    //load image into openGl texture memory
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageInOpenGLFormat.width(), imageInOpenGLFormat.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageInOpenGLFormat.bits());

    //setup texture rendering options
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //should now be in openGL memory, don't need image file anymore
    //delete default_image;
}

void glTextureSelectWidget::paintGL()
{
    //use gree to draw lines
    glColor3f(0,1,0);

    glVertexPointer(3, GL_FLOAT, 0, &textureOfFace.vertexACor.x);
    glTexCoordPointer(2, GL_FLOAT, 0,  &textureOfFace.vertexATex.s);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void glTextureSelectWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glTextureSelectWidget::mousePressEvent(QMouseEvent *event)
{

}
