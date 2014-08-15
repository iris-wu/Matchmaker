#include "glprogresswidget.h"
#include "QMouseEvent"
#include "qimage.h"
#include "qbuffer.h"
#include "qbytearray.h"
#include "mainwindow.h"

glProgressWidget::glProgressWidget()
{

}

glProgressWidget::~glProgressWidget()
{
    //nothing owned

}

void glProgressWidget::initializeGL()
{
    //setup OpenGl
    //glEnable(GL_TEXTURE_2D); //enable textures
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void glProgressWidget::paintGL()
{
    //glClear(GL_COLOR_BUFFER_BIT);
}

void glProgressWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glProgressWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    //redraw glWidget
    updateGL();
}

void glProgressWidget::addConstraintMatchAddVertexInMesh(MathAlgorithms::Vertex aVertex)
{
    glProgressWidget::ConstraintMatch newMatch;
    newMatch.vertexInMesh = aVertex;
    ConstraintMatches.append(newMatch);

    //change flags for widgets
    MainWindow::globalInstance->glMeshWidget->SetEnableConstraint(false);
    MainWindow::globalInstance->glTextureWidget->SetEnableConstraint(true);
    MainWindow::globalInstance->matchAction->setEnabled(false); //since we still need another vertex for texture
}

void glProgressWidget::addConstraintMatchAddVertexInTexture(MathAlgorithms::Vertex aVertex)
{
    ConstraintMatches[ConstraintMatches.size() - 1].vertexInTexture = aVertex;

    //change flags for widgets
    MainWindow::globalInstance->glMeshWidget->SetEnableConstraint(true);
    MainWindow::globalInstance->glTextureWidget->SetEnableConstraint(false);
    MainWindow::globalInstance->matchAction->setEnabled(true); //since we have a full set of pairs now
}

void glProgressWidget::performMatch()
{
    //need to add border constraints to list
    QVector<MathAlgorithms::Vertex> textureBorder = MainWindow::globalInstance->glTextureWidget->createBorderConstraints();
    QVector<MathAlgorithms::Vertex> meshBorder = MainWindow::globalInstance->glMeshWidget->createBorderConstraints();

    for(int i = 0; i < 12 ; i++)
    {
        glProgressWidget::ConstraintMatch newMatch;
        newMatch.vertexInMesh = meshBorder[i];
        newMatch.vertexInTexture= textureBorder[i];
        ConstraintMatches.append(newMatch);
    }

    //need to get edges, vertices and triangles from Mesh
    meshVertices = MainWindow::globalInstance->glMeshWidget->GetVertices();
    meshEdges = MainWindow::globalInstance->glMeshWidget->GetEdges();
    meshTriangles = MainWindow::globalInstance->glMeshWidget->GetTriangles();

    //need to get edges, vertices and triangles from Mesh
    QVector<glMeshSelectWidget::vertex*>& textureVertices = MainWindow::globalInstance->glTextureWidget->GetVertices();
    QVector<glMeshSelectWidget::edge*>& textureEdges = MainWindow::globalInstance->glTextureWidget->GetEdges();
    QVector<glMeshSelectWidget::triangle*>& textureTriangles = MainWindow::globalInstance->glTextureWidget->GetTriangles();

    for(int i = 0; i < textureTriangles.size(); i++)
    {
        glMeshSelectWidget::vertex* meshVerticesA = 0;
        glMeshSelectWidget::vertex* meshVerticesB = 0;
        glMeshSelectWidget::vertex* meshVerticesC = 0;

        //get vertexA in mesh that corresponds to constraint in texture
        for(int ii = 0; ii < ConstraintMatches.size(); ii++)
        {
            //find what texture vertex matches the correct constrant Vertex
            if(textureTriangles[i]->vertexA->x == ConstraintMatches[ii].vertexInTexture.x
            && textureTriangles[i]->vertexA->y == ConstraintMatches[ii].vertexInTexture.y
            && textureTriangles[i]->vertexA->z == ConstraintMatches[ii].vertexInTexture.z)
            {
                for(int iii = 0; iii < meshVertices->size(); iii++)
                {
                    //find what constraint mesh vertex matches the meshVertices
                    if(ConstraintMatches[ii].vertexInMesh.x == meshVertices->value(iii)->x
                    && ConstraintMatches[ii].vertexInMesh.y == meshVertices->value(iii)->y
                    && ConstraintMatches[ii].vertexInMesh.z == meshVertices->value(iii)->z)
                    {
                        meshVerticesA = meshVertices->value(iii);
                        break;
                    }
                }
                    break;
            }
        }
        //get vertexB in mesh that corresponds to constraint in texture
        for(int ii = 0; ii < ConstraintMatches.size(); ii++)
        {
            //find what texture vertex matches the correct constrant Vertex
            if(textureTriangles[i]->vertexB->x == ConstraintMatches[ii].vertexInTexture.x
            && textureTriangles[i]->vertexB->y == ConstraintMatches[ii].vertexInTexture.y
            && textureTriangles[i]->vertexB->z == ConstraintMatches[ii].vertexInTexture.z)
            {
                for(int iii = 0; iii < meshVertices->size(); iii++)
                {
                    //find what constraint mesh vertex matches the meshVertices
                    if(ConstraintMatches[ii].vertexInMesh.x == meshVertices->value(iii)->x
                    && ConstraintMatches[ii].vertexInMesh.y == meshVertices->value(iii)->y
                    && ConstraintMatches[ii].vertexInMesh.z == meshVertices->value(iii)->z)
                    {
                        meshVerticesB = meshVertices->value(iii);
                        break;
                    }
                }
                    break;
            }
        }
        //get vertexC in mesh that corresponds to constraint in texture
        for(int ii = 0; ii < ConstraintMatches.size(); ii++)
        {
            //find what texture vertex matches the correct constrant Vertex
            if(textureTriangles[i]->vertexC->x == ConstraintMatches[ii].vertexInTexture.x
            && textureTriangles[i]->vertexC->y == ConstraintMatches[ii].vertexInTexture.y
            && textureTriangles[i]->vertexC->z == ConstraintMatches[ii].vertexInTexture.z)
            {
                for(int iii = 0; iii < meshVertices->size(); iii++)
                {
                    //find what constraint mesh vertex matches the meshVertices
                    if(ConstraintMatches[ii].vertexInMesh.x == meshVertices->value(iii)->x
                    && ConstraintMatches[ii].vertexInMesh.y == meshVertices->value(iii)->y
                    && ConstraintMatches[ii].vertexInMesh.z == meshVertices->value(iii)->z)
                    {
                        meshVerticesC = meshVertices->value(iii);
                        break;
                    }
                }
                    break;
            }
        }

        //now we know the vertex, we need to find shortest path between them in the mesh
        //A --> B
        edgeWalker* walkerAToB = new edgeWalker;
        walkerAToB->startVertex = meshVerticesA;
        walkerAToB->targetVertex = meshVerticesB;
        walkToVertex(walkerAToB);//find closest path
        //B --> C
        edgeWalker* walkerBToC = new edgeWalker;
        walkerBToC->startVertex = meshVerticesB;
        walkerBToC->targetVertex = meshVerticesC;
        walkToVertex(walkerBToC);//find closest path
        //A --> C
        edgeWalker* walkerAToC = new edgeWalker;
        walkerAToC->startVertex = meshVerticesA;
        walkerAToC->targetVertex = meshVerticesC;
        walkToVertex(walkerAToC);//find closest path
        break;
    }
}

void glProgressWidget::walkToVertex(edgeWalker* walker)
{
    QVector<int>* distances = new QVector<int>;

    for(int i = 0; i < walker->startVertex->edgeIndicies.size(); i++)
    {
         glMeshSelectWidget::edge* currentEdge = meshEdges->value(walker->startVertex->edgeIndicies[i]);
         if(currentEdge->vertexA == walker->startVertex)
         {
             //vertex B is destination?
             if(currentEdge->vertexB == walker->targetVertex)
             {
                 //found destination, add edge and terminate
                 walker->edgesIWalked.append(currentEdge);
                 return;
             }
             else
             {
                 MathAlgorithms::Vertex distance;
                 distance.x = walker->targetVertex->x - currentEdge->vertexB->x;
                 distance.y = walker->targetVertex->y - currentEdge->vertexB->y;
                 int dist = sqrt((distance.x * distance.x) +  (distance.y * distance.y));

                 distances->append(dist);
             }
         }
         else
         {
             //vertex A is destination?
             if(currentEdge->vertexA == walker->targetVertex)
             {
                 //found destination, add edge and terminate
                 walker->edgesIWalked.append(currentEdge);
                 return;
             }
             else
             {
                 MathAlgorithms::Vertex distance;
                 distance.x = walker->targetVertex->x - currentEdge->vertexA->x;
                 distance.y = walker->targetVertex->y - currentEdge->vertexA->y;
                 int dist = sqrt((distance.x * distance.x) +  (distance.y * distance.y));

                 distances->append(dist);
             }
         }
    }

    int shortestDisance = -1;
    int index = -1;
    for(int i = 0; i < distances->size(); i++)
    {
        if(distances->value(i) < shortestDisance || shortestDisance == -1)
        {
            shortestDisance = distances->value(i);
            index = i;
        }
    }

    glMeshSelectWidget::edge* bestEdge = meshEdges->value(walker->startVertex->edgeIndicies[index]);
    walker->edgesIWalked.append(bestEdge);
    if(bestEdge->vertexA == walker->targetVertex)
    {
        walker->startVertex = bestEdge->vertexB;
        walkToVertex(walker);
    }
    else
    {
        walker->startVertex = bestEdge->vertexA;
        walkToVertex(walker);
    }
}
