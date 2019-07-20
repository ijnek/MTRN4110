#ifndef LOCALISATION_H
#define LOCALISATION_H

#include "../Blackboard/Blackboard.h"

class Localisation
{
  public:
    Localisation(Blackboard &blackboard)
      : blackboard(blackboard)
    {

    }

    void tick();

  private:
    Blackboard &blackboard;
};


#endif // LOCALISATION_H