
//#include <IRremote.h>

//#define IR_SEND_PIN 4
//#define ENABLE_LED_FEEDBACK  true
//#define USE_DEFAULT_FEEDBACK_LED_PIN true
#define SENDING_REPEATS 0


#define TV_POWER 0x50AFD02F
#define TV_ENERGY 0x20DFA956
#define TV_BACK 0x20DF14EB
#define TV_DOWN 0x20DF827D
#define TV_UP 0x20DF02FD
#define TV_OK 0x20DF22DD
#define TV_VOL_UP 0x50AF40BF
#define TV_VOL_DOWN 0x50AFC03F
#define TV_INPUT_SOURCE 0x20DFD02F

#define LED_POWER 0x00FF02FD
#define LED_BRIGHT_DOWN 0x00FFBA45
#define LED_BRIGHT_UP 0x00FF3AC5
#define LED_DIY_1 0x00FF30CF 
#define LED_DIY_2 0x00FFB04F 
#define LED_DIY_3 0x00FF708F 
#define LED_DIY_4 0x00FF10EF 
#define LED_DIY_5 0x00FF906F 
#define LED_DIY_6 0x00FF50AF 


// Para cambiar el brillo: pulsar el botón energy, delay, pulsar de nuevo (baja a la siguiente opcion), delay, OK, back (salir de energy), back (quita el cuadrado)
// y para subirlo, en vez de pulsar el botón de arriba, pulsar 5 veces el de energy/abajo 

// SETUP
//IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin


void send_message_TV(const char message[]){
  char buffer[75];
  Serial.println("ADDR COMM \t- ENCODED RAW");
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x04, 0x95, irsend.encodeNEC(0x04, 0x95), irsend.encodeNEC(0x04, 0x95)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x04, 0x41, irsend.encodeNEC(0x04, 0x41), irsend.encodeNEC(0x04, 0x41)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x04, 0x40, irsend.encodeNEC(0x04, 0x40), irsend.encodeNEC(0x04, 0x40)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x04, 0x44, irsend.encodeNEC(0x04, 0x44), irsend.encodeNEC(0x04, 0x44)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x04, 0x28, irsend.encodeNEC(0x04, 0x28), irsend.encodeNEC(0x04, 0x28)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x04, 0x0C, irsend.encodeNEC(0x04, 0x0C), irsend.encodeNEC(0x04, 0x0C)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x04, 0x95, irsend.encodeNEC(0x04, 0x95), irsend.encodeNEC(0x04, 0x95)); Serial.println(buffer);//
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x40, irsend.encodeNEC(0x00, 0x40), irsend.encodeNEC(0x00, 0x40)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x5D, irsend.encodeNEC(0x00, 0x5D), irsend.encodeNEC(0x00, 0x5D)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x5C, irsend.encodeNEC(0x00, 0x5C), irsend.encodeNEC(0x00, 0x5C)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x0C, irsend.encodeNEC(0x00, 0x0C), irsend.encodeNEC(0x00, 0x0C)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x0D, irsend.encodeNEC(0x00, 0x0D), irsend.encodeNEC(0x00, 0x0D)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x0E, irsend.encodeNEC(0x00, 0x0E), irsend.encodeNEC(0x00, 0x0E)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x08, irsend.encodeNEC(0x00, 0x08), irsend.encodeNEC(0x00, 0x08)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x09, irsend.encodeNEC(0x00, 0x09), irsend.encodeNEC(0x00, 0x09)); Serial.println(buffer);
  sprintf(buffer, "%02X %02X \t- %08ul o 0x%08X", 0x00, 0x0A, irsend.encodeNEC(0x00, 0x0A), irsend.encodeNEC(0x00, 0x0A)); Serial.println(buffer);

  
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


uint32_t encode_NEC(int addr, int cmd){
  // TODO borrar buffer
  char buffer[20];
  Serial.println("desde encode: ");
  sprintf(buffer, " nec: %X", irsend.encodeNEC(addr, cmd));
  Serial.println(buffer);
  irsend.sendNEC(irsend.encodeNEC(addr, cmd));
  return irsend.encodeNEC(addr, cmd);
}


// apaga TV)
void shutdown_tv(){
  irsend.sendNEC(TV_POWER);    
}

// hacia abajo, baja brillo, sube ahorro energia
void turn_down_bright(){
  irsend.sendNEC(TV_BACK); // atras                         irsend.encodeNEC(0x04, 0x28) // atras  
  delay(30);
  irsend.sendNEC(TV_ENERGY); // energía                     irsend.encodeNEC(0x04, 0x95) // energía  
  delay(1020);
  irsend.sendNEC(TV_DOWN); // abajo                         irsend.encodeNEC(0x04, 0x41) // abajo
  delay(115);
  irsend.sendNEC(TV_OK); // ok                              irsend.encodeNEC(0x04, 0x44) // ok  
  delay(30);
  irsend.sendNEC(TV_OK); // ok                              irsend.encodeNEC(0x04, 0x44) // ok  
  delay(90);
  irsend.sendNEC(TV_BACK); // atras                         irsend.encodeNEC(0x04, 0x28) // atras  
  delay(30);
  irsend.sendNEC(TV_BACK); // atras otra vez                irsend.encodeNEC(0x04, 0x28) // atras otra vez  
  delay(30);
  irsend.sendNEC(TV_BACK); // atras otra vez pa porsi       irsend.encodeNEC(0x04, 0x28) // atras otra vez pa porsi  
  delay(30);
  irsend.sendNEC(LED_DIY_1);// peachy TODO ponerlo fuera    irsend.encodeNEC(0x00, 0x0C) // peachy TODO ponerlo fuera  

}

// hacia arriba, sube brillo, baja ahorro energia
void turn_up_bright(){
  irsend.sendNEC(TV_BACK); // atras                         irsend.encodeNEC(0x04, 0x28) // atras  
  delay(30);
  irsend.sendNEC(TV_ENERGY); // energía                     irsend.encodeNEC(0x04, 0x95) // energía  
  delay(1020);
  irsend.sendNEC(TV_UP); // arriba                          irsend.encodeNEC(0x04, 0x40) // arriba
  delay(115);
  irsend.sendNEC(TV_OK); // ok                              irsend.encodeNEC(0x04, 0x44) // ok  
  delay(30);
  irsend.sendNEC(TV_OK); // ok                              irsend.encodeNEC(0x04, 0x44) // ok  
  delay(90);
  irsend.sendNEC(TV_BACK); // atras                         irsend.encodeNEC(0x04, 0x28) // atras  
  delay(30);
  irsend.sendNEC(TV_BACK); // atras otra vez                irsend.encodeNEC(0x04, 0x28) // atras otra vez  
  delay(30);
  irsend.sendNEC(TV_BACK); // atras otra vez pa porsi       irsend.encodeNEC(0x04, 0x28) // atras otra vez pa porsi  
  delay(30);
  irsend.sendNEC(LED_DIY_1);// peachy TODO ponerlo fuera    irsend.encodeNEC(0x00, 0x0C) // peachy TODO ponerlo fuera  

}
void tv_source() {
  irsend.sendNEC(TV_INPUT_SOURCE);              // irsend.encodeNEC(0x04,0x0B)  // Source
  delay(750);
  irsend.sendNEC(TV_INPUT_SOURCE);              // irsend.encodeNEC(0x04,0x0B)  // Source
  delay(260);
  irsend.sendNEC(TV_OK);   // irsend.encodeNEC(0x04, 0x44)  ok
  delay(360);
  irsend.sendNEC(TV_BACK); // irsend.encodeNEC(0x04, 0x28)  atras
  delay(100);
  irsend.sendNEC(TV_BACK); // irsend.encodeNEC(0x04, 0x28)  atras
  delay(30);
  irsend.sendNEC(LED_DIY_1);// peachy TODO ponerlo fuera    irsend.encodeNEC(0x00, 0x0C) // peachy TODO ponerlo fuera  
}


void led_shut_down(){
  // LED: power off
  irsend.sendNEC(LED_POWER); // irsend.encodeNEC(0x00, 0x40)
}

void led_dim_down(){
  // LED: lower brightness
  irsend.sendNEC(LED_BRIGHT_DOWN); // irsend.encodeNEC(0x00, 0x5D)
}
void led_dim_up(){
  // LED: increase brightness
  irsend.sendNEC(LED_BRIGHT_UP); // irsend.encodeNEC(0x00, 0x5C)
}


void led_diy(int n){
  switch(n){
    case 1:
      // LED DIY 1 (Peachy)
      irsend.sendNEC(LED_DIY_1); // irsend.encodeNEC(0x00, 0x0C)
      break;
    case 2:
      // LED DIY 2 ()
      irsend.sendNEC(LED_DIY_2); // irsend.encodeNEC(0x00, 0x0D)
      break;
    case 3:
      // LED DIY 3 ()
      irsend.sendNEC(LED_DIY_3); // irsend.encodeNEC(0x00, 0x0E)
      break;
    case 4:
      // LED DIY 4 ()
      irsend.sendNEC(LED_DIY_4); // irsend.encodeNEC(0x00, 0x08)
      break;
    case 5:
      // LED DIY 5 ()
      irsend.sendNEC(LED_DIY_5); // irsend.encodeNEC(0x00, 0x09)
      break;
    case 6:
      // LED DIY 6 ()
      irsend.sendNEC(LED_DIY_6); // irsend.encodeNEC(0x00, 0x0A)
      break;
    default:
      // LED DIY 1 (Peachy)
      irsend.sendNEC(LED_DIY_1); // irsend.encodeNEC(0x00, 0x0C)
  }
}
