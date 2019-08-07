#ifndef WAY_POINT_H
#define WAY_POINT_H

class WayPoint
{
  public:
    WayPoint() : x(0), y(0), h(0) {}
    WayPoint(float x, float y, float h) : x(x), y(y), h(h) {}

    float x;
    float y;
    float h;
};

#endif // WAY_POINT_H