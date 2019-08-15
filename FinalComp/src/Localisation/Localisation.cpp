#include "Localisation.h"
#include "../Constants/VehicleConstants.h"
#include "../Utils/MathUtil/MathUtil.h"

#define INITIAL_UNCERTAINTY_X 50.0  // mm
#define INITIAL_UNCERTAINTY_Y 50.0  // mm
#define INITIAL_UNCERTAINTY_H DEG2RAD(10)  // rad

#define LIDAR_LEFT_X 55.0  // mm
#define LIDAR_LEFT_Y 50.0  //  mm
#define LIDAR_LEFT_H DEG2RAD(90)  // rad

#define LIDAR_RIGHT_X 55.0  // mm
#define LIDAR_RIGHT_Y -50.0  // mm
#define LIDAR_RIGHT_H DEG2RAD(-90)  // rad

#define LIDAR_FRONT_X 80.0  // mm
#define LIDAR_FRONT_Y 0.0  // mm
#define LIDAR_FRONT_H 0.0  // rad

#define LIDAR_UNCERTAINTY 40  // mm

#define CELL_SIZE 250.0  // mm

#define ODOMETRY_FORWARD_MULTIPLY_FACTOR 1.0
#define ODOMETRY_TURN_PROCESS_VARIANCE DEG2RAD(2) * DEG2RAD(2)


// static int dataRowIndex = 0;

// const float data[][7] = {
// 40, 2.59, 0.00, 255, 86, 78, -0.01, 39, 2.69, 0.00, 255, 84, 79, -0.00, 39, 2.50, -0.00, 255, 84, 79, -0.00, 39, 2.69, 0.00, 255, 84, 79, -0.00, 39, 2.83, 0.00, 255, 83, 80, -0.00, 40, 3.39, 0.00, 255, 84, 79, 0.00, 38, 3.53, -0.00, 255, 85, 79, -0.00, 40, 3.25, -0.01, 255, 85, 80, 0.00, 39, 3.16, -0.00, 255, 85, 80, 0.00, 38, 3.16, 0.00, 255, 82, 79, -0.00, 40, 3.44, -0.00, 255, 82, 81, 0.00, 39, 3.63, -0.00, 255, 84, 82, 0.00, 39, 3.58, 0.00, 255, 82, 81, -0.00, 39, 3.20, 0.01, 255, 82, 82, -0.01, 40, 2.17, 0.00, 255, 82, 81, 0.00, 39, 3.91, -0.00, 255, 83, 82, -0.00, 39, 2.73, -0.01, 255, 81, 83, 0.00, 39, 4.52, -0.01, 255, 80, 83, 0.01, 39, 3.06, -0.00, 255, 82, 84, 0.00, 40, 4.90, -0.00, 255, 82, 83, 0.02, 39, 3.35, 0.00, 255, 82, 83, 0.00, 39, 5.42, 0.01, 255, 82, 82, 0.00, 39, 3.53, 0.01, 255, 81, 84, 0.00, 38, 5.56, 0.01, 255, 82, 84, -0.02, 40, 3.30, -0.00, 255, 82, 84, 0.00, 39, 5.47, -0.01, 255, 82, 84, -0.01, 39, 4.71, -0.01, 255, 79, 86, 0.00, 39, 5.00, -0.01, 255, 81, 87, 0.01, 39, 5.09, 0.00, 255, 83, 83, 0.00, 40, 4.95, 0.01, 255, 79, 86, -0.00, 39, 4.52, 0.01, 255, 77, 89, -0.01, 40, 3.96, 0.01, 255, 80, 87, -0.01, 39, 3.58, 0.00, 255, 79, 90, -0.01, 39, 3.49, 0.00, 255, 78, 91, -0.00, 38, 3.49, 0.00, 255, 79, 92, -0.00, 39, 3.20, -0.00, 255, 76, 91, -0.00, 39, 3.16, -0.01, 255, 77, 90, 0.00, 40, 3.49, -0.01, 255, 81, 92, 0.01, 39, 4.10, -0.00, 255, 81, 92, 0.01, 39, 4.52, -0.00, 255, 80, 92, 0.00, 40, 4.76, -0.00, 255, 77, 90, 0.00, 39, 4.85, -0.00, 255, 77, 90, 0.00, 39, 5.00, -0.00, 255, 78, 93, 0.00, 39, 5.51, -0.00, 255, 77, 94, 0.00, 39, 5.75, -0.00, 255, 78, 93, -0.00, 40, 5.65, -0.01, 255, 78, 96, 0.00, 38, 5.37, -0.01, 255, 78, 94, 0.01, 39, 4.71, -0.02, 255, 78, 92, 0.02, 39, 4.38, -0.02, 255, 78, 92, 0.02, 40, 4.19, -0.01, 255, 78, 94, 0.01, 39, 4.19, 0.01, 255, 78, 92, -0.01, 39, 4.01, 0.02, 255, 81, 93, -0.01, 40, 3.77, 0.02, 255, 86, 98, -0.02, 39, 3.63, 0.01, 255, 92, 112, -0.02, 39, 3.53, 0.01, 255, 102, 163, -0.01, 40, 3.35, 0.00, 255, 123, 255, -0.01, 39, 3.25, -0.00, 255, 144, 255, 0.00, 39, 3.35, -0.00, 255, 162, 255, 0.00, 39, 3.72, -0.00, 255, 161, 255, 0.00, 39, 4.43, -0.00, 255, 156, 255, 0.00, 39, 5.09, 0.00, 255, 151, 255, 0.00, 39, 5.70, -0.01, 255, 131, 255, 0.00, 40, 4.43, -0.01, 255, 109, 255, 0.00, 39, 7.30, -0.01, 255, 99, 255, 0.01, 39, 4.48, 0.00, 255, 100, 255, 0.00, 39, 7.35, 0.01, 255, 98, 255, 0.01, 40, 4.76, 0.01, 255, 101, 255, 0.00, 39, 7.12, 0.02, 255, 100, 255, -0.02, 39, 4.48, 0.01, 255, 97, 255, 0.00, 39, 7.12, -0.01, 255, 99, 255, -0.03, 40, 4.43, -0.01, 255, 96, 255, 0.00, 38, 7.26, -0.02, 255, 99, 255, 0.02, 39, 4.67, -0.01, 255, 121, 255, 0.00, 40, 7.21, -0.01, 255, 255, 255, 0.03, 39, 6.13, 0.01, 255, 255, 255, 0.01, 39, 5.80, 0.01, 255, 255, 255, -0.01, 39, 6.13, 0.01, 193, 255, 255, -0.02, 40, 6.64, 0.00, 185, 255, 255, -0.01, 39, 6.88, -0.01, 182, 255, 255, -0.01, 39, 6.50, -0.02, 172, 255, 255, 0.01, 64, 6.08, -0.02, 164, 255, 255, 0.02, 39, 10.13, -0.01, 157, 255, 255, 0.02, 37, 6.41, 0.01, 153, 255, 255, 0.01, 41, 6.17, 0.01, 144, 255, 255, -0.01
// };


Localisation::Localisation(Blackboard &blackboard)
    : blackboard(blackboard)
{
    reset();
}

void Localisation::reset()
{
    blackboard.worldPose = Pose(-WHEELS_OFFSET_X_MM, 0, 0);

    state[0] = -WHEELS_OFFSET_X_MM;
    state[1] = 0;
    state[2] = 0;

    for (unsigned i = 0; i < 3; ++i)
    {
        for (unsigned j = 0; j < 3; ++j)
        {
            covariance[i][j] = 0;
        }
    }
    covariance[0][0] = INITIAL_UNCERTAINTY_X * INITIAL_UNCERTAINTY_X;
    covariance[1][1] = INITIAL_UNCERTAINTY_Y * INITIAL_UNCERTAINTY_Y;
    covariance[2][2] = INITIAL_UNCERTAINTY_H * INITIAL_UNCERTAINTY_H;
}

void Localisation::tick()
{
    if (blackboard.startDetected)
    {

        float forwardChange = blackboard.odometryDiff.forward;
        float turnChange = blackboard.odometryDiff.turn;
        float lidarFront = blackboard.lidarFront;
        float lidarLeft = blackboard.lidarLeft;
        float lidarRight = blackboard.lidarRight;

        // uncomment if simulating
        // float forwardChange = data[dataRowIndex][1];
        // float turnChange = data[dataRowIndex][2];
        // float lidarFront = data[dataRowIndex][3];
        // float lidarLeft = data[dataRowIndex][4];
        // float lidarRight = data[dataRowIndex][5];
        // dataRowIndex++;

        predict(forwardChange, turnChange);

        // Only do updates if we think we're facing one of the four directions
        if (abs(state[2]) < DEG2RAD(10) ||
            abs(state[2] - DEG2RAD(90)) < DEG2RAD(10) || 
            abs(state[2] + DEG2RAD(90)) < DEG2RAD(10) || 
            abs(normaliseTheta(state[2] + DEG2RAD(180))) < DEG2RAD(10))
        {
            // Serial1.print("Left: ");
            // Serial1.println(lidarLeft);
            updateWithLidar(LIDAR_LEFT_X, LIDAR_LEFT_Y, LIDAR_LEFT_H, lidarLeft);
            // Serial1.println("Front");
            // Serial1.print("Front: ");
            // Serial1.println(lidarFront);
            updateWithLidar(LIDAR_FRONT_X, LIDAR_FRONT_Y, LIDAR_FRONT_H, lidarFront);
            // // Serial1.println("Right");
            // Serial1.print("Right: ");
            // Serial1.println(lidarRight);
            updateWithLidar(LIDAR_RIGHT_X, LIDAR_RIGHT_Y, LIDAR_RIGHT_H, lidarRight);
        }
    }

    // Serial1.print(state[0]);
    // Serial1.print(", ");
    // Serial1.print(state[1]);
    // Serial1.print(", ");
    // Serial1.print(state[2]);
    // Serial1.print(", ");

    // for (unsigned i = 0; i < 3; ++i)
    // {
    //     for (unsigned j = 0; j < 3; ++j)
    //     {
    //         Serial1.print(covariance[i][j]);
    //         if (i != 2 || j != 2)
    //             Serial1.print(", ");
    //     }
    // }
    // Serial1.print("\n");

    blackboard.worldPose = Pose(state[0], state[1], state[2]);
}

void Localisation::predict(float forwardChange, float turnChange)
{
    // Do a simple update, only using encoders for now
    float deltaX = forwardChange * cosf(state[2]);
    float deltaY = forwardChange * sinf(state[2]);
    state[0] += deltaX;
    state[1] += deltaY;
    state[2] = normaliseTheta(state[2] + turnChange);

    mtx_type forwardCovariance[1][1];
    forwardCovariance[0][0] = forwardChange * forwardChange * ODOMETRY_FORWARD_MULTIPLY_FACTOR;

    mtx_type rotationMatrix[2][1];
    rotationMatrix[0][0] = cos(state[2]);
    rotationMatrix[1][0] = sin(state[2]);

    mtx_type rotationMatrixTranspose[1][2];
    rotationMatrixTranspose[0][0] = cos(state[2]);
    rotationMatrixTranspose[0][1] = sin(state[2]);

    mtx_type tmp1[2][1];
    Matrix.Multiply((mtx_type*) rotationMatrix, (mtx_type*) forwardCovariance, 2, 1, 1, (mtx_type*) tmp1);
    
    mtx_type tmp2[2][2];
    Matrix.Multiply((mtx_type*) tmp1, (mtx_type*) rotationMatrixTranspose, 2, 1, 2, (mtx_type*) tmp2);

    for (unsigned i = 0; i < 2; ++i)
    {
        for (unsigned j = 0; j < 2; ++j)
        {
            covariance[i][j] += tmp2[i][j];
        }
    }

    mtx_type propagationJacobian[3][3];
    propagationJacobian[0][0] = 1.0;
    propagationJacobian[0][1] = 0.0;
    propagationJacobian[0][2] = -forwardChange * sinf(state[2]);
    propagationJacobian[1][0] = 0.0;
    propagationJacobian[1][1] = 1.0;
    propagationJacobian[1][2] = forwardChange * cosf(state[2]);
    propagationJacobian[2][0] = 0.0;
    propagationJacobian[2][1] = 0.0;
    propagationJacobian[2][2] = 1.0;

    mtx_type propagationJacobianTranspose[3][3];
    Matrix.Transpose((mtx_type*) propagationJacobian, 3, 3, (mtx_type*) propagationJacobianTranspose);

    mtx_type tmp3[3][3];
    Matrix.Multiply((mtx_type*) propagationJacobian, (mtx_type*) covariance, 3, 3, 3, (mtx_type*) tmp3);
    Matrix.Multiply((mtx_type*) tmp3, (mtx_type*)propagationJacobianTranspose, 3, 3, 3, (mtx_type*)covariance);

    covariance[2][2] += ODOMETRY_TURN_PROCESS_VARIANCE;
}

void Localisation::updateWithLidar(float lidarX, float lidarY, float lidarH, float measuredDistance)
{

    // If there is no wall, return
    if (measuredDistance > 254)
        return;

    // Precalculate sin and cos of heading because its used everywhere
    float cosHeading = cosf(state[2]);
    float sinHeading = sinf(state[2]);

    float relativeX = lidarX + measuredDistance * cosf(lidarH);
    float relativeY = lidarY + measuredDistance * sinf(lidarH);

    float expectedX = state[0] + cosHeading * relativeX - sinHeading * relativeY;
    float expectedY = state[1] + sinHeading * relativeX + cosHeading * relativeY;

    float closestWallX = floor(expectedX / CELL_SIZE) * CELL_SIZE + CELL_SIZE / 2;
    float closestWallY = floor(expectedY / CELL_SIZE) * CELL_SIZE + CELL_SIZE / 2;

    float angleLidar = normaliseTheta(state[2] + lidarH);

    // Serial1.print("angleLidar: ");
    // Serial1.println(angleLidar);

    float sinHeadingPlusLidarH = sinf(state[2] + lidarH);
    float cosHeadingPlusLidarH = cosf(state[2] + lidarH);

    // Serial1.print("sinHeadingPlusLidarH: ");
    // Serial1.println(sinHeadingPlusLidarH);

    mtx_type innovationVector[1][1];
    mtx_type jacobian[1][3];

    if ((fabs(normaliseTheta(angleLidar - DEG2RAD(0))) < DEG2RAD(20)) ||
        (fabs(normaliseTheta(angleLidar - DEG2RAD(180))) < DEG2RAD(20)))
    {
        // do x update

        float lidarGlobalX = state[0] + cosHeading * lidarX - sinHeading * lidarY;  // y position of lidar in global coordinates
        float expectedDistance = fabs((closestWallX - lidarGlobalX) / cosHeadingPlusLidarH);
        innovationVector[0][0] = measuredDistance - expectedDistance;

        jacobian[0][0] = -1.0 / cosHeadingPlusLidarH;
        jacobian[0][1] = 0.0;
        jacobian[0][2] = (lidarY * cosHeading + lidarX * sinHeading) / cosHeadingPlusLidarH + (sinHeadingPlusLidarH * (closestWallX - state[0] - lidarX * cosHeading + lidarY * sinHeading)) / (cosHeadingPlusLidarH * cosHeadingPlusLidarH);

        // Serial1.print("state before: \n"); 
        // Matrix.Print((mtx_type*) state, 3, 1, "");
        // Serial1.print("covariance before: \n"); 
        // Matrix.Print((mtx_type*) covariance, 3, 3, "");
        // Serial1.print("closestWallX: \n"); 
        // Serial1.println(closestWallX);
        // Serial1.print("lidarGlobalX: \n"); 
        // Serial1.println(lidarGlobalX);
        // Serial1.print("expectedDistance: \n"); 
        // Serial1.println(expectedDistance);
        // Serial1.print("innovationVector: \n"); 
        // Matrix.Print((mtx_type*) innovationVector, 1, 1, "");
        // Serial1.print("jacobian: \n"); 
        // Matrix.Print((mtx_type*) jacobian, 1, 3, "");
    }
    else if (fabs(normaliseTheta(angleLidar - DEG2RAD(90))) < DEG2RAD(20) ||
             fabs(normaliseTheta(angleLidar - DEG2RAD(-90))) < DEG2RAD(20))
    {
        // do y update 
        float lidarGlobalY = state[1] + sinHeading * lidarX + cosHeading * lidarY; // y position of lidar in global coordinates
        float expectedDistance = fabs((closestWallY - lidarGlobalY) / sinHeadingPlusLidarH);
        innovationVector[0][0] = measuredDistance - expectedDistance;

        jacobian[0][0] = 0.0;
        jacobian[0][1] = -1.0 / sinHeadingPlusLidarH;
        jacobian[0][2] = (cosHeadingPlusLidarH * (state[1] - closestWallY + lidarY * cosHeading + lidarX * sinHeading))/(sinHeadingPlusLidarH * sinHeadingPlusLidarH) - (lidarX * cosHeading - lidarY * sinHeading) / sinHeadingPlusLidarH;

        // Serial1.print("state before: \n"); 
        // Matrix.Print((mtx_type*) state, 3, 1, "");
        // Serial1.print("covariance before: \n"); 
        // Matrix.Print((mtx_type*) covariance, 3, 3, "");
        // Serial1.print("closestWallY: \n"); 
        // Serial1.println(closestWallY);
        // Serial1.print("lidarGlobalY: \n"); 
        // Serial1.println(lidarGlobalY);
        // Serial1.print("expectedDistance: \n"); 
        // Serial1.println(expectedDistance);
        // Serial1.print("innovationVector: \n"); 
        // Matrix.Print((mtx_type*) innovationVector, 1, 1, "");
        // Serial1.print("jacobian: \n"); 
        // Matrix.Print((mtx_type*) jacobian, 1, 3, "");
    }
    else
    {
        // Could not associate to a wall, don't use it
        return;
    }

    mtx_type jacobianTranspose[3][1];
    Matrix.Transpose((mtx_type*) jacobian, 1, 3, (mtx_type*) jacobianTranspose);

    mtx_type tmp4[1][3];
    Matrix.Multiply((mtx_type*) jacobian, (mtx_type*) covariance, 1, 3, 3, (mtx_type*) tmp4);

    mtx_type tmp5[1][1];
    Matrix.Multiply((mtx_type*) tmp4, (mtx_type*) jacobianTranspose, 1, 3, 1, (mtx_type*) tmp5);
    
    mtx_type innovationCovariance[1][1];
    innovationCovariance[0][0] = tmp5[0][0] + LIDAR_UNCERTAINTY * LIDAR_UNCERTAINTY;

    mtx_type innovationCovarianceInverse[1][1];
    innovationCovarianceInverse[0][0] = 1.0 / innovationCovariance[0][0];  // 1 by 1 matrix inversion is easy! yay!

    mtx_type tmp6[3][1];
    Matrix.Multiply((mtx_type*) covariance, (mtx_type*) jacobianTranspose, 3, 3, 1, (mtx_type*) tmp6);

    mtx_type kalmanGain[3][1];
    Matrix.Multiply((mtx_type*) tmp6, (mtx_type*) innovationCovarianceInverse, 3, 1, 1, (mtx_type*) kalmanGain);

    mtx_type tmp7[3][1];
    Matrix.Multiply((mtx_type*) kalmanGain, (mtx_type*) innovationVector, 3, 1, 1, (mtx_type*) tmp7);

    Matrix.Add((mtx_type*) state, (mtx_type*) tmp7, 3, 1, (mtx_type*) state);

    mtx_type tmp8[3][3];
    Matrix.Multiply((mtx_type*) kalmanGain, (mtx_type*) jacobian, 3, 1, 3, (mtx_type*) tmp8);

    mtx_type tmp9[3][3];
    tmp9[0][0] = 1.0;
    tmp9[0][1] = 0.0;
    tmp9[0][2] = 0.0;
    tmp9[1][0] = 0.0;
    tmp9[1][1] = 1.0;
    tmp9[1][2] = 0.0;
    tmp9[2][0] = 0.0;
    tmp9[2][1] = 0.0;
    tmp9[2][2] = 1.0;

    Matrix.Subtract((mtx_type*) tmp9, (mtx_type*) tmp8, 3, 3, (mtx_type*) tmp9);

    mtx_type tmp10[3][3];
    Matrix.Multiply((mtx_type*) tmp9, (mtx_type*) covariance, 3, 3, 3, (mtx_type*) tmp10);

    for (unsigned i = 0; i < 3; ++i)
    {
        for (unsigned j = 0; j < 3; ++j)
        {
            covariance[i][j] = tmp10[i][j];
        }
    }
}