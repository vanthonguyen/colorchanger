#ifndef COLORHSV_H
#define COLORHSV_H


class ColorHSV{
public:
    ColorHSV() : h(0), s(0), v(0){
    }

    ColorHSV(double hh, double ss, double vv) : h(hh), s(ss), v(vv){
    }

    double distance(ColorHSV other);
    double getH();
    double getS();
    double getV();
    double getD() const;
    void setD(double dd);

private:
    double h;
    double s;
    double v;
    double d;
};

#endif // COLORHSV_H
