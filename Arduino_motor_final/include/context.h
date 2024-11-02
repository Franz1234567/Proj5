#ifndef CONTEXT_H
#define CONTEXT_H

#include "state.h"

class Context
{
private:
  State *state_;

public:
  Context(State *state);
  ~Context();

  void transition_to(State *state);
  void do_work();
  void event(int event);
};

#endif