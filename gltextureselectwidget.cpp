#include "glTextureSelectWidget.h"
#include "QMouseEvent"
#include "qimage.h"
#include "qbuffer.h"
#include "qbytearray.h"

glTextureSelectWidget::glTextureSelectWidget()
{
    textureOfFace.vertexACor.x = 100;
    textureOfFace.vertexACor.y = 0;
    textureOfFace.vertexACor.z = 0;
    textureOfFace.vertexATex.s = 1;
    textureOfFace.vertexATex.t = 0;

    textureOfFace.vertexBCor.x = 100;
    textureOfFace.vertexBCor.y = 100;
    textureOfFace.vertexBCor.z = 0;
    textureOfFace.vertexBTex.s = 1;
    textureOfFace.vertexBTex.t = 1;

    textureOfFace.vertexCCor.x = 0;
    textureOfFace.vertexCCor.y = 0;
    textureOfFace.vertexCCor.z = 0;
    textureOfFace.vertexCTex.s = 0;
    textureOfFace.vertexCTex.t = 0;

    textureOfFace.vertexDCor.x = 0;
    textureOfFace.vertexDCor.y = 100;
    textureOfFace.vertexDCor.z = 0;
    textureOfFace.vertexDTex.s = 0;
    textureOfFace.vertexDTex.t = 1;

    textureLoaded = false;
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
    delete default_image;
}

void glTextureSelectWidget::paintGL()
{
    //use gree to draw lines
    glColor3f(0,1,0);

    glVertexPointer(3, GL_FLOAT, 0, &textureOfFace.vertexACor.x);
    glTexCoordPointer(2, GL_FLOAT, 0,  &textureOfFace.vertexATex.s);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if(textureLoaded)
    {
        //draw constrain points, don't want to use texture in this case
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.5, 0,0.5);

        //draw border constraints
        for(int i = 0; i < 12; i++)
        {
            glVertexPointer(3, GL_FLOAT, 0,  &borderConstraints[i].rightBottom.x);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        //draw user constraints
        for(int i = 0; i < userConstraints.size(); i++)
        {
            glVertexPointer(3, GL_FLOAT, 0,  &userConstraints[i].rightBottom.x);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        //all done, enable textures again
        glEnable(GL_TEXTURE_2D);
    }
}

void glTextureSelectWidget::resizeGL(int width, int height)
{
    widgetWidth = width;
    widgetHeight = height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, GL_TEXTUREWIDGET_CANVAS_WIDTH, 0, GL_TEXTUREWIDGET_CANVAS_HEIGHT, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glTextureSelectWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    if(textureLoaded)
    {
         glTextureSelectWidget::constraintPoint point = createContraintPoint(mouseEvent->x(), mouseEvent->y());
         userConstraints.push_back(point);
    }

    //redraw glWidget
    updateGL();
}

void glTextureSelectWidget::loadTextureFromFile(QString& fileName)
{
    //load image data from file
    QImage *image = new QImage(fileName);
    if(image->isNull())
    {
        image = NULL; //something went wrong with loading the file from the resources
    }
    QImage imageInOpenGLFormat = QGLWidget::convertToGLFormat(*image);

    //replace current displayed texture
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageInOpenGLFormat.width(), imageInOpenGLFormat.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageInOpenGLFormat.bits());

    //should now be in openGL memory, don't need image file anymore
    delete image;
    textureLoaded = true;

    //setup border constraints
    //Left side
    borderConstraints[0] = createContraintPoint(0, 0);
    borderConstraints[1] = createContraintPoint(0, widgetHeight / 3);
    borderConstraints[2] = createContraintPoint(0, (2 * widgetHeight) / 3);
    borderConstraints[3] = createContraintPoint(0, widgetHeight);
    //Top side
    borderConstraints[4] = createContraintPoint(widgetWidth / 3, 0);
    borderConstraints[5] = createContraintPoint((2 * widgetWidth) / 3, 0);
    borderConstraints[6] = createContraintPoint(widgetWidth, 0);
    //Right side
    borderConstraints[7] = createContraintPoint(widgetWidth, widgetHeight / 3);
    borderConstraints[8] = createContraintPoint(widgetWidth, (2 * widgetHeight) / 3);
    borderConstraints[9] = createContraintPoint(widgetWidth, widgetHeight);
    //Bottom side
    borderConstraints[10] = createContraintPoint(widgetWidth / 3, widgetHeight);
    borderConstraints[11] = createContraintPoint((2 * widgetWidth) / 3, widgetHeight);

    //redraw glWidget
    updateGL();
}

glTextureSelectWidget::constraintPoint glTextureSelectWidget::createContraintPoint(int x, int y)
{
    int glXLocation = ((float) x / widgetWidth) * GL_TEXTUREWIDGET_CANVAS_WIDTH;
    int glYLocation = ((float) (widgetHeight - y) / widgetHeight) * GL_TEXTUREWIDGET_CANVAS_HEIGHT;

    //X and Y location is always center of the constraint point
    constraintPoint newPoint;
    newPoint.pixelXLocation = glXLocation;
    newPoint.pixelYLocation = glYLocation;
    //Left Bottom
    newPoint.leftBottom.x = glXLocation - GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.leftBottom.y = glYLocation - GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.leftBottom.z = 0;
    //Left Top
    newPoint.leftTop.x = glXLocation - GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.leftTop.y = glYLocation + GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.leftTop.z = 0;
    //Right Bottom
    newPoint.rightBottom.x = glXLocation + GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.rightBottom.y = glYLocation - GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.rightBottom.z = 0;
    //Right Top
    newPoint.rightTop.x = glXLocation + GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.rightTop.y = glYLocation + GL_TEXTUREWIDGET_CONSTRAINT_SIZE;
    newPoint.rightTop.z = 0;

    return newPoint;
}
