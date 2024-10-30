#include "state.h"
#include "context.h"

// Virtual destructor definition
State::~State()
{
}

void State::set_context(Context *context)
{
  this->context_ = context;
}