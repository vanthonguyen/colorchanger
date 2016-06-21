#ifndef IMAGELABEL_H
#define IMAGELABEL_H
#include<QLabel>

class ImageLabel : public QLabel
{
    Q_OBJECT
public:        
    void mousePressEvent(QMouseEvent *event);
    void setScaleFactor(double factor);

public slots:
    void setFactor(const double &factor);

signals:
    void pixelSelected( const QPoint& );


private:
    double scaleFactor;
};

#endif // IMAGELABEL_H
