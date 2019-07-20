#ifndef POSE_H
#define POSE_H

class Pose
{
  public:
    Pose() : x(0), y(0), theta(0){}

    float x;
    float y;
    float theta;
};

#endif // POSE_H