#ifndef GLMESHSELECTWIDGET_H
#define GLMESHSELECTWIDGET_H

#include <QGLWidget>
#include <QTextStream>

#include <map>
#include <vector>

class glMeshSelectWidget: public QGLWidget

{
    struct vertex
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };

    struct triangle
    {
        vertex vertexA;
        vertex vertexB;
        vertex vertexC;
    };

public:
    glMeshSelectWidget();
    ~glMeshSelectWidget();

    //is called when a the user wants to load a different mesh file
    void loadMeshFileCallback(QTextStream* fileStream);

protected:
    void initializeGL(); //called once before drawing happens
    void paintGL(); //called during draw call back
    void resizeGL(int width, int height); //called when widget size changes
    void mousePressEvent(QMouseEvent *event); //when mouse is pressed inside the area

    triangle testShape;

private:

    void FindNormals( const std::vector<unsigned int>& face );
    void Normalize( std::vector<GLfloat>& n );
    void DrawObject();

    std::vector< std::vector< GLfloat > > vertices;
    std::vector< std::vector< unsigned int > > faces;
    std::map< unsigned int, std::vector<GLfloat> > vNormals;
    std::map< unsigned int, std::vector<GLfloat> > fNormals;
};

#endif // GLMESHSELECTWIDGET_H
