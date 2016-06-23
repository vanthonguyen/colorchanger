#ifndef POINTHSV_H
#define POINTHSV_H
#include <opencv2/core/core.hpp>
#include<QPoint>

class PointHSV
{
public:
    PointHSV(int xx, int yy): p(xx, yy){
    }

    PointHSV(){
    }

    QPoint p;
    cv::Vec3f color;
    double d;
};

#endif // POINTHSV_H
