#ifndef POSE_H
#define POSE_H

class Pose
{
  public:
    Pose() : x(0), y(0), theta(0){}
    Pose(float x, float y, float theta) : x(x), y(y), theta(theta){}

    float x;  // distance in robot's initial forward diretion (mm)
    float y;  // distance in robot's initial left diretion (mm)
    float theta;  // heading from robot's initial heading (-3.14rad to 3.14rad)
};

#endif // POSE_H