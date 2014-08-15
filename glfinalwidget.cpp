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
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glColor3f(0.5f, 0.0f, 0.5f);

    glBegin( GL_TRIANGLES );
    for ( int i = 0; i < m_qTriangles.size(); ++i )
    {
        glMeshSelectWidget::triangle t = m_qTriangles[i];
        glMeshSelectWidget::vertex a, b, c;
        a = *(t.vertexA);
        b = *(t.vertexB);
        c = *(t.vertexC);

        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
    }
    glEnd();
}

void glFinalWidget::resizeGL(int width, int height)
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-75, 75, 0, 150, -90, 160);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glFinalWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    //redraw glWidget
    updateGL();
}

GLfloat glFinalWidget::Sign(const glMeshSelectWidget::vertex& p1, const glMeshSelectWidget::vertex& p2, const glMeshSelectWidget::vertex& p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool glFinalWidget::PointInTriangle(const glMeshSelectWidget::vertex& pt, const glMeshSelectWidget::vertex& v1, const glMeshSelectWidget::vertex& v2, const glMeshSelectWidget::vertex& v3)
{
  bool b1, b2, b3;

  b1 = Sign(pt, v1, v2) < 0.0f;
  b2 = Sign(pt, v2, v3) < 0.0f;
  b3 = Sign(pt, v3, v1) < 0.0f;

  return ((b1 == b2) && (b2 == b3));
}

void glFinalWidget::Reposition()
{
    for (int vIndex = 0; vIndex < m_qVertices.size(); ++vIndex)
    {
        glMeshSelectWidget::vertex currentVertex = m_qVertices[vIndex];

        bool pointInPatch = false;
        for (int cIndex = 0; cIndex < m_constriants.size(); ++cIndex)
        {
            glMeshSelectWidget::triangle currentPatch = m_constriants[cIndex];
            pointInPatch = PointInTriangle(currentVertex, *(currentPatch.vertexA), *(currentPatch.vertexB), *(currentPatch.vertexC));

            // if patch is found
            if (pointInPatch)
            {
                // Find all neighbors
                QVector<glMeshSelectWidget::vertex*> neighbors;
                for (int eIndex = 0; eIndex < currentVertex.edgeIndicies.size(); ++eIndex)
                {
                    if (m_qEdges[eIndex].vertexA == &m_qVertices[vIndex])
                    {
                        neighbors.push_back(m_qEdges[eIndex].vertexB);
                    }
                    else
                    {
                        neighbors.push_back(m_qEdges[eIndex].vertexA);
                    }
                }

                // re-compute the position
                glMeshSelectWidget::vertex sum;
                sum.x = 0.0;
                sum.y = 0.0;
                sum.z = 0.0;

                for (int nIndex = 0; nIndex < neighbors.size(); ++nIndex)
                {
                    sum.x += neighbors[nIndex]->x;
                    sum.y += neighbors[nIndex]->y;
                    sum.z += neighbors[nIndex]->z;
                }

                int num = neighbors.size();
                if (num > 0)
                {
                    sum.x = sum.x/(GLfloat)num;
                    sum.y = sum.y/(GLfloat)num;
                    sum.z = sum.z/(GLfloat)num;

                    m_qVertices[vIndex].x = sum.x;
                    m_qVertices[vIndex].y = sum.y;
                    m_qVertices[vIndex].z = sum.z;
                }
                break;
            }
        }
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-75, 75, 0, 150, -90, 160);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    updateGL();
}
