// point class implementation:

#include "point.h"

namespace point1
{

// default const.
point::point()
{
    x_ = y_ = 0;
}

point::point(int yy, int xx)
{
    x_ = xx;
    y_ = yy;
}

// set...
void point::set(int yy, int xx)
{
    x_ = xx;
    y_ = yy;
}

// set y only
void point::sety(int yy)
{
    y_ = yy;
}

// set x only
void point::setx(int xx)
{
    x_ = xx;
}

// get y value of point.
int point::y()
{
    return y_;
}

// get x value of point.
int point::x()
{
    return x_;
}

bool isAt(point p1, point p2)
{
    return (p1.y() == p2.y() && p1.x() == p2.x());
}

bool isWithin(point p1, point p2, int n)
{
    return ((p1.y() - p2.y() <= n && p1.y() - p2.y() >= -n) &&
            (p1.x() - p2.x() <= n && p1.x() - p2.x() >= -n));
}

int pointDist(point p1, point p2)
{
    return (int)sqrt((double)((p2.y() - p1.y()) * (p2.y() - p1.y()) +
                              (p2.x() - p1.x()) * (p2.x() - p1.x()) ));
}

bool inStraightLineFrom(point p1, point p2)
{
    return (abs(p1.y() - p2.y()) == abs(p1.x() - p2.x()) || (p1.y() == p2.y()) || (p1.x() == p2.x()));
}

bool inBounds(point p)
{
    return (p.y() >= 0 && p.x() >= 0 && p.y() < PHGT && p.x() < PWID);
}

};
