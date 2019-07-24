#ifndef ODOMETRY_H
#define ODOMETRY_H

class Odometry
{
  public:
    Odometry() : forward(0), turn(0) {};

    float forward;  // how far vehicle has moved forward (mm)
    float turn;  // how much vehicle has turned clockwise (rad)
};

#endif // ODOMETRY_H