#include <iostream>

struct hsv{
    hsv() {};
    hsv(double h_in, double s_in, double v_in) : h(h_in), s(s_in), v(v_in) {}

    double h;// angle in degrees
    double s;// a fraction between 0 and 1
    double v;// a fraction between 0 and 1
};

struct rgb{
    rgb() {}
    rgb(hsv in) {
        double r_;
        double g_;
        double b_;
        double hh, p, q, t, ff;
        long i;
        if (in.s <= 0.0)
        { // < is bogus, just shuts up warnings
            r_ = in.v;
            g_ = in.v;
            b_ = in.v;
        }
        hh = in.h;
        if (hh >= 360.0)
            hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = in.v * (1.0 - in.s);
        q = in.v * (1.0 - (in.s * ff));
        t = in.v * (1.0 - (in.s * (1.0 - ff)));

        switch (i)
        {
        case 0:
            r_ = in.v;
            g_ = t;
            b_ = p;
            break;
        case 1:
            r_ = q;
            g_ = in.v;
            b_ = p;
            break;
        case 2:
            r_ = p;
            g_ = in.v;
            b_ = t;
            break;

        case 3:
            r_ = p;
            g_ = q;
            b_ = in.v;
            break;
        case 4:
            r_ = t;
            g_ = p;
            b_ = in.v;
            break;
        case 5:
        default:
            r_ = in.v;
            g_ = p;
            b_ = q;
            break;
        }
        r = r_ * 255;
        g = g_ * 255;
        b = b_ * 255;
    };
    
    int r;       // a fraction between 0 and 1
    int g;       // a fraction between 0 and 1
    int b;       // a fraction between 0 and 1
};

std::ostream   &operator<<(std::ostream &f, rgb color) {
    return f << "{" << color.r << ", " << color.g << ", " << color.b << "}" << std::endl;
}

int     main(void) {
    hsv c(219, 0.78, 0.92);

    std::cout << c << std::endl;
}