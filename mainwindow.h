#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include "glmeshselectwidget.h"
#include "gltextureselectwidget.h"
#include "glfinalwidget.h"
#include "glprogresswidget.h"

#define WINDOW_TITLE "Matchmaker"
#define WINDOW_SIZE_WIDTH 800
#define WINDOW_SIZE_HEIGHT 800

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private slots:
    void SLOT_loadMeshFile();
    void SLOT_loadTextureFile();
    void SLOT_ParameterizeMesh();
    void SLOT_TriangulateTexture();

protected:
    QWidget* mainWidget; //contains basically all layout elements, can be considered the frame of the window
    glMeshSelectWidget* glMeshWidget;
    glTextureSelectWidget* glTextureWidget;
    glProgressWidget* progressWidget;
    glFinalWidget* finalWidget;
    QScrollArea* MeshSelectArea;
    QScrollArea* TextureSelectArea;
    QScrollArea* FinalSelectArea;
    QScrollArea* ProgressSelectArea;
    QAction* meshParameterizationAction;
    QAction* triangulateTextureAction;
};

#endif // MAINWINDOW_H
