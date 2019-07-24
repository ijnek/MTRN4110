#include "Planning.h"
#include "../Constants/Constants.h"
#include "../MovementRequest/MovementRequest.h"
#include "../MathUtil/MathUtil.h"
#include "PlanNone.h"
#include "Plan1.h"
#include "Plan2.h"
#include "Plan3.h"
#include "Plan4.h"
#include "PlanTurnLeft.h"
#include "PlanTurnRight.h"
#include "PlanForwards.h"

Planning::Planning(Blackboard &blackboard)
    : blackboard(blackboard), plans(plansArray)
{
    // Add plans. Note, this must be in same order as listed in Constants.h
    plans.push_back(new PlanNone());
    plans.push_back(new Plan1());
    plans.push_back(new Plan2());
    plans.push_back(new Plan3());
    plans.push_back(new Plan4());
    plans.push_back(new PlanTurnLeft());
    plans.push_back(new PlanTurnRight());
    plans.push_back(new PlanForwards());
}

void Planning::tick()
{
    // If the plan has changed, then make sure reset is called on the new one
    if (blackboard.plan != prevPlan)
    {
        plans[blackboard.plan]->reset();
        blackboard.commandCompleted = false;
    }

    // Once we're done with a plan, reset plan to None on the blackboard
    if (plans[blackboard.plan]->done())
    {
        blackboard.plan = PLAN_NONE;
        blackboard.commandCompleted = true;
    }

    // Call getMovementRequest on appropriate plan, and write to blackboard
    blackboard.movementRequest = plans[blackboard.plan]->getMovementRequest(
        blackboard.worldPose.x,
        blackboard.worldPose.y,
        blackboard.worldPose.theta);

    prevPlan = blackboard.plan;   // update previous plan
}