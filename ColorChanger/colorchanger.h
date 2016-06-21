#ifndef COLORCHANGER_H
#define COLORCHANGER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

#include <opencv2/core/core.hpp>
#include "imagelabel.h"

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
    void changeColor(QPoint p);

private slots:
    void on_btnOpen_clicked();

private:
    QImage      mRenderQtImg;           /// Qt image to be rendered
    QImage      mResizedImg;
    cv::Mat     mOrigImage; /// original OpenCV image to be shown

    Ui::ColorChanger *ui;
    ImageLabel *imageLabel;
    QScrollArea *scrollArea;
};

#endif // COLORCHANGER_H
