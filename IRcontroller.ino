
//#include <IRremote.h>

//#define IR_SEND_PIN 4
//#define ENABLE_LED_FEEDBACK  true
//#define USE_DEFAULT_FEEDBACK_LED_PIN true
#define SENDING_REPEATS 0


#define TV_VOL_DOWN "0x04000300"
#define TV_VOL_UP "0x04000200"
#define TV_SOURCE "0x040B0000"
#define TV_ENERGY "0x04009500"

// Para cambiar el brillo: pulsar el botón energy, delay, pulsar de nuevo (baja a la siguiente opcion), delay, OK, back (salir de energy), back (quita el cuadrado)
// y para subirlo, en vez de pulsar el botón de arriba, pulsar 5 veces el de energy/abajo 

// SETUP
//IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin


void send_message_TV(const char message[]){
  char buffer[75];
  Serial.println("ADDR COMM \t- ENCODED RAW");
  //sprintf(buffer, "%x %x \t- %ul", 0x04, 0x95, irsend.encodeNEC(0x04, 0x95)); Serial.println(buffer);
  //sprintf(buffer, "%x %x \t- %ul o %x", 0x04, 0x95, irsend.encodeNEC(0x04, 0x95), irsend.encodeNEC(0x04, 0x95)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x04, 0x95, irsend.encodeNEC(0x04, 0x95), irsend.encodeNEC(0x04, 0x95)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x04, 0x41, irsend.encodeNEC(0x04, 0x41), irsend.encodeNEC(0x04, 0x41)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x04, 0x40, irsend.encodeNEC(0x04, 0x40), irsend.encodeNEC(0x04, 0x40)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x04, 0x44, irsend.encodeNEC(0x04, 0x44), irsend.encodeNEC(0x04, 0x44)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x04, 0x28, irsend.encodeNEC(0x04, 0x28), irsend.encodeNEC(0x04, 0x28)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x04, 0x0C, irsend.encodeNEC(0x04, 0x0C), irsend.encodeNEC(0x04, 0x0C)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x04, 0x95, irsend.encodeNEC(0x04, 0x95), irsend.encodeNEC(0x04, 0x95)); Serial.println(buffer);//
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x40, irsend.encodeNEC(0x00, 0x40), irsend.encodeNEC(0x00, 0x40)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x5D, irsend.encodeNEC(0x00, 0x5D), irsend.encodeNEC(0x00, 0x5D)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x5C, irsend.encodeNEC(0x00, 0x5C), irsend.encodeNEC(0x00, 0x5C)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x0C, irsend.encodeNEC(0x00, 0x0C), irsend.encodeNEC(0x00, 0x0C)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x0D, irsend.encodeNEC(0x00, 0x0D), irsend.encodeNEC(0x00, 0x0D)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x0E, irsend.encodeNEC(0x00, 0x0E), irsend.encodeNEC(0x00, 0x0E)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x08, irsend.encodeNEC(0x00, 0x08), irsend.encodeNEC(0x00, 0x08)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x09, irsend.encodeNEC(0x00, 0x09), irsend.encodeNEC(0x00, 0x09)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %ul o %X", 0x00, 0x0A, irsend.encodeNEC(0x00, 0x0A), irsend.encodeNEC(0x00, 0x0A)); Serial.println(buffer);

  
  //unsigned longs are 32 bits so max number is 4294967295 = FFFFFFFF
  unsigned long code = strtoul(message, NULL, 16);
  Serial.print("Sending code:");
  Serial.println(message);
   
  //IrSender.sendNECRaw(code, SENDING_REPEATS);  // Arduino-IRremote 
  uint64_t vol_down_code = 0x04000300;
  //irsend.sendNEC(irsend.encodeNEC(0x04, 0x03));  // IRremoteESP8266
  delay(350);
  //irsend.sendNEC(0x04000300);  // IRremoteESP8266
}


// apaga brillo (apaga TV)
void shutdown_tv(){
  irsend.sendNEC(irsend.encodeNEC(0x04, 0x08));
}

// hacia abajo, baja brillo, sube ahorro energia
void turn_down_bright(){
  irsend.sendNEC(0x20DF14EB); // atras                        irsend.encodeNEC(0x04, 0x28) // atras  
  delay(20);
  irsend.sendNEC(0x20DFA956); // energía                      irsend.encodeNEC(0x04, 0x95) // energía  
  delay(1015);
  irsend.sendNEC(0x20DF827D); // abajo                        irsend.encodeNEC(0x04, 0x41) // abajo
  delay(105);
  irsend.sendNEC(0x20DF22DD); // ok                           irsend.encodeNEC(0x04, 0x44) // ok  
  delay(20);
  irsend.sendNEC(0x20DF22DD); // ok                           irsend.encodeNEC(0x04, 0x44) // ok  
  delay(80);
  irsend.sendNEC(0x20DF14EB); // atras                        irsend.encodeNEC(0x04, 0x28) // atras  
  delay(20);
  irsend.sendNEC(0x20DF14EB); // atras otra vez               irsend.encodeNEC(0x04, 0x28) // atras otra vez  
  delay(20);
  irsend.sendNEC(0x20DF14EB); // atras otra vez pa porsi      irsend.encodeNEC(0x04, 0x28) // atras otra vez pa porsi  
  delay(20);
  irsend.sendNEC(0xFF30CF);// peachy TODO ponerlo fuera       irsend.encodeNEC(0x00, 0x0C) // peachy TODO ponerlo fuera  

}


// hacia arriba, sube brillo, baja ahorro energia
void turn_up_bright(){
  irsend.sendNEC(0x20DF14EB); // atras                        irsend.encodeNEC(0x04, 0x28) // atras  
  delay(20);
  irsend.sendNEC(0x20DFA956); // energía                      irsend.encodeNEC(0x04, 0x95) // energía  
  delay(1015);
  irsend.sendNEC(0x20DF02FD); // arriba                       irsend.encodeNEC(0x04, 0x40) // arriba
  delay(105);
  irsend.sendNEC(0x20DF22DD); // ok                           irsend.encodeNEC(0x04, 0x44) // ok  
  delay(20);
  irsend.sendNEC(0x20DF22DD); // ok                           irsend.encodeNEC(0x04, 0x44) // ok  
  delay(80);
  irsend.sendNEC(0x20DF14EB); // atras                        irsend.encodeNEC(0x04, 0x28) // atras  
  delay(20);
  irsend.sendNEC(0x20DF14EB); // atras otra vez               irsend.encodeNEC(0x04, 0x28) // atras otra vez  
  delay(20);
  irsend.sendNEC(0x20DF14EB); // atras otra vez pa porsi      irsend.encodeNEC(0x04, 0x28) // atras otra vez pa porsi  
  delay(20);
  irsend.sendNEC(0xFF30CF);// peachy TODO ponerlo fuera       irsend.encodeNEC(0x00, 0x0C) // peachy TODO ponerlo fuera  

}

// TODO precalcular los valores, para así evitar usar encodeNEC

void led_shut_down(){
  // LED: power off
  irsend.sendNEC(irsend.encodeNEC(0x00, 0x40));
}

void led_dim_down(){
  // LED: lower brightness
  irsend.sendNEC(irsend.encodeNEC(0x00, 0x5D));
}
void led_dim_up(){
  // LED: increase brightness
  irsend.sendNEC(irsend.encodeNEC(0x00, 0x5C));
}


void led_diy(int n){
  switch(n){
    case 1:
      // LED DIY 1 (Peachy)
      irsend.sendNEC(irsend.encodeNEC(0x00, 0x0C));
      break;
    case 2:
      // LED DIY 2 ()
      irsend.sendNEC(irsend.encodeNEC(0x00, 0x0D));
      break;
    case 3:
      // LED DIY 3 ()
      irsend.sendNEC(irsend.encodeNEC(0x00, 0x0E));
      break;
    case 4:
      // LED DIY 4 ()
      irsend.sendNEC(irsend.encodeNEC(0x00, 0x08));
      break;
    case 5:
      // LED DIY 5 ()
      irsend.sendNEC(irsend.encodeNEC(0x00, 0x09));
      break;
    case 6:
      // LED DIY 6 ()
      irsend.sendNEC(irsend.encodeNEC(0x00, 0x0A));
      break;
    default:
      // LED DIY 1 (Peachy)
      irsend.sendNEC(irsend.encodeNEC(0x00, 0x0C));
  }
}
