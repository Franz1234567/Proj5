#ifndef INIT_H
#define INIT_H

#include "state.h"

class InitState : public State
{
public:
  void on_entry() override;
  void on_do() override;
  void on_exit() override;
  void on_event(int event) override;
};


#endif