#ifndef PID_CUSTOM_H
#define PID_CUSTOM_H

class PID
{
  public:
    PID(float Kp, float Ki, float Kd);
    int compute(float target, float current);

  private:
    float Kp;
    float Ki;
    float Kd;

    float previousError;
    float cumulativeError;
};

#endif // PID_CUSTOM_H