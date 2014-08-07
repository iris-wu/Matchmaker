#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow()
{
    //create widget that is directly used by the window
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    //-- create areas for OpenGl displays --//

    //create GL widgets
    glMeshWidget = new glMeshSelectWidget();
    glTextureWidget = new glTextureSelectWidget();

    //MeshSelectArea setup
    MeshSelectArea = new QScrollArea;
    MeshSelectArea->setWidget(glMeshWidget);
    MeshSelectArea->setWidgetResizable(true);
    MeshSelectArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MeshSelectArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MeshSelectArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    MeshSelectArea->setMinimumSize(100, 100);

    //TextureSelectArea setup
    TextureSelectArea = new QScrollArea;
    TextureSelectArea->setWidget(glTextureWidget);
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
    QAction* loadMeshFileAction = new QAction(tr("Load Mesh File"), this);
    connect(loadMeshFileAction, SIGNAL(triggered()), this, SLOT(SLOT_loadMeshFile()));
    QAction* loadTextureFileAction = new QAction(tr("Load Texture File"), this);
    connect(loadTextureFileAction, SIGNAL(triggered()), this, SLOT(SLOT_loadTextureFile()));

    fileMenu->addAction(loadMeshFileAction);
    fileMenu->addAction(loadTextureFileAction);

    //set title and size of window
    setWindowTitle(tr(WINDOW_TITLE));
    resize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
}

MainWindow::~MainWindow()
{

}

void MainWindow::SLOT_loadMeshFile()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    dialog.setFileMode(QFileDialog::ExistingFile);
    //dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    if(dialog.exec())
    {
       QString fileName = dialog.selectedFiles()[0];
       QFile file(fileName);
       file.open(QIODevice::ReadOnly);
       QTextStream* fileStream= new QTextStream(&file);

       //call widget handle read
       glMeshWidget->loadMeshFileCallback(fileStream);

       //all done
       delete fileStream;
       file.close();
    }
}

void MainWindow::SLOT_loadTextureFile()
{

    QFileDialog dialog(this);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    if(dialog.exec())
    {
       //get selected file name
       QString fileName = dialog.selectedFiles()[0];

       //call widget handle read
       glTextureWidget->loadTextureFromFile(fileName);
    }
}
