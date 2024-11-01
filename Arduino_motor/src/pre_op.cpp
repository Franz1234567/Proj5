#include "global.h"

bool advanced_control;

void PreOpState::on_entry(){
  msg[4] = 0;
	msg[5] = function_code;
	uint16_t crc = ModRTU_CRC(msg, MSG_LEN - 2);
	crc1 = crc >> 8;
	crc2 = crc & 0xFF;
	msg[6] = crc1;
	msg[7] = crc2;
  
  Serial.write(msg, MSG_LEN);
	// Serial.println("Entering Pre Op State");
  led.set_hi();
  // Serial.println("Press b for basic proportional control or a for advanced pi control");
  on_do();
	
}

void PreOpState::on_exit(){
	// Serial.println("Exiting Pre Op State");
}

void PreOpState::on_do(){
  advanced_control = true;
}


void PreOpState::on_event(int event){
  if (event == 1){
	this->context_->transition_to(new OperationalState);
  }
  else if (event == 2){
    this->context_->transition_to(new StoppedState);
  }
  else if (event == 81){
    this->context_->transition_to(new InitState);
  }
}