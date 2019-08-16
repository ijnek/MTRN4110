#include "Behaviour.h"
#include "../Constants/NetworkingConstants.h"
#include "../Types/MovementRequest.h"
#include "../Utils/MathUtil/MathUtil.h"
#include "BehaviourNone.h"
#include "BehaviourTurnLeft.h"
#include "BehaviourTurnRight.h"
#include "BehaviourForwards.h"
#include "BehaviourNextWayPoint.h"
#include "BehaviourPlannedRoute.h"

Behaviour::Behaviour(Blackboard &blackboard)
    : blackboard(blackboard), plans(plansArray)
{
    addPlans();
}

void Behaviour::reset()
{
    for (unsigned i = 0; i < plans.size(); ++i)
    {
        delete plans[i];
    }
    plans.clear();
    addPlans();
}

void Behaviour::addPlans()
{
    // Add plans. Note, this must be in same order as listed in NetworkingConstants.h
    plans.push_back(new BehaviourNone());
    plans.push_back(new BehaviourTurnLeft());
    plans.push_back(new BehaviourTurnRight());
    plans.push_back(new BehaviourForwards());
    plans.push_back(new BehaviourNextWayPoint());
    plans.push_back(new BehaviourPlannedRoute());  

}

void Behaviour::tick()
{
    // If the plan has changed, then make sure reset is called on the new one
    if (blackboard.plan != prevBehaviour)
    {
        plans[blackboard.plan]->reset();
        blackboard.commandCompleted = false;

        // make sure we read the waypoint from blackboard if we're using the nextWayPoint plan (HACK ALERT :p)
        if (blackboard.plan == BEHAVIOUR_NEXT_WAYPOINT)
        {
            reinterpret_cast<WayPointBehaviour*>(plans[blackboard.plan])->addWayPoint(blackboard.nextWayPoint);
        }

        if (blackboard.plan == BEHAVIOUR_PLANNED_ROUTE)
        {
            for (unsigned i = 0; i < blackboard.plannedWayPoints.size(); ++i)
            {
                reinterpret_cast<WayPointBehaviour*>(plans[blackboard.plan])->addWayPoint(blackboard.plannedWayPoints[i]);
            }
        }
    }

    // Once we're done with a plan, reset plan to None on the blackboard
    if (plans[blackboard.plan]->done())
    {
        // If we're performing a planned route, we're in a speed run. If we finish it, then notify the leds that we're done
        if (blackboard.plan == BEHAVIOUR_PLANNED_ROUTE)
        {
            blackboard.reachedGoal = true;
        }

        blackboard.plan = BEHAVIOUR_NONE;
        blackboard.commandCompleted = true;
    }

    // Call getMovementRequest on appropriate plan, and write to blackboard
    blackboard.movementRequest = plans[blackboard.plan]->getMovementRequest(
        blackboard.worldPose.x,
        blackboard.worldPose.y,
        blackboard.worldPose.theta);

    prevBehaviour = blackboard.plan;   // update previous plan
}