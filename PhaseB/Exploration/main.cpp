#include "Exploration.h"
#include "Blackboard.h"
#include <unistd.h>

Blackboard blackboard;
Exploration exploration(blackboard);
SensingSimulation sensingSimulation(blackboard);

int main (int argc, char *argv[])
{
    // Always assume we start at (8, 0, SOUTH)
    blackboard.x = 8;
    blackboard.y = 0;
    blackboard.facingDirection = SOUTH;
    while (true)
    {
        sensingSimulation.tick();
        exploration.tick();
        usleep(200000);
    }
}