#ifndef GLMESHSELECTWIDGET_H
#define GLMESHSELECTWIDGET_H

#include <QGLWidget>
#include <QTextStream>
#include <QVector>

#include <map>
#include <set>
#include <vector>

#define GL_MESHWIDGET_CANVAS_WIDTH 150
#define GL_MESHWIDGET_CANVAS_HEIGHT 150
#define X_OFFSET -75
#define Y_OFFSET 0
#define GL_MESHWIDGET_CONSTRAINT_SIZE 1

class glMeshSelectWidget: public QGLWidget

{
    struct vertex
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };

    struct edge
    {
        vertex* vertexA;
        vertex* vertexB;
    };

    struct triangle
    {
        vertex* vertexA;
        vertex* vertexB;
        vertex* vertexC;
        edge* edgeA;
        edge* edgeB;
        edge* edgeC;
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

    // called when the user wants to parameterize mesh
    void parameterizeMesh();
    void SetEnableConstraint(bool aValue);

    const QVector<vertex>& GetVertices() const;
    const QVector<edge>& GetEdges() const;
    const QVector<triangle>& GetTriangles() const;

protected:
    void initializeGL(); //called once before drawing happens
    void paintGL(); //called during draw call back
    void resizeGL(int width, int height); //called when widget size changes
    void mousePressEvent(QMouseEvent *event); //when mouse is pressed inside the area

private:

    void DrawObject();
    void AddEdgesAndTriangles();
    int FindEdgeIndex(const edge& e);

    void RemoveFacesOutsideBoundary( std::set<unsigned int>& edgePoints );
    void AddVirtualBoundary( const std::set<unsigned int>& edgePoints );
    void MakeNewStructure();

    void CreateBorder();
    constraintPoint CreateContraintPoint(int x, int y);
    std::vector<GLfloat> GetClosestVertex( GLfloat x, GLfloat y );

    QVector<vertex> m_qVertices;
    QVector<edge> m_qEdges;
    QVector<triangle> m_qTriangles;

    std::vector< std::vector< GLfloat > > m_vertices;
    std::vector< std::vector< unsigned int > > m_faces;

    // origin mesh vertices and faces which are loaded from .obj file
    std::vector< std::vector< GLfloat > > m_originVertices;
    std::vector< std::vector< unsigned int > > m_originFaces;

    // origin texture vertices and faces
    std::vector< std::vector< GLfloat > > m_vTexture;
    std::vector< std::vector< unsigned int > > m_fTexture;

    // actual vertices and faces after cutting and adding boundary
    std::vector< std::vector< unsigned int > > m_actualFaces;   // orgin faces - faces cut
    std::vector< std::vector< GLfloat > > m_actualVTexture;     // orgin texture vertices + additional boundary points
    std::vector< std::vector< unsigned int > > m_actualFTexture;    // origin texture faces - texture faces cut

    //std::set< std::pair<unsigned int, unsigned int> > m_edges;

    int m_widgetWidth;
    int m_widgetHeight;

    QVector<constraintPoint> m_borderPoints;
    QVector<constraintPoint> m_userConstraints;
    std::vector< std::vector< GLfloat > > m_constraints;

    bool m_meshLoaded;
    bool enableSetConstraint;
};

#endif // GLMESHSELECTWIDGET_H
