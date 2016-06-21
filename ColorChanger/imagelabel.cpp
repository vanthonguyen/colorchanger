#include "imagelabel.h"
#include<iostream>
#include<QMouseEvent>

void ImageLabel::mousePressEvent(QMouseEvent *event)

{
    if (event->button() == Qt::LeftButton && event->modifiers() & Qt::MetaModifier) {
         std::cout<<"x: "<< event->x() <<std::endl;
         std::cout<<"y: "<< event->y() <<std::endl;

         emit pixelSelected( event->pos() / scaleFactor);
    }

}

void ImageLabel::setFactor(const double &factor){
   scaleFactor = factor;
}
