#include "Localisation.h"
#include "../Constants/Constants.h"
#include "../MathUtil/MathUtil.h"

Localisation::Localisation(Blackboard &blackboard)
    : blackboard(blackboard)
{
    reset();
}

void Localisation::reset()
{
    blackboard.worldPose = Pose(WHEELS_OFFSET_X_MM, 0, 0);
}

void Localisation::tick()
{
    if (blackboard.resetLocalisation)
        reset();

    blackboard.resetLocalisation = false;  // reset flag

    // Do a simple update, only using encoders for now
    Pose &pose = blackboard.worldPose;
    Odometry &odometryDiff = blackboard.odometryDiff;
    pose.x += odometryDiff.forward * cos(pose.theta);
    pose.y += odometryDiff.forward * sin(pose.theta);
    pose.theta = normaliseTheta(pose.theta + odometryDiff.turn);
    
    // unsigned long time = millis();
    // pose.theta = normaliseTheta(pose.theta + blackboard.gyroZ * MS_TO_S(time - lastTime));
    // lastTime = time;
}
