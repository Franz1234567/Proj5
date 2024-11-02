#ifndef STATE_H
#define STATE_H

class Context; // Forward declaration to allow the definition of a pointer to Context without compile error

class State
{
protected:
  Context *context_;

public:
  virtual ~State(); // Virtual destructor

  void set_context(Context *context); // Sets the context

  virtual void on_do() = 0; 
  virtual void on_entry() = 0; 
  virtual void on_exit() = 0; 
  virtual void on_event(int event) = 0;
};

#endif