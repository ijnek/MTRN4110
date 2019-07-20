#ifndef MOVEMENT_REQUEST_H
#define MOVEMENT_REQUEST_H

class MovementRequest
{
  public:
    MovementRequest(float forwardVelocity=0, float turnVelocity=0) : forwardVelocity(forwardVelocity), turnVelocity(turnVelocity) {}
    float forwardVelocity;  // velocity forwards (mm/s)
    float turnVelocity;  // turn velocity clockwise (rad/s)
};

#endif // MOVEMENT_REQUEST_H