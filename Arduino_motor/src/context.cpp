#include "context.h"

Context::Context(State *state) : state_(nullptr){
  this->transition_to(state);
}

Context::~Context(){
  delete state_;
}

void Context::transition_to(State *state){
  if (this->state_ != nullptr) {
    this->state_->on_exit();
    delete this->state_;
  }

  this->state_ = state; // Assign new state to the context
  this->state_->set_context(this); // Make sure the new state knows which context it belongs to
  this->state_->on_entry(); // Perform initialization required by the new state when entering it
}

void Context::do_work(){
  if (state_) {
    this->state_->on_do(); // Delegate work to the current state
  }
}

void Context::event(int event){
  if (state_) {
    this->state_->on_event(event); // Delegate event handling to the current state
  }
}