#include "glmeshselectwidget.h"

#include "mathalgorithms.h"

#include <cmath>
#include <QFileDialog>
#include <QMouseEvent>

glMeshSelectWidget::glMeshSelectWidget()
    : m_meshLoaded( false )
{
}

glMeshSelectWidget::~glMeshSelectWidget()
{
    //nothing owned
}

void glMeshSelectWidget::initializeGL()
{
    //setup OpenGl
    glEnableClientState(GL_VERTEX_ARRAY);
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_NORMALIZE );
    glEnable(GL_DEPTH_TEST);
}

void glMeshSelectWidget::paintGL()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //glShadeModel( GL_FLAT );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Draw meshes
    glColor3f(0.0f, 1.0f, 0.0f);
    DrawObject();

    // Draw border points
    if ( m_meshLoaded )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glColor3f(0.5f, 0.0f, 0.5f);

        for(int i = 0; i < m_borderPoints.size(); i++)
        {
            glVertexPointer(3, GL_FLOAT, 0,  &m_borderPoints[i].rightBottom.x);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        for(int i = 0; i < m_userConstraints.size(); i++)
        {
            glVertexPointer(3, GL_FLOAT, 0,  &m_userConstraints[i].rightBottom.x);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
}

void glMeshSelectWidget::resizeGL(int width, int height)
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_widgetWidth = width;
    m_widgetHeight = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if ( m_meshLoaded )
    {
        glOrtho(-75, 75, 0, 150, -90, 160);
    }
    else
    {
        glOrtho(-125, 125, -50, 200, -90, 160);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    updateGL();
}

void glMeshSelectWidget::mousePressEvent(QMouseEvent *event)
{
    if( m_meshLoaded )
    {
        glMeshSelectWidget::constraintPoint point = CreateContraintPoint(event->x(), event->y());
        m_userConstraints.push_back(point);
    }

    //redraw glWidget
    updateGL();
}

void glMeshSelectWidget::loadMeshFileCallback(QTextStream* fileStream)
{
    m_vertices.clear();
    m_faces.clear();

    QString v = "v";
    QString f = "f";
    QString vt = "vt";

    while(!fileStream->atEnd())
    {
        QString currentLine = fileStream->readLine();
        if (currentLine.size() < 2)
        {
            continue;
        }

        QStringList values = currentLine.split(" ");
        std::vector<GLfloat> vertex(3);
        std::vector<unsigned int> face(3);
        std::vector<GLfloat> vTexture(3);
        std::vector<unsigned int> fTexture(3);

        QString type = values[0];
        if ( type.compare(v) == 0 )
        {
            for ( unsigned int i = 0; i < 3; ++i )
            {
                vertex[i] = values[i + 2].toFloat();
            }
            m_vertices.push_back( vertex );
        }
        else if ( type.compare(f) == 0 )
        {
            for ( unsigned int i = 0; i < 3; ++i )
            {
                face[i] = (values[i + 1].split("/"))[0].toUInt();
                fTexture[i] = (values[i + 1].split("/"))[1].toUInt();
            }
            m_faces.push_back( face );
            m_fTexture.push_back( fTexture );
        }
        else if ( type.compare(vt) == 0 )
        {
            vTexture[0] = values[1].toFloat()*250-125;
            vTexture[1] = values[2].toFloat()*250-50;
            vTexture[2] = values[3].toFloat();
            m_vTexture.push_back( vTexture );
        }
    }

    m_originVertices = m_vertices;
    m_originFaces = m_faces;

    //FindEdges();

    updateGL();
}

void glMeshSelectWidget::DrawObject()
{
    // Draw object
    glBegin( GL_TRIANGLES );
    for ( unsigned int i = 0; i < m_faces.size(); ++i )
    {
      //unsigned int faceIndex = i;
        for ( unsigned int j = 0; j < m_faces[i].size(); ++j )
        {
            unsigned int vertexIndex = m_faces[i][j];
            //std::vector<GLfloat> n = m_vNormals[vertexIndex];
            //Normalize( n );
            //glNormal3f( n[0], n[1], n[2] );

            std::vector<GLfloat> currentVertex = m_vertices[ vertexIndex - 1 ];
            glVertex3f( currentVertex[0], currentVertex[1], currentVertex[2] );
        }
    }
    glEnd();
}

void glMeshSelectWidget::CreateBorder()
{
    //Setup border constraints
    //Left side
    m_borderPoints.push_back(CreateContraintPoint(0, 0));
    m_borderPoints.push_back(CreateContraintPoint(0, m_widgetHeight / 3));
    m_borderPoints.push_back(CreateContraintPoint(0, (2 * m_widgetHeight) / 3));
    m_borderPoints.push_back(CreateContraintPoint(0, m_widgetHeight));
    //Top side
    m_borderPoints.push_back(CreateContraintPoint(m_widgetWidth / 3, 0));
    m_borderPoints.push_back(CreateContraintPoint((2 * m_widgetWidth) / 3, 0));
    m_borderPoints.push_back(CreateContraintPoint(m_widgetWidth, 0));
    //Right side
    m_borderPoints.push_back(CreateContraintPoint(m_widgetWidth, m_widgetHeight / 3));
    m_borderPoints.push_back(CreateContraintPoint(m_widgetWidth, (2 * m_widgetHeight) / 3));
    m_borderPoints.push_back(CreateContraintPoint(m_widgetWidth, m_widgetHeight));
    //Bottom side
    m_borderPoints.push_back(CreateContraintPoint(m_widgetWidth / 3, m_widgetHeight));
    m_borderPoints.push_back(CreateContraintPoint((2 * m_widgetWidth) / 3, m_widgetHeight));
}

glMeshSelectWidget::constraintPoint glMeshSelectWidget::CreateContraintPoint(int x, int y)
{
    int glXLocation = ((float)x / m_widgetWidth) * GL_MESHWIDGET_CANVAS_WIDTH + X_OFFSET;
    int glYLocation = ((float)(m_widgetHeight - y) / m_widgetHeight) * GL_MESHWIDGET_CANVAS_HEIGHT + Y_OFFSET;
    //int glXLocation = x;
    //int glYLocation = y;

    std::vector<GLfloat> closestVertex = GetClosestVertex( glXLocation, glYLocation );
    m_constraints.push_back( closestVertex );

    //printf("border: %d %d\n", glXLocation, glYLocation);
    //printf("closest: %f %f\n", closestVertex[0], closestVertex[1]);

    glXLocation = (int)closestVertex[0];
    glYLocation = (int)closestVertex[1];

    //X and Y location is always center of the constraint point
    constraintPoint newPoint;
    newPoint.pixelXLocation = glXLocation;
    newPoint.pixelYLocation = glYLocation;
    //Left Bottom
    newPoint.leftBottom.x = glXLocation - GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.leftBottom.y = glYLocation - GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.leftBottom.z = 0;
    //Left Top
    newPoint.leftTop.x = glXLocation - GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.leftTop.y = glYLocation + GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.leftTop.z = 0;
    //Right Bottom
    newPoint.rightBottom.x = glXLocation + GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.rightBottom.y = glYLocation - GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.rightBottom.z = 0;
    //Right Top
    newPoint.rightTop.x = glXLocation + GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.rightTop.y = glYLocation + GL_MESHWIDGET_CONSTRAINT_SIZE;
    newPoint.rightTop.z = 0;

    return newPoint;
}

void glMeshSelectWidget::FindEdges()
{
    m_edges.clear();

    for ( unsigned int fIndex = 0; fIndex < m_faces.size(); ++fIndex )
    {
        std::vector<unsigned int> currentFace = m_faces[fIndex];
        for ( unsigned int i = 0; i < 3; ++i )
        {
            unsigned int currentVertex = currentFace[i] - 1;
            unsigned int neighborVertex = currentFace[(i+1)%3] - 1;

            if ( currentVertex < neighborVertex )
            {
                m_edges.insert( std::make_pair( currentVertex, neighborVertex ) );
            }
        }
    }

    // debug
    printf("e: %d\n", (int)m_edges.size() );
}

void glMeshSelectWidget::parameterizeMesh()
{
    m_meshLoaded = true;

    // init
    m_actualVTexture.clear();
    m_actualFaces = m_originFaces;
    m_actualFTexture = m_fTexture;

    RemoveFacesOutsideBoundary();
    CreateBorder();

    // projection (2D) points
    m_vertices = m_vTexture;
    m_faces = m_actualFTexture;

    // 3D points
    //m_vertices = m_originVertices;
    //m_faces = m_actualFaces;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-125, 125, -50, 200, -90, 160);
    glOrtho(-75, 75, 0, 150, -90, 160);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    updateGL();
}

std::vector<GLfloat> glMeshSelectWidget::GetClosestVertex( GLfloat x, GLfloat y )
{
    std::vector<GLfloat> closestVertex;
    GLfloat minDistance = 150;

    for( unsigned int i = 0; i < m_actualVTexture.size(); ++i )
    {
        std::vector<GLfloat> currentVertex = m_actualVTexture[i];
        GLfloat distance = sqrt( pow( currentVertex[0] - x, 2 ) + pow( currentVertex[1] - y ,2 ) );
        if ( distance < minDistance )
        {
            minDistance = distance;
            closestVertex = currentVertex;
        }
    }
    return closestVertex;
}

void glMeshSelectWidget::RemoveFacesOutsideBoundary()
{
    for ( unsigned int i = 0; i < m_vTexture.size(); ++i )
    {
        std::vector<GLfloat> currentVertex = m_vTexture[i];
        // if a vertex is outside boundary
        if ( (currentVertex[0] < -75 || currentVertex[0] > 75) || (currentVertex[1] < 0 || currentVertex[1] > 150) )
        {
            // find faces having this vertex
            for ( int j = 0; j < m_actualFTexture.size(); ++j )
            {
                std::vector<unsigned int> currentFTexture = m_actualFTexture[j];
                if ( currentFTexture[0] - 1 == i || currentFTexture[1] - 1 == i || currentFTexture[2] - 1 == i )
                {
                    // remove the face
                    m_actualFTexture.erase( m_actualFTexture.begin() + j );
                    m_actualFaces.erase( m_actualFaces.begin() + j );
                    j--;
                }
            }
        }
        else
        {
            m_actualVTexture.push_back( currentVertex );
        }
    }
}
