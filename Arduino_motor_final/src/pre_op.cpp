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
  led.set_hi();
  on_do();
	
}

void PreOpState::on_exit(){
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
  else if(event == 80){
    msg[1] = (uint8_t)function_code + 80;
    msg[4] = (uint8_t)0;
    msg[5] = (uint8_t)3;   //sending code error 3 in data because of no existing transition
    uint16_t crc = ModRTU_CRC(msg, MSG_LEN - 2);
    msg[6] = crc >> 8;
    msg[7] = crc & 0xFF;
    Serial.write(msg, MSG_LEN);
  }
}