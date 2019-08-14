#include "PID.h"
#include "../../Utils/MathUtil/MathUtil.h"
#include "../../Constants/VehicleConstants.h"

PID::PID(float Kp, float Ki, float Kd)
  : Kp(Kp), Ki(Ki), Kd(Kd), previousError(0), cumulativeError(0)
{
}

int PID::compute(float target, float current)
{
    float currentError = target - current;
    cumulativeError += currentError;

    float output = 
        Kp * currentError +
        Ki * cumulativeError +
        Kd * (currentError - previousError);

    // Clamp output value, so we can set a maximum speed of the vehicle
    output = CLAMP(-MAX_MOTOR_PWM_OUTPUT, output, MAX_MOTOR_PWM_OUTPUT);
    
    // back up current error
    previousError = currentError;

    return output;
}