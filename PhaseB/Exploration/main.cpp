#include "Exploration.h"
#include "Blackboard.h"
#include <unistd.h>

Blackboard blackboard;
Exploration exploration(blackboard);
SensingSimulation sensingSimulation(blackboard);

int main (int argc, char *argv[])
{
    // Always assume we start at (8, 0, SOUTH)
    blackboard.x = EXPLORATION_STARTING_X;
    blackboard.y = EXPLORATION_STARTING_Y;
    blackboard.facingDirection = EXPLORATION_STARTING_FACING_DIRECTION;
    while (true)
    {
        sensingSimulation.tick();
        exploration.tick();
        usleep(200000);
    }
}