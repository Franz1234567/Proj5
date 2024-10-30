#include "global.h"

bool advanced_control;

void PreOpState::on_entry(){
  msg[5] = function_code;
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
  // bool kp_received = false;
  // bool ti_received = false;
  // bool control_received = false;
  // advanced_control = false;
  // String Kp_str;
  // String Ti_str;

  // unsigned long previousMillis = 0; // Stores last time LED was updated
  // const long interval = 500; // Interval for LED toggle (500 ms)

  // while(1){
  //   unsigned long currentMillis = millis();

  //   if (currentMillis - previousMillis >= interval) { // every 500 ms
  //       previousMillis = currentMillis;
  //       led.toggle();
  //   }

  //   if (Serial.available() > 0) {
  //     char receivedChar = Serial.read();  // Read the next available character
      
  //     if (receivedChar == 'r' || receivedChar == 'o' || receivedChar == 's') {
  //       if (receivedChar == 'o'){
  //       this->context_->transition_to(new OperationalState);
  //       }
  //       else if (receivedChar == 's'){
  //         this->context_->transition_to(new StoppedState);
  //       }
  //       else if (receivedChar == 'r'){
  //         this->context_->transition_to(new InitState);
  //       }
  //       break;
  //     }

  //     if (control_received) {
  //       // Handle input for Kp
  //       if (!kp_received) {
  //         if (receivedChar == '\n') {  // Newline indicates input is complete
  //           if (Kp_str.length() > 0) {
  //             Kp = Kp_str.toDouble();
  //             kp_received = true;
  //             Serial.print("Kp received: ");
  //             Serial.println(Kp, 5);
  //             Kp_str = "";
  //           }
  //         }
  //         else{
  //           Kp_str += receivedChar; 
  //         }
  //       }

  //       if (advanced_control){
  //         // Handle input for Ti
  //         if (!ti_received && kp_received) {
  //           if (receivedChar == '\n') {  // Newline indicates input is complete
  //             if (Ti_str.length() > 0) {
  //                 Ti = Ti_str.toDouble(); 
  //                 ti_received = true;
  //                 Serial.print("Ti received: ");
  //                 Serial.println(Ti, 5);
  //                 Ti_str = ""; 
  //             }
  //           }
  //           else {
  //           Ti_str += receivedChar;  // Add received character to the Ti string
  //           }
  //         }
  //       }
  //     }

  //     if (!control_received){
  //       if (receivedChar == 'b') {
  //         advanced_control = false;
  //         control_received = true;
  //         Serial.println("Type Kp then press enter");
  //       }
  //       else if (receivedChar == 'a') {
  //         advanced_control = true;
  //         control_received = true;
  //         Serial.println("Type Kp, press enter then type Ti, press enter");
  //       }
  //     }
  //   }
  // }
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