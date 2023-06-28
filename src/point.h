// Header file for point class

#ifndef POINT_H_
#define POINT_H_

#include "globals.h"

namespace point1
{

class point
{
public:
    // default const.
    point();
    // copy const.
    point(int yy, int xx);
    // set...
    void set(int yy, int xx);
    // set y only
    void sety(int yy);
    // set x only
    void setx(int xx);
    // get y value of point.
    int y();
    // get x value of point.
    int x();
private:
    int x_,y_; // member variables for x and y coord.
};

bool isAt(point,point);

bool isWithin(point,point,int);

bool inStraightLineFrom(point,point);

int pointDist(point,point);

bool inBounds(point);

}

#endif
