#include "glmeshselectwidget.h"

#include <cmath>

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
    //glEnableClientState(GL_VERTEX_ARRAY);
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_NORMALIZE );
    glEnable(GL_DEPTH_TEST);

    //swapBuffers();
}

void glMeshSelectWidget::paintGL()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /*glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();*/

    //glShadeModel( GL_FLAT );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glColor3f(0.0f, 1.0f, 0.0f);
    DrawObject();

    //use gree to draw lines
    /*glColor3f(0,1,0);

    glVertexPointer(3, GL_FLOAT, 0, &testShape.vertexA.x);
    glDrawArrays(GL_LINE_LOOP, 0, 3);*/
}

void glMeshSelectWidget::resizeGL(int width, int height)
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 200, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    swapBuffers();
}

void glMeshSelectWidget::mousePressEvent(QMouseEvent *event)
{

}

void glMeshSelectWidget::loadMeshFileCallback(QTextStream* fileStream)
{
    vertices.clear();
    faces.clear();
    vNormals.clear();
    fNormals.clear();

    QString v = "v";
    QString f = "f";

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

        QString type = values[0];
        if ( type.compare(v) == 0 )
        {
            for ( unsigned int i = 0; i < 3; ++i )
            {
                vertex[i] = values[i + 2].toFloat();
            }
            vertices.push_back( vertex );
        }
        else if ( type.compare(f) == 0 )
        {
            for ( unsigned int i = 0; i < 3; ++i )
            {
                face[i] = (values[i + 1].split("/"))[0].toUInt();
            }
            faces.push_back( face );

            FindNormals( face );
        }


        //do your magic :)
    }

    updateGL();
    printf("v: %d, f: %d\n", (int)vertices.size(), (int)faces.size());
}

void glMeshSelectWidget::FindNormals(const std::vector<unsigned int> &face)
{
    std::vector< unsigned int > vertex(3);
    for ( unsigned int i = 0; i < 3; ++i )
    {
        vertex[i] = face[i] - 1;
    }

    std::vector<GLfloat> vector_01(3), vector_12(3), n(3);
    for ( unsigned int i = 0; i < 3; ++i )
    {
        vector_01[i] = vertices[vertex[1]][i] - vertices[vertex[0]][i];
        vector_12[i] = vertices[vertex[2]][i] - vertices[vertex[1]][i];
    }
    n[0] = vector_01[1] * vector_12[2] - vector_01[2] * vector_12[1];
    n[1] = vector_12[0] * vector_01[2] - vector_12[2] * vector_01[0];
    n[2] = vector_01[0] * vector_12[1] - vector_01[1] * vector_12[0];

    //Normalize( n );

    unsigned int index = faces.size() - 1;
    fNormals[index] = n;

    for ( unsigned int i = 0; i < 3; ++i )
    {
        unsigned int currentVertexIndex = face[i];
        if( vNormals.find( currentVertexIndex ) == vNormals.end() )
        {
            vNormals[ currentVertexIndex ] = n;
        }
        else
        {
            for ( unsigned int j = 0; j < 3; ++j )
            {
                vNormals[ currentVertexIndex ][j] += n[j];
            }
        }
    }
}

void glMeshSelectWidget::Normalize( std::vector<GLfloat>& n )
{
    GLfloat l = std::sqrt( n[0] * n[0] + n[1] * n[1] + n[2] * n[2] );
    n[0] /= l;
    n[1] /= l;
    n[2] /= l;
}

void glMeshSelectWidget::DrawObject()
{
    // Draw object
    glBegin( GL_TRIANGLES );
    for ( unsigned int i = 0; i < faces.size(); ++i )
    {
      unsigned int faceIndex = i;
        for ( unsigned int j = 0; j < faces[i].size(); ++j )
        {
            unsigned int vertexIndex = faces[i][j];
            std::vector<GLfloat> n = vNormals[vertexIndex];
            Normalize( n );
            glNormal3f( n[0], n[1], n[2] );

            std::vector<GLfloat> currentVertex = vertices[ vertexIndex - 1 ];
            glVertex3f( currentVertex[0], currentVertex[1], currentVertex[2] );
        }
    }
    glEnd();
}
