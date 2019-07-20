#ifndef WAY_POINT_H
#define WAY_POINT_H

class WayPoint
{
  public:
    WayPoint() : x(0), y(0) {}
    WayPoint(float x, float y) : x(x), y(y) {}

    float x;
    float y;
};

#endif // WAY_POINT_H