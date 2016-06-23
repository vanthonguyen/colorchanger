#ifndef COLORCHANGER_H
#define COLORCHANGER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QSlider>

#include <opencv2/core/core.hpp>
#include "imagelabel.h"

#define MAX_PIXELS 40000
#define SCAN_RADIUS 10

namespace Ui {
class ColorChanger;
}

class ColorChanger : public QMainWindow
{
    Q_OBJECT

public:
    explicit ColorChanger(QWidget *parent = 0);
    ~ColorChanger();

public slots:
    void openFile();
    void saveImage();
    void updateThreshold(int th);
    void updateNbPixel(int newNb);
    void changeColor(QPoint p);
    void showColorLabel(QPoint p);
    void chooseColor();

private:
    QImage      mRenderQtImg;           /// Qt image to be rendered
    QImage      mchangedImg;
    cv::Mat     mImage; /// original OpenCV image to be shown
    cv::Mat     mHsvImage;
    cv::Mat     mHsvChangedImage;
    cv::Vec3b   newColor;
    int         mNbPixel;
    double      mThreshold;
    //for undo
    //std::vector<>

    Ui::ColorChanger *ui;
    ImageLabel *imageLabel;
    QScrollArea *scrollArea;
    QSlider      *thresholdSlider;
    QSlider      *nbPixelSlider;
    QLabel      *thresholdLabel;
    QLabel      *nbPixelLabel;
    QLabel      *debugLabel;

};

#endif // COLORCHANGER_H
