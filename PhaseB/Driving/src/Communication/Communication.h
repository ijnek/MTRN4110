#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../Blackboard/Blackboard.h"

class Communication
{
  public:
    Communication(Blackboard &blackboard)
      : blackboard(blackboard)
    {
    }

    void tick();

  private:
    Blackboard &blackboard;
};


#endif // COMMUNICATION_H