#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include "glmeshselectwidget.h"
#include "gltextureselectwidget.h"

#define WINDOW_TITLE "Matchmaker"
#define WINDOW_SIZE_WIDTH 800
#define WINDOW_SIZE_HEIGHT 600

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private slots:
    void SLOT_loadMeshFile();
    void SLOT_loadTextureFile();

protected:
    QWidget* mainWidget; //contains basically all layout elements, can be considered the frame of the window
    glMeshSelectWidget* glMeshWidget;
    glTextureSelectWidget* glTextureWidget;
    QScrollArea* MeshSelectArea;
    QScrollArea* TextureSelectArea;
};

#endif // MAINWINDOW_H
