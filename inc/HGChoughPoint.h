
#ifndef HGCHOUGHPOINT_H
#define HGCHOUGHPOINT_H

/* C++ */
#include <iostream>


class HGChoughPoint{

public:

    HGChoughPoint() { ; }

    HGChoughPoint(double x, double y) { 
        _x=x;
        _y=y;
    }
    ~HGChoughPoint() { ; }

    void setX(double x) { _x = x; }
    void setY(double y) { _y = y; }

    double getX() { return _x; }
    double getY() { return _y; }


private:

    double _x;
    double _y;

};

#endif
