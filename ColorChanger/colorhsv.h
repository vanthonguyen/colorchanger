#ifndef COLORHSV_H
#define COLORHSV_H


class ColorHSV
{
public:
    ColorHSV() : h(0), s(0), v(0);
    ColorHSV(double hh, double ss, double vv) : h(hh), s(ss), v(vv);
    double distance();
    public getH();
    public getS();
    public getV();
}

private:
    double h;
    double s;
    double v;
};

#endif // COLORHSV_H
