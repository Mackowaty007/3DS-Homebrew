#ifndef FIND_INTER_HPP
#define FIND_INTER_HPP


#ifndef max
  #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
  #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


class Point2d{
public:
    Point2d() {}
    Point2d(double x, double y)
        : X(x), Y(y) {}

    double x() const { return X; }
    double y() const { return Y; }

    /**
     * Returns the norm of this vector.
     * @return the norm
    */
    double norm() const {
        return sqrt( X * X + Y * Y );
    }

    void setCoords(double x, double y) {
        X = x; Y = y;
    }

private:
    double X;
    double Y;
};

class Circle{
public:
    /**
     * @param R - radius
     * @param C - center
     */
    Circle(double R, Point2d& C)
        : r(R), c(C) {}

    /**
     * @param R - radius
     * @param X - center's x coordinate
     * @param Y - center's y coordinate
     */
    Circle(double R, double X, double Y)
        : r(R), c(X, Y) {}

    Point2d getC() const { return c; }
    double getR() const { return r; }

    size_t intersect(const Circle& C2, Point2d& i1, Point2d& i2) {
        // distance between the centers
        double d = Point2d(c.x() - C2.c.x(),
                c.y() - C2.c.y()).norm();


        double a = (r*r - C2.r*C2.r + d*d)/ (2.0*d);
        double h = sqrt(r*r - a*a);

        // find p2
        Point2d p2( c.x() + (a * (C2.c.x() - c.x())) / d,
            c.y() + (a * (C2.c.y() - c.y())) / d);

            // find intersection points p3
        i1.setCoords( p2.x() + (h * (C2.c.y() - c.y())/ d),
            p2.y() - (h * (C2.c.x() - c.x())/ d)
        );
        i2.setCoords( p2.x() - (h * (C2.c.y() - c.y())/ d),
            p2.y() + (h * (C2.c.x() - c.x())/ d)
        );

        return 1;
    }

private:
    // radius
    double r;
    // center
    Point2d c;
};

#endif
