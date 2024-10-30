#include "global.h"


void InitState::on_entry(){
	msg[5] = function_code;
	Serial.write(msg, MSG_LEN);
	// Serial.println("Entering Init State");
	on_do();
}

void InitState::on_exit(){
	// Serial.println("Exiting Init State");
}

void InitState::on_do(){ // Initialization of the variables

	max_speed = 2800; 
	current_speed = 0;
	ref = 2000; // reference speed
	duty_cycle_first = 50; //original duty cycle

	last_state_A = encA.is_low();
	last_state_B = encB.is_low();
	
	led.init();
	led.set_lo();
	analog.pin_digi.set_lo();
	fault.init();
	// Serial.println("Variables initialized !");
	this->context_->transition_to(new PreOpState);
}

void InitState::on_event(int event)
{
  if (event == 1){ // We kept a manual way to go there but it should be automatic
	this->context_->transition_to(new OperationalState);
  }
}