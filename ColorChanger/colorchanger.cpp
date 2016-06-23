#include "colorchanger.h"
#include "ui_colorchanger.h"
#include "imageutils.h"
#include "pointhsv.h"

#include <iostream>
#include <set>

#include <QOpenGLFunctions>
#include <QFileDialog>
#include <QColorDialog>
#include <QString>

#include <opencv2/opencv.hpp>

struct hsvOrder {
  bool operator() (const PointHSV &c1, const PointHSV &c2) const
  {
      return c1.d < c2.d;
  }
};

ColorChanger::ColorChanger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColorChanger){
    ui->setupUi(this);
    mThreshold = 10;
    mNbPixel = 40000;
    QToolBar *toolbar = addToolBar("main toolbar");
    QAction *aOpen = toolbar->addAction("Open");
    QAction *aSave = toolbar->addAction("Save");
    QAction *aColor = toolbar->addAction("Color");

    thresholdLabel = new QLabel(QString::number(mThreshold));
    debugLabel = new QLabel("");
    nbPixelLabel = new QLabel(QString::number(mNbPixel));
    thresholdSlider = new QSlider(Qt::Horizontal,0);
    nbPixelSlider = new QSlider(Qt::Horizontal,0);
    thresholdSlider->setMaximum(255*3);
    nbPixelSlider->setMaximum(1000000);
    nbPixelSlider->setTickInterval(1000);
    toolbar->addWidget(thresholdLabel);
    toolbar->addWidget(thresholdSlider);
    toolbar->addSeparator();
    toolbar->addWidget(nbPixelLabel);
    toolbar->addWidget(nbPixelSlider);
    toolbar->addSeparator();
    toolbar->addWidget(debugLabel);

    connect(aOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(aSave, SIGNAL(triggered()), this, SLOT(saveImage()));
    connect(aColor, SIGNAL(triggered()), this, SLOT(chooseColor()));
    connect(thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(updateThreshold(int)));
    connect(nbPixelSlider, SIGNAL(valueChanged(int)), this, SLOT(updateNbPixel(int)));


    imageLabel = new ImageLabel;
    imageLabel->setFactor(1);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //imageLabel->setScaledContents(true);
    imageLabel->setMinimumHeight(600);
    imageLabel->setMinimumWidth(800);
    connect(imageLabel, SIGNAL(pixelSelected(QPoint)), this, SLOT(changeColor(QPoint)));
    connect(imageLabel, SIGNAL(showColor(QPoint)), this, SLOT(showColorLabel(QPoint)));

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
    delete imageLabel;
    delete scrollArea;
    delete thresholdSlider;
    delete nbPixelSlider;
    delete thresholdLabel;
    delete debugLabel;
    delete nbPixelLabel;
}

void ColorChanger::changeColor(QPoint q)
{
    std::cout<<"Change color at: "<<q.x()<<" "<<q.y() <<std::endl;

    //get seed color
    cv::Vec3b hsvPixel = mHsvImage.at<cv::Vec3b>(q.y(), q.x());

    cv::Vec3b startColor = mHsvImage.at<cv::Vec3b>(q.y(), q.x());
    mHsvChangedImage = mHsvImage.clone();
    std::multiset<PointHSV, hsvOrder> pQueue;
    std::vector<QPoint> scanDirs;

    int nbPixel = mHsvImage.cols * mHsvImage.rows;
    std::vector<bool> region(nbPixel, false);
    //bool region[mHsvImage.rows][mHsvImage.cols];
    int maxArea = (nbPixel > mNbPixel) ? mNbPixel : nbPixel;

    QPoint d1(0,1);
    QPoint d2(0,-1);
    QPoint d3(1,0);
    QPoint d4(-1,0);
    QPoint d5(1,1);
    QPoint d6(1,-1);
    QPoint d7(-1,-1);
    QPoint d8(-1,1);

    scanDirs.push_back(d1);
    scanDirs.push_back(d2);
    scanDirs.push_back(d3);
    scanDirs.push_back(d4);
    scanDirs.push_back(d5);
    scanDirs.push_back(d6);
    scanDirs.push_back(d7);
    scanDirs.push_back(d8);

    int scanned = 0;
    QPoint currentPixel = q;
    cv::Vec3b meanColor = hsvPixel;

    while(scanned < maxArea){
        for(QPoint dir: scanDirs){
            int newX = currentPixel.x() + dir.x();
            int newY = currentPixel.y() + dir.y();
            if(scanned == 0){
                //std::cout<<newX<<" "<<newY<<std::endl;
            }
           int position = newY*mHsvImage.cols + newX;
//            int d2q = std::abs(newX - q.x()) + std::abs(newY - q.y());
//            if(d2q > SCAN_RADIUS){
//                continue;
//            }

            if(newX >= 0 && newX < mHsvImage.cols && newY >=0 && newY < mHsvImage.rows && region[position] == false){
                cv::Vec3b newPixel = mHsvImage.at<cv::Vec3b>(newY, newX);                
                PointHSV pointHSV(newX, newY);
                double d = ImageUtils::distanceHsL1(newPixel, startColor);
                pointHSV.d = d;
                pointHSV.color = newPixel;
                region[position] = true;
                pQueue.insert(pointHSV);
                std::cout<<newX<<" "<<newY<<std::endl;
            }
        }        
        if(pQueue.size() == 0 || (*pQueue.begin()).d > mThreshold){
            break;
        }

        auto it = pQueue.begin();
        PointHSV currentPoint = *it;

        currentPixel = currentPoint.p;
        pQueue.erase(it);
//std::cout<<currentPoint.d<<std::endl;
//if(!pQueue.empty()){
//    std::cout<<"xxx"<<(*pQueue.begin()).d<<std::endl;
//}
        cv::Vec3b currentHsv = mHsvImage.at<cv::Vec3b>(currentPixel.y(), currentPixel.x());
//std::cout<<"min bf"<<meanColor<<std::endl;
        meanColor.val[0] = (meanColor.val[0] * scanned + currentPoint.color.val[0])/(scanned + 1);
        meanColor.val[1] = (meanColor.val[1]* scanned + currentPoint.color.val[1])/(scanned + 1);
        meanColor.val[2] = (meanColor.val[2]* scanned + currentPoint.color.val[2])/(scanned + 1);
//std::cout<<"min af"<<meanColor<<std::endl;
        cv::Vec3b transColor(newColor.val[0], newColor.val[1], currentHsv.val[2]);
        mHsvChangedImage.at<cv::Vec3b>(currentPixel.y(), currentPixel.x()) = transColor;
        scanned++;
    }
    //display
    cv::Mat imgChanged;
    //test if it changed?
//    for(int row = 0; row < mHsvChangedImage.rows/2; row++){
//        for(int col = 0; col < mHsvChangedImage.cols/2; col++){
//             mHsvChangedImage.at<cv::Vec3b>(row, col) = newColor;
//        }
//    }
    cvtColor(mHsvChangedImage, imgChanged, CV_HSV2BGR);
    mchangedImg = ImageUtils::cvMatToQImage(imgChanged);
    mHsvImage = mHsvChangedImage.clone();
    imageLabel->setPixmap(QPixmap::fromImage(mchangedImg));
    imageLabel->resize(imageLabel->pixmap()->size());
    imageLabel->update();
    std::cout<<scanned<<std::endl;
}

void ColorChanger::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/tom/loisir", tr("Image Files (*.png *.jpg *.bmp)"));

    mRenderQtImg = QImage(fileName);
    imageLabel->setPixmap(QPixmap::fromImage(mRenderQtImg));
    imageLabel->resize(imageLabel->pixmap()->size());
    //convert to Opencv
    mImage = ImageUtils::QImageToCvMat(mRenderQtImg);
    cvtColor(mImage, mHsvImage, CV_BGR2HSV);
    //std::cout<<fileName.toStdString()<<std::endl;
}
void ColorChanger::saveImage(){
    QString fileName = QFileDialog::getSaveFileName(this,
         tr("Open Image"), "/home/tom/loisir", tr("Image Files (*.png *.jpg *.bmp)"));
     cv::Mat imgChanged;
     cvtColor(mHsvChangedImage, imgChanged, CV_HSV2BGR);
     imwrite(fileName.toStdString(), imgChanged);
}

void ColorChanger::chooseColor(){
    QColor color = QColorDialog::getColor(Qt::yellow, this );
    cv::Vec3b newHsv(color.hsvHue(), color.hsvSaturation(), 0);
    newColor = newHsv;
}

void ColorChanger::updateThreshold(int th){
    mThreshold = th;
    thresholdLabel->setText(QString::number(th));
}

void ColorChanger::updateNbPixel(int newNb){
    mNbPixel = newNb;
    nbPixelLabel->setText(QString::number(newNb));
}

void ColorChanger::showColorLabel(QPoint q){
    std::cout<<"clicked"<<std::endl;
    if(mHsvImage.cols > 0){
        cv::Vec3b phsv = mHsvImage.at<cv::Vec3b>(q.y(), q.x());
        debugLabel->setText(QString::number(q.x()) + "," + QString::number(q.y()) + ":" + QString::number(phsv.val[0]) + "," +
                QString::number(phsv.val[1]) + "," + QString::number(phsv.val[2]));
    }
}
