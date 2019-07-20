#include "Localisation.h"
#include "../Constants/Constants.h"
#include "../MathUtil/MathUtil.h"

void Localisation::tick()
{
    // Do a simple update, only using encoders for now
    Pose &pose = blackboard.worldPose;
    Odometry &odometryDiff = blackboard.odometryDiff;
    pose.x += odometryDiff.forward * cos(pose.theta);
    pose.y += odometryDiff.forward * sin(pose.theta);
    pose.theta = normaliseTheta(pose.theta + odometryDiff.turn);
}