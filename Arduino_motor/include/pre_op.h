#ifndef PRE_OP_H
#define PRE_OP_H

#include "state.h"

class PreOpState : public State
{
public:
  void on_entry() override;
  void on_do() override;
  void on_exit() override;
  void on_event(int event) override;
};


#endif