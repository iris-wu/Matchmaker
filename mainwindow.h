#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>

#define WINDOW_TITLE "Matchmaker"
#define WINDOW_SIZE_WIDTH 800
#define WINDOW_SIZE_HEIGHT 600

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

protected:
    QWidget* mainWidget; //contains basically all layout elements, can be considered the frame of the window
    QScrollArea* MeshSelectArea;
    QScrollArea* TextureSelectArea;
};

#endif // MAINWINDOW_H
