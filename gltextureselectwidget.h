#ifndef GLTEXTURESELECTWIDGET_H
#define GLTEXTURESELECTWIDGET_H

#include <QGLWidget>

//This widget just displays one texture mapped over the entire area of the widget
//The user can then select certain constraint points on this texutre

class glTextureSelectWidget : public QGLWidget
{
    struct vertexCor
    {
        float x;
        float y;
        float z;
    };

    struct vertexTex
    {
        float s;
        float t;
    };

    struct rectangle //since we want to span the entire area we need a rectangle
    {
        vertexCor vertexACor;
        vertexCor vertexBCor;
        vertexCor vertexCCor;
        vertexCor vertexDCor;

        vertexTex vertexATex;
        vertexTex vertexBTex;
        vertexTex vertexCTex;
        vertexTex vertexDTex;
    };

public:
    glTextureSelectWidget();
    ~glTextureSelectWidget();

protected:
    void initializeGL(); //called once before drawing happens
    void paintGL(); //called during draw call back
    void resizeGL(int width, int height); //called when widget size changes
    void mousePressEvent(QMouseEvent *event); //when mouse is pressed inside the area

    rectangle textureOfFace;
    GLuint textureId;
};

#endif // GLTEXTURESELECTWIDGET_H
