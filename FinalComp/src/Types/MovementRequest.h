#ifndef MOVEMENT_REQUEST_H
#define MOVEMENT_REQUEST_H

class MovementRequest
{
  public:
    MovementRequest(float forwardAmount=0, float turnAmount=0) : forwardAmount(forwardAmount), turnAmount(turnAmount) {}
    float forwardAmount;  // (mm)
    float turnAmount;  // (rad)
};

#endif // MOVEMENT_REQUEST_H