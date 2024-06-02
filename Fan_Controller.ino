//#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Arduino.h>

#define FAN_HIGH 0xC01
#define FAN_MED 0xC04
#define FAN_LOW 0xC43
#define FAN_OFF 0xC10
#define FAN_LIGHT 0xC08
#define FAN_2H 0xC20
#define FAN_4H 0xC46
#define FAN_8H 0xC02

#define SYMPHONY_PREAMBLE 0xC00
#define SYMPHONY_HEADER 0xC7F

#define IR_LED_PIN 32  

IRsend irsend(IR_LED_PIN);

//  Send the Fan IR command
void send_cmd(unsigned long command){
  irsend.sendSymphony(SYMPHONY_PREAMBLE,12,1);
  irsend.sendSymphony(SYMPHONY_HEADER,12,1);
  irsend.sendSymphony(command,12, 3);
}

void speed_high(){
  send_cmd(FAN_HIGH);
}
void speed_med(){
  send_cmd(FAN_MED);
}
void speed_low(){
  send_cmd(FAN_LOW);
}
void speed_off(){
  send_cmd(FAN_OFF);
}

void toggle_light() {
  send_cmd(FAN_LIGHT);
}

void time_2h() {
  send_cmd(FAN_2H);
}
void time_4h() {
  send_cmd(FAN_4H);
}
void time_8h() {
  send_cmd(FAN_8H);
}

void shut_down() {
  send_cmd(FAN_OFF);
  send_cmd(FAN_LIGHT);
}

void test_light() {
  send_cmd(FAN_LIGHT);
  delay(210);
  send_cmd(FAN_LIGHT);
}
