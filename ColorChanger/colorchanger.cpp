#include "colorchanger.h"
#include "ui_colorchanger.h"

#include <iostream>

#include <QOpenGLFunctions>
#include <QFileDialog>
#include <QString>

#include <opencv2/opencv.hpp>

ColorChanger::ColorChanger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColorChanger)
{
    ui->setupUi(this);

    QToolBar *toolbar = addToolBar("main toolbar");
    QAction *aOpen = toolbar->addAction("Open");
    QAction *aSave = toolbar->addAction("Save");
    toolbar->addSeparator();

    connect(aOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(imageLabel, SIGNAL(pointSelected(QPoint &)), this, SLOT(changeColor(QPoint)));

    imageLabel = new ImageLabel;
    imageLabel->setFactor(1);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //imageLabel->setScaledContents(true);
    imageLabel->setMinimumHeight(600);
    imageLabel->setMinimumWidth(800);


    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setMinimumHeight(600);
    scrollArea->setMinimumWidth(800);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setCentralWidget(scrollArea);


    setWindowTitle(tr("Image Viewer"));
    resize(800, 600);
}

ColorChanger::~ColorChanger()
{
    delete ui;
}

ColorChanger::changeColor(QPoint q)
{

}


void ColorChanger::on_btnOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/tom", tr("Image Files (*.png *.jpg *.bmp)"));

    mRenderQtImg = QImage(fileName);
    //ui->imageLabel->setPixmap(QPixmap::fromImage(mRenderQtImg));
    std::cout<<fileName.toStdString()<<std::endl;
}

void ColorChanger::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/tom", tr("Image Files (*.png *.jpg *.bmp)"));

    mRenderQtImg = QImage(fileName);
    imageLabel->setPixmap(QPixmap::fromImage(mRenderQtImg));
    imageLabel->resize(imageLabel->pixmap()->size());
    std::cout<<fileName.toStdString()<<std::endl;
}
