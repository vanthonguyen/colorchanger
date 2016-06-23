#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H
#include <opencv2/core/core.hpp>
#include <QDebug>

class ImageUtils{
public:
    static inline cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true )
    {
       switch ( inImage.format() )
       {
          // 8-bit, 4 channel
          case QImage::Format_RGB32:
          {
             cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

             return (inCloneImageData ? mat.clone() : mat);
          }

          // 8-bit, 3 channel
          case QImage::Format_RGB888:
          {
             if ( !inCloneImageData )
                qWarning() << "ImageUtils.h - Conversion requires cloning since we use a temporary QImage";

             QImage   swapped = inImage.rgbSwapped();

             return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
          }

          // 8-bit, 1 channel
          case QImage::Format_Indexed8:
          {
             cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

             return (inCloneImageData ? mat.clone() : mat);
          }

          default:
             qWarning() << "ImageUtils.h - QImage format not handled in switch:" << inImage.format();
             break;
       }

       return cv::Mat();
    }

    static inline QImage  cvMatToQImage( const cv::Mat &inMat )
    {
       switch ( inMat.type() )
       {
          // 8-bit, 4 channel
          case CV_8UC4:
          {
             QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

             return image;
          }

          // 8-bit, 3 channel
          case CV_8UC3:
          {
             QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

             return image.rgbSwapped();
          }

          // 8-bit, 1 channel
          case CV_8UC1:
          {
             static QVector<QRgb>  sColorTable;

             // only create our color table once
             if ( sColorTable.isEmpty() )
             {
                for ( int i = 0; i < 256; ++i )
                   sColorTable.push_back( qRgb( i, i, i ) );
             }

             QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

             image.setColorTable( sColorTable );

             return image;
          }

          default:
             qWarning() << "ImageUtil.h - cv::Mat image type not handled in switch:" << inMat.type();
             break;
       }

       return QImage();
    }
    static double hsvCompare(cv::Vec3b c1, cv::Vec3b c2 ){
        double distH = 0;
        //Hue
        if(c1.val[0] > c2.val[0]){
            distH = std::min(c1.val[0] - c2.val[0], c2.val[0] - c1.val[0] + 360);
        }else{
            distH = std::min(c2.val[0] - c1.val[0] , c1.val[0] - c2.val[0] + 360);
        }
        distH /= 360;

        double distS = c1.val[1] - c2.val[1];
        double distV = c1.val[2] - c2.val[2];
        return std::sqrt(distH * distH + distS * distS + distV * distV);
    }

    static double distanceHsvL1(cv::Vec3b c1, cv::Vec3b c2 ){
//        double distH = 0;
//        //Hue
//        if(c1.val[0] > c2.val[0]){
//            distH = std::min(c1.val[0] - c2.val[0], c2.val[0] - c1.val[0] + 360);
//        }else{
//            distH = std::min(c2.val[0] - c1.val[0] , c1.val[0] - c2.val[0] + 360);
//        }
//        distH /= 360;
        double distH = std::abs(c1.val[0] - c2.val[0]);
        double distS = std::abs(c1.val[1] - c2.val[1]);
        double distV = std::abs(c1.val[2] - c2.val[2]);
        return distH + distS + distV;
    }

    static double distanceHs(cv::Vec3b c1, cv::Vec3b c2 ){
        double distH = c1.val[0] - c2.val[0];
        double distS = c1.val[1] - c2.val[1];
        return std::sqrt(distH * distH + distS * distS);
    }
    static double distanceHsL1(cv::Vec3b c1, cv::Vec3b c2 ){
        double distH = std::abs(c1.val[0] - c2.val[0]);
        double distS = std::abs(c1.val[1] - c2.val[1]);
        return distH + distS;
    }
    static double distanceH(cv::Vec3b c1, cv::Vec3b c2 ){
        double distH = 0;
        //Hue
        if(c1.val[0] > c2.val[0]){
            distH = std::min(c1.val[0] - c2.val[0], c2.val[0] - c1.val[0] + 360);
        }else{
            distH = std::min(c2.val[0] - c1.val[0] , c1.val[0] - c2.val[0] + 360);
        }
        distH /= 360;

        double distS = c1.val[1] - c2.val[1];
        double distV = c1.val[2] - c2.val[2];
        return distH;
    }
};

#endif // IMAGEUTILS_H
