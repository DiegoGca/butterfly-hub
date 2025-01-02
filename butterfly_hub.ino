// Chip is ESP32-D0WD-V3 (revision v3.1)
// ESP32-WROOM-32
// ESP32 Dev Module  // con este no luce el LED azul del wifi
// la biblioteca de placas ESP32 Arduino tiene que estar en la version 2.0.17 (downgrade) https://github.com/crankyoldgit/IRremoteESP8266/issues/2039
/*
 * Programa para hacer ciertas acciones mediante IR con acciones de Alexa
 * A) Manejar el ventilador (3 velocidades + TEMPORIZADOR + apagar + Luz)
 * B) TV: encender/apagar, subir/bajar volumen, modo eco
 * Requirements (ArduinoJson, UniversalTelegramBot, xoseperez_fauxmoesp, AsyncTCP, IRemote  )
 * TODO:
 * 1 Conectarse a las redes (static IP, reconectar, ifdef solo con una red)
 * 2 Enviar Telegram (tambien controlar por telegram?) y solo admin (mi chat_id). Setear comandos automaticamente
 * 3 Ordenes Alexa [Se pueden poner "apodos" y ordenes concreta haciendo uso de las rutinas de alexa]
 * 4 Envíar IR. Escanear primero con el receptor y luego hardcodearlo. NECext is just NEC where the parity bits have been replaced with more data
 * 5 OTA? (si ya por USB es bastante lento...)
 * 6 refactor para seperar el wifi (y poner redes individualmente), telegram etc en ficheros individuales y sin secrets
 * 7 optimizar: F(), uint8_t, ifdefs DEBUGS
 * 8 bluetooth para LEDS del techo... se necesita hacer ingenieria inversa. Al menos conectarlo y luego enviar IR, porque de primeras al mando nunca responde
 * 9 modo Debug que se active por telegram
 * 10 limitar el tiempo de refresco https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/SetMyCommands/SetMyCommands.ino
 * 11 control de telegram con ignorecase en los commandos, y quitarlo de los números
 */

/* Ventilador: informe de hora de apagado programado
*/

//#include <WiFi.h>
#include <WiFiMulti.h>
#include <esp_wifi.h> 
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>
#include <fauxmoESP.h>
#include "arduino_secrets.h"
//#include <IRremote.hpp> // ? no se cual de los 2
//#include <IRremote.h>


#define LED_GPIO 32

#define OP_FAN_OFF 1000 // apagar ventilador
#define OP_FAN_LOW 1001 // ventilador low
#define OP_FAN_MDM 1002 // ventilador medium
#define OP_FAN_HGH 1003 // ventilador high

#define OP_TECHO_OFF 2000 // techo off
#define OP_TECHO_ON 2001 // techo on 

#define OP_TELE_OFF 3000 // tele off/on
#define OP_TELE_UP 3001 // subir brillo
#define OP_TELE_DOWN 3002 // bajar brillo

#define OP_LEDS_OFF 4000 // LEDS off/on
#define OP_LEDS_UP 4001 // subir brillo LEDS
#define OP_LEDS_DOWN 4002 // bajar brillo LEDS
#define OP_LEDS_DIY 4003  // DIV LED 1-6 -> 4003-4008



int operation = 0;
fauxmoESP fauxmo;
WiFiClientSecure secure_client;

UniversalTelegramBot bot(BOT_TOKEN, secure_client);


#define BOT_MTBS 870 // time between scan messages
unsigned long bot_lasttime; // last time messages' scan has been done


void wifiSetup() {
  WiFi.hostname(ESP_HOSTNAME);
  // ESP32 Board add-on after version > 1.0.5
  uint8_t newMACAddress[] = {0xE8, 0x6B, 0xEA, 0xE3, 0x1B, 0xBE};
  // Original MAC E8:6B:EA:E0:0B:BC
  esp_wifi_set_mac(WIFI_IF_STA, newMACAddress);
  
  
  WiFiMulti wifiMulti;
  wifiMulti.addAP(WIFI_SSID, WIFI_PSSW);

  esp_wifi_set_mac(WIFI_IF_STA, newMACAddress);
  // WiFi.scanNetworks will return the number of networks found
  Serial.println("Scanning networks");
  int n = WiFi.scanNetworks();
  if (n == 0) {
      Serial.println("no networks found");
  } 
  else {
    Serial.print(n);
    Serial.println(" networks found ");
    Serial.println("==================");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i)+" ("+WiFi.RSSI(i)+")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?"º":" ");
      delay(10);
    }
  }
  // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
  Serial.println("Connecting Wifi...");
  esp_wifi_set_mac(WIFI_IF_STA, newMACAddress);
  if(wifiMulti.run(10000) == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print(WiFi.SSID());
    Serial.print(" ("+WiFi.BSSIDstr()+")");
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: " );
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS 1: ");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS 2: ");
    Serial.println(WiFi.dnsIP(1));
    Serial.print("MAC Address:  ");
    Serial.println(WiFi.macAddress());
  }
}

void handle_operation() {
  switch (operation) {
    case OP_FAN_OFF:
      // 
      speed_off();
      bot.sendMessage(CHAT_ID, "Se ha apagado el ventilador FAN OFF", "");
      break;
    case OP_FAN_LOW:
      // 
      speed_low();
      bot.sendMessage(CHAT_ID, "Se ha encendido el ventilador en modo LOW ", "");
      break;
    case OP_FAN_MDM:
      // 
      bot.sendMessage(CHAT_ID, "Se ha puesto el ventilador en modo MED", "");
      speed_med();
      break;
    case OP_FAN_HGH:
      // 
      bot.sendMessage(CHAT_ID, "Se ha puesto el ventilador en modo HIGH", "");
      speed_high();
      break;
    case OP_TECHO_ON:
      // 
      bot.sendMessage(CHAT_ID, "Se ha encendido el techo", "");
      toggle_light();
      break;
    case OP_TECHO_OFF:
      // 
      bot.sendMessage(CHAT_ID, "Se ha apagado el techo", "");
      toggle_light();
      break;
      
    case OP_TELE_OFF:
      // 
      bot.sendMessage(CHAT_ID, "Se ha apagado la tele", "");
      shutdown_tv();
      break;
    case OP_TELE_UP:
      // 
      bot.sendMessage(CHAT_ID, "Se ha bajado el brillo de la tele", "");
      turn_up_bright();
      break;
    case OP_TELE_DOWN:
      // 
      bot.sendMessage(CHAT_ID, "Se ha bajado el brillo de la tele", "");
      turn_down_bright();
      break;

    case OP_LEDS_DIY:
    case OP_LEDS_DIY+1:
    case OP_LEDS_DIY+2:
    case OP_LEDS_DIY+3:
    case OP_LEDS_DIY+4:
    case OP_LEDS_DIY+5:
      //
      int diy_number = 0;
      diy_number = operation - OP_LEDS_DIY +1;
      Serial.print("LEDS DIY: ");
      Serial.println(diy_number);
      bot.sendMessage(CHAT_ID, " Se ha puesto el color de LEDS: ", "");
      led_diy(diy_number);
      break;

  }
  operation = 0;
}

void setup() {
  // initialize digital pin as an output.
  pinMode(LED_GPIO, OUTPUT);
  Serial.begin(115200);
  Serial.println("Inicio");

  // wifi
  wifiSetup();

  // telegram
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secure_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 

  setup_telegram_controller();

  //fauxmoESP fauxmo;
    
  // By default, fauxmoESP creates it's own webserver on the defined port
  // The TCP port must be 80 for gen3 devices (default is 1901)
  // This has to be done before the call to enable()
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
   fauxmo.enable(true);
  // You can use different ways to invoke alexa to modify the devices state:
  // "Alexa, turn lamp two on"


  // Add virtual devices
  fauxmo.addDevice(DEVICE_VENTILADOR); // quizás unir el venitlador y el techo?
  fauxmo.addDevice(DEVICE_TECHO);
  fauxmo.addDevice(DEVICE_TELE);
  // alexa allows up to 3 devices sharing the same IP address.
  //fauxmo.addDevice(DEVICE_LEDS);
 

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Callback when a command from Alexa is received. 
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // <state> is a boolean (ON/OFF) and <value> a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.
        
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if (strcmp(device_name, DEVICE_VENTILADOR) == 0) {
      // this just sets a variable that the main loop() does something about
      if (state == true) {
        switch (value) {
          case 0:
            operation = OP_FAN_OFF;    // apagar ventilador
            Serial.println("switch fan 0, apagar");
            break;
          case 4:
            operation = OP_FAN_LOW;    // ventilador low
            Serial.println("switch fan 4: ");
            break;
          case 6:
            operation = OP_FAN_MDM;    // ventilador medium
            Serial.println("switch fan 6: ");
            break;
          case 9:
            operation = OP_FAN_HGH;    // ventilador high
            Serial.println("switch fan 9: ");
            break;
          case 254:
          case 255:
            operation = OP_FAN_LOW;
            // you can instruct the library to report the new state to Alexa on next request:
            fauxmo.setState(DEVICE_VENTILADOR, true, 254);
            Serial.println("switch fan 254/255 ");
            break;
          default:
            operation = OP_FAN_LOW;
            Serial.println("switch fan default (LOW)");
            break;
        }
      } else {
        operation = OP_FAN_OFF;    // ventilador OFF
        fauxmo.setState(DEVICE_VENTILADOR, false, 0);
        Serial.println("switch fan OFF ");
      }
    }
    else if (strcmp(device_name, DEVICE_TECHO) == 0) {
      if (state == true) {
        fauxmo.setState(DEVICE_TECHO, true, 254);
        operation = OP_TECHO_ON;     // encender luz techo
      } else {
        fauxmo.setState(DEVICE_TECHO, false, 0);
        operation = OP_TECHO_OFF;    // apagar luz techo
      }
    }
    
    else if (strcmp(device_name, DEVICE_TELE) == 0) {
      if (state == true) {
        Serial.print("valor brillo: ");
        Serial.println(value);
        if (value < 127){
          operation = OP_TELE_DOWN;  // bajar brillo
          Serial.println("switch tele < 127: ");
        } else if (value >= 127){
          operation = OP_TELE_UP;    // subir brillo
            Serial.println("switch tele >= 127: ");
        }        
        fauxmo.setState(DEVICE_TELE, true, 127); // dejar este valor como valor central
      
      } else if (state == false){
        fauxmo.setState(DEVICE_TELE, false, 0);
        operation = OP_TELE_OFF;    // apagar tele
      }
    }
    
    else if (strcmp(device_name, DEVICE_LEDS) == 0) {
      if (state == true) {
        Serial.print("valor LEDS: ");
        Serial.println(value);
        switch(value){
          case 4:
            //
            operation = OP_LEDS_DIY + 0;
            break;
          case 6:
            //
            operation = OP_LEDS_DIY + 1;
            break;
          case 9:
            //
            operation = OP_LEDS_DIY + 2;
            break;
          case 11:
            //
            operation = OP_LEDS_DIY + 3;
            break;
          case 14:
            //
            operation = OP_LEDS_DIY + 4;
            break;
          case 16:
            //
            operation = OP_LEDS_DIY + 5;
            break;
          default:
            if (value < 127){
              operation = OP_LEDS_DOWN;  // bajar brillo LEDS
              Serial.println("switch LEDS < 127: ");
            } else if (value >= 127){
              operation = OP_LEDS_UP;    // subir brillo LEDS
                Serial.println("switch LEDS >= 127: ");
            }
        }        
        fauxmo.setState(DEVICE_LEDS, true, 127); // dejar este valor como valor central
      
      } else if (state == false){
        fauxmo.setState(DEVICE_LEDS, false, 0);
        operation = OP_LEDS_OFF;    // apagar tele
      }
    }
    //Serial.println(operation);
  });

}

// the loop function runs over and over again forever
void loop() {

  if (millis() - bot_lasttime >= BOT_MTBS) {

    if (WiFi.status() == WL_CONNECTED) {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

      while (numNewMessages > 0) {
        for (int i = 0; i < numNewMessages; i++) {
          //String chat_id = String(bot.messages[i].chat_id);
          String text = bot.messages[i].text;
          // TODO parsear telegram
          numNewMessages = telegram_control(text, bot.messages[i].chat_id, numNewMessages);
          }
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      // fauxmoESP uses an async TCP server but a sync UDP server
      // Therefore, we have to manually poll for UDP packets
      fauxmo.handle();
      
      Serial.println(operation);
      if (operation != 0) {
        Serial.println("Recibido operacion: ");
        Serial.println(operation);
        //bot.sendMessage(CHAT_ID, "Se ha recibido una operacion", "");     
        // procesar operation
        handle_operation();
        delay(130);
      }

      bot_lasttime = millis();
    } else {
      // Wifi not connected:
      wifiSetup();
    }
  }
}
