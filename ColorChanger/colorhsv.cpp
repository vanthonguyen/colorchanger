#include "colorhsv.h"
#include <algorithm>


double ColorHSV::distance(ColorHSV other){
    float otherH = other.getH();
    double distH = 0;
    if(h > otherH){
        distH = std::min(h - otherH, otherH - h + 360);
    }else{
        distH = std::min(otherH - h, h - otherH + 360);
    }
    distH /= 360;
    double distS = s - other.getS();
    double distV = v - other.getV();
    return (float) std::sqrt(distH * distH + distS * distS + distV * distV);
}


double ColorHSV::getH(){
    return h;
}

double ColorHSV::getS(){
    return s;
}

double ColorHSV::getV(){
    return v;
}

double ColorHSV::getD() const{
    return d;
}

void ColorHSV::setD(double dd){
    d = dd;
}
