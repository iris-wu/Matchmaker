#include "glprogresswidget.h"
#include "QMouseEvent"
#include "qimage.h"
#include "qbuffer.h"
#include "qbytearray.h"
#include "mainwindow.h"

glProgressWidget::glProgressWidget()
{
    meshVertices = 0;
    meshEdges = 0;
    meshTriangles = 0;
}

glProgressWidget::~glProgressWidget()
{
    //nothing owned

}

void glProgressWidget::initializeGL()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
}

void glProgressWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //glShadeModel( GL_FLAT );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    if(meshTriangles)
    {
        //draw mesh
        glColor3f(0.0f, 1.0f, 0.0f);

        for(int i = 0; i < meshTriangles->size(); i++)
        {
            glMeshSelectWidget::triangle* triangle = meshTriangles->value(i);

            //draw triangle mesh
            glBegin( GL_TRIANGLES );

            //set vertex
            glVertex3f(triangle->vertexA->x, triangle->vertexA->y, triangle->vertexA->z);
            glVertex3f(triangle->vertexB->x, triangle->vertexB->y, triangle->vertexB->z);
            glVertex3f(triangle->vertexC->x, triangle->vertexC->y, triangle->vertexC->z);

            glEnd();
        }

        //draw edges that connect constraints
        glColor3f(0.5, 0, 0.5); //purple

        for(int i = 0; i < constraintEdgesWithPoints.size(); i++)
        {
            edgeWalker* constraintWithEdges = constraintEdgesWithPoints[i];

            //draw edges
            for(int ii = 0; ii < constraintWithEdges->edgesIWalked.size(); ii++)
            {
                glMeshSelectWidget::vertex* vertexA = constraintWithEdges->edgesIWalked[ii]->vertexA;
                glMeshSelectWidget::vertex* vertexB = constraintWithEdges->edgesIWalked[ii]->vertexB;

                //draw line for edge
                glBegin( GL_LINES );

                //set vertex
                glVertex3f(vertexA->x, vertexA->y, vertexA->z);
                glVertex3f(vertexB->x, vertexB->y, vertexB->z);

                glEnd();
            }

            //draw Constraint Points as well
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); //constraint points should be filled

            MathAlgorithms::Vertex bottomLeftA, bottomRightA, topRightA, topLeftA;
            bottomLeftA.x = constraintWithEdges->startVertex->x - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            bottomLeftA.y = constraintWithEdges->startVertex->y - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            bottomRightA.x = constraintWithEdges->startVertex->x + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            bottomRightA.y = constraintWithEdges->startVertex->y - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topRightA.x = constraintWithEdges->startVertex->x + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topRightA.y = constraintWithEdges->startVertex->y + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topLeftA.x = constraintWithEdges->startVertex->x - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topLeftA.y = constraintWithEdges->startVertex->y + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;

            MathAlgorithms::Vertex bottomLeftB, bottomRightB, topRightB, topLeftB;
            bottomLeftB.x = constraintWithEdges->targetVertex->x - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            bottomLeftB.y = constraintWithEdges->targetVertex->y - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            bottomRightB.x = constraintWithEdges->targetVertex->x + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            bottomRightB.y = constraintWithEdges->targetVertex->y - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topRightB.x = constraintWithEdges->targetVertex->x + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topRightB.y = constraintWithEdges->targetVertex->y + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topLeftB.x = constraintWithEdges->targetVertex->x - GL_PROGRESSWIDGET_CONSTRAINT_SIZE;
            topLeftB.y = constraintWithEdges->targetVertex->y + GL_PROGRESSWIDGET_CONSTRAINT_SIZE;

            //start Vertext Constraint
            glBegin( GL_TRIANGLE_STRIP );
            glVertex2f(bottomRightA.x, bottomRightA.y);
            glVertex2f(topRightA.x, topRightA.y);
            glVertex2f(bottomLeftA.x, bottomLeftA.y);
            glVertex2f(topLeftA.x, topLeftA.y);
            glEnd();

            //target Vertext Constraint
            glBegin( GL_TRIANGLE_STRIP );
            glVertex2f(bottomRightB.x, bottomRightB.y);
            glVertex2f(topRightB.x, topRightB.y);
            glVertex2f(bottomLeftB.x, bottomLeftB.y);
            glVertex2f(topLeftB.x, topLeftB.y);
            glEnd();
        }
    }
}

void glProgressWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-75, 75, 0, 150, -90, 160);
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

    //create match for border constraints between texture and mesh view
    for(int i = 0; i < 12 ; i++)
    {
        glProgressWidget::ConstraintMatch newMatch;
        newMatch.vertexInMesh = meshBorder[i];
        newMatch.vertexInTexture= textureBorder[i];
        ConstraintMatches.append(newMatch);
    }

    //reset what edges are part of the constraints
    for(int i = 0; i < constraintEdgesWithPoints.size(); i++)
    {
        constraintEdgesWithPoints[i]->edgesIWalked.clear();
        delete constraintEdgesWithPoints[i]; //clear up memory it points to
    }
    constraintEdgesWithPoints.clear(); //clear up memoy taken by pointers

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

        //add constraint constraint information
        walkerAToB->startVertex = meshVerticesA; //need to reset it, since it changes due to function calls
        constraintEdgesWithPoints.append(walkerAToB);
        walkerBToC->startVertex = meshVerticesB; //need to reset it, since it changes due to function calls
        constraintEdgesWithPoints.append(walkerBToC);
        walkerAToC->startVertex = meshVerticesA; //need to reset it, since it changes due to function calls
        constraintEdgesWithPoints.append(walkerAToC);
        //break;
    }

    //redraw glWidget
    updateGL();
}

void glProgressWidget::walkToVertex(edgeWalker* walker)
{
    QVector<float> distances;

    for(int i = 0; i < walker->startVertex->edgeIndicies.size(); i++)
    {
         glMeshSelectWidget::edge* currentEdge = meshEdges->value(walker->startVertex->edgeIndicies[i]);
         glMeshSelectWidget::vertex* goToVertex = 0;

         if(currentEdge->vertexA == walker->startVertex)
         {
            goToVertex = currentEdge->vertexB;
         }
         else
         {
            goToVertex = currentEdge->vertexA;
         }

         //vertex is destination?
         if(goToVertex == walker->targetVertex)
         {
             //found destination, add edge and terminate
             walker->edgesIWalked.append(currentEdge);
             return;
         }
         else
         {
             MathAlgorithms::Vertex distance;
             distance.x = walker->targetVertex->x - goToVertex->x;
             distance.y = walker->targetVertex->y - goToVertex->y;
             float dist = sqrt((distance.x * distance.x) +  (distance.y * distance.y));

             distances.append(dist);
         }
    }

    float shortestDisance = -1;
    int index = -1;
    for(int i = 0; i < distances.size(); i++)
    {
        if(distances[i] < shortestDisance || shortestDisance == -1)
        {
            bool notGoodEdge = false;
            for(int ii = 0; ii < walker->edgesIWalked.count(); ii++)
            {
                if(walker->edgesIWalked[ii] == meshEdges->value(walker->startVertex->edgeIndicies[i]))
                {
                    //already walked edge
                    notGoodEdge = true;
                    break;
                }
            }

            if(!notGoodEdge)
            {
                //also need to check if edge leads into deadend
                glMeshSelectWidget::edge* candidateEdge = meshEdges->value(walker->startVertex->edgeIndicies[i]);
                glMeshSelectWidget::vertex* goToVertex = 0;

                if(candidateEdge->vertexA == walker->startVertex)
                {
                   goToVertex = candidateEdge->vertexB;
                }
                else
                {
                   goToVertex = candidateEdge->vertexA;
                }

                for(int ii = 0; ii < walker->edgesIWalked.count(); ii++)
                {
                    bool hasNoneWalkedEdge = true;
                    notGoodEdge = true;

                    for(int iii = 0; iii < goToVertex->edgeIndicies.count(); iii++)
                    {
                        if(walker->edgesIWalked[ii] == meshEdges->value(goToVertex->edgeIndicies[iii]))
                        {
                            hasNoneWalkedEdge = false;
                        }
                    }

                    if(hasNoneWalkedEdge)
                    {
                        //can use this destination
                        notGoodEdge = false;
                        break;
                    }
                }
            }

            if(!notGoodEdge)
            {
                shortestDisance = distances[i];
                index = i;
            }
        }
    }

    if(index == -1)
        return;

    glMeshSelectWidget::edge* bestEdge = meshEdges->value(walker->startVertex->edgeIndicies[index]);
    walker->edgesIWalked.append(bestEdge);
    if(bestEdge->vertexA == walker->startVertex)
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
