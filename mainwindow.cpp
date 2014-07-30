#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glmeshselectwidget.h"
#include "gltextureselectwidget.h"

MainWindow::MainWindow()
{
    //create widget that is directly used by the window
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    //-- create areas for OpenGl displays --//

    //MeshSelectArea setup
    MeshSelectArea = new QScrollArea;
    MeshSelectArea->setWidget(new glMeshSelectWidget());
    MeshSelectArea->setWidgetResizable(true);
    MeshSelectArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MeshSelectArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MeshSelectArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    MeshSelectArea->setMinimumSize(100, 100);

    //TextureSelectArea setup
    TextureSelectArea = new QScrollArea;
    TextureSelectArea->setWidget(new glTextureSelectWidget());
    TextureSelectArea->setWidgetResizable(true);
    TextureSelectArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    TextureSelectArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    TextureSelectArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    TextureSelectArea->setMinimumSize(100, 100);

    //-- Set what layout and add compontents --//

    //use grid layout to split screen
    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(MeshSelectArea, 0, 0);
    centralLayout->addWidget(TextureSelectArea, 0, 1);
    mainWidget->setLayout(centralLayout);

    //add menu bar and associated options
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    //actions
    QAction* loadMeshFile = new QAction(tr("Load Mesh File"), this);
    QAction* loadTextureFile = new QAction(tr("Load Texture File"), this);

    fileMenu->addAction(loadMeshFile);
    fileMenu->addAction(loadTextureFile);

    //set title and size of window
    setWindowTitle(tr(WINDOW_TITLE));
    resize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
}

MainWindow::~MainWindow()
{
}
