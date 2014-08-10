#ifndef GLMESHSELECTWIDGET_H
#define GLMESHSELECTWIDGET_H

#include <QGLWidget>
#include <QTextStream>
#include <QVector>

#include <map>
#include <set>
#include <vector>

#define GL_MESHWIDGET_CANVAS_WIDTH 160
#define GL_MESHWIDGET_CANVAS_HEIGHT 160
#define X_OFFSET -80
#define Y_OFFSET -10
#define GL_MESHWIDGET_CONSTRAINT_SIZE 1

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

    struct constraintPoint
    {
        int pixelXLocation;
        int pixelYLocation;
        vertex rightBottom;
        vertex rightTop;
        vertex leftBottom;
        vertex leftTop;
    };

public:
    glMeshSelectWidget();
    ~glMeshSelectWidget();

    //is called when a the user wants to load a different mesh file
    void loadMeshFileCallback(QTextStream* fileStream);

    void parameterizeMesh();

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
    void FindEdges();

    void CreateBorder();
    constraintPoint CreateContraintPoint(int x, int y);

    std::vector< std::vector< GLfloat > > m_vertices;
    std::vector< std::vector< unsigned int > > m_faces;

    std::vector< std::vector< GLfloat > > m_originVertices;
    std::vector< std::vector< unsigned int > > m_originFaces;

    std::vector< std::vector< GLfloat > > m_vTexture;
    std::vector< std::vector< unsigned int > > m_fTexture;

    std::set< std::pair<unsigned int, unsigned int> > m_edges;

    std::map< unsigned int, std::vector<GLfloat> > m_vNormals;
    std::map< unsigned int, std::vector<GLfloat> > m_fNormals;

    int m_widgetWidth;
    int m_widgetHeight;
    QVector<constraintPoint> m_borderPoints;
    QVector<constraintPoint> m_userConstraints;

    bool m_meshLoaded;
};

#endif // GLMESHSELECTWIDGET_H
