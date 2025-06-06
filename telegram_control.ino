
const String COMMANDS = F("["
                          "{\"command\":\"off\", \"description\":\"Fan OFF\"},"
                          "{\"command\":\"low\", \"description\":\"Fan speed: low\"},"
                          "{\"command\":\"med\", \"description\":\"Fan speed: medium\"},"
                          "{\"command\":\"high\", \"description\":\"Fan speed: high\"},"
                          "{\"command\":\"light\", \"description\":\"Toggles light\"},"
                          "{\"command\":\"brillo_menos\", \"description\":\"Brightness -\"},"
                          "{\"command\":\"brillo_mas\", \"description\":\"Brightness +\"},"
                          "{\"command\":\"2h\", \"description\":\"Fan will turn off after 2h\"},"
                          "{\"command\":\"4h\", \"description\":\"Fan will turn off after 4h\"},"
                          "{\"command\":\"8h\", \"description\":\"Fan will turn off after 8h\"},"
                          "{\"command\":\"source\", \"description\":\"Changes TV source\"},"
                          "{\"command\":\"restart\", \"description\":\"Restart the device\"},"
                          "{\"command\":\"offall\", \"description\":\"Turn off everything\"},"
                          "{\"command\":\"help\",  \"description\":\"Get bot usage help\"},"
                          "{\"command\":\"start\", \"description\":\"Message sent when you open a chat with a bot\"},"
                          "{\"command\":\"status\",\"description\":\"Answer device current status\"}" // no comma on last command
                          "]");

char admin1_chat_id[20];

void setup_telegram_controller(){
  bot.setMyCommands(COMMANDS);

  getFirstID(ADMINS_CHAT_IDS, admin1_chat_id);

  bot.sendMessage(admin1_chat_id, "Alexa 🦋 Bot started up ⤴ with IP: "+WiFi.localIP().toString(), "");
}



bool isIDInList(const char* id, const char* idList) {
  const char* start = idList;
  const char* end;
  int idLength = strlen(id);

  // strchr(start, ',') finds the next comma in the string.
  while ((end = strchr(start, ',')) != NULL) {
    if ((end - start == idLength) && (strncmp(start, id, idLength) == 0)) {
      return true;
    }
    start = end + 1;
  }
  // Check the last token (or the only one if there's no comma)
  if (strcmp(start, id) == 0) {
    return true;
  }

  return false;
}

// Function to get the first ID from a comma-separated list
void getFirstID(const char* idList, char* firstID) {
  const char* end = strchr(idList, ','); // Find the first comma

  if (end != NULL) {
    // Copy the first ID into firstID buffer
    strncpy(firstID, idList, end - idList);
    firstID[end - idList] = '\0'; // Null-terminate the string
  } else {
    // If no comma is found, the whole idList is a single ID
    strcpy(firstID, idList);
  }
}


// checks if telegram user is allowed to use the bot
int is_admin(String user_chat_id){
  //if(user_chat_id.equals(ADMIN1_CHAT_ID)){
  //if (ADMINS_CHAT_IDS.indexOf(user_chat_id) > 0) {
  if (isIDInList(user_chat_id.c_str(), ADMINS_CHAT_IDS)){
    return true;
  }
  return false;
}

int telegram_control(String text, String msg_chat_id, int numNewMessages){
  if (is_admin(msg_chat_id)){
    // Echo the received message back to the sender
    bot.sendMessage(msg_chat_id, "Received: " +text+" ....");

    if (text.equalsIgnoreCase("/start")){
      Serial.println("/start command");
      bot.sendMessage(msg_chat_id, "Start command.");
    }
    else if (text.equalsIgnoreCase("/help")){
      Serial.println("/help command");
      bot.sendMessage(msg_chat_id, "Print available commands ");
    }
    else if (text.equalsIgnoreCase("/restart")){
      Serial.println("Reinciando...");
      bot.sendMessage(msg_chat_id, "Reiniciando...");
      numNewMessages = bot.getUpdates(bot.last_message_received + 1); 
      ESP.restart();
    }
    else if (text.equalsIgnoreCase("/status")){
      Serial.println("/status command");
      //bot.sendMessage(msg_chat_id, ESP.esp_chip_info());
    }
    else if (text.equalsIgnoreCase("/offall")){
      Serial.println("/offall command");
      shut_down();
      bot.sendMessage(msg_chat_id, "Todo apagado");
    }

    else if (text.equalsIgnoreCase("high") or text == "/high"){
      Serial.println("enviando HIGH fan");
      operation = OP_FAN_HGH;
      bot.sendMessage(msg_chat_id, "Enviado HIGH fan ");
    }
    else if (text.equalsIgnoreCase("med") or text == "/med"){
      Serial.println("enviando MED fan");
      operation = OP_FAN_MDM;
      bot.sendMessage(msg_chat_id, "Enviado MED fan ");
    }
    else if (text.equalsIgnoreCase("low") or text == "/low"){
      Serial.println("enviando LOW fan");
      operation = OP_FAN_LOW;
      bot.sendMessage(msg_chat_id, "Enviado LOW fan ");
    }
    else if (text.equalsIgnoreCase("off") or text == "/off"){
      Serial.println("enviando off fan");
      operation = OP_FAN_OFF;
      bot.sendMessage(msg_chat_id, "Enviado off fan ");
    }
    else if (text.equalsIgnoreCase("Light") or text == "/light"){
      Serial.println("enviando LIGHT fan 💡");
      operation = OP_TECHO_ON;
      bot.sendMessage(msg_chat_id, "Enviado LIGHT fan 💡 ");
    }
    else if (text.equalsIgnoreCase("2") or text == "/2h"){
      Serial.println("enviando 2H fan");
      time_2h(); // TODO: cambiarlo a una operacion (operation = OP_FAN_2H)
      bot.sendMessage(msg_chat_id, "Enviado 2H fan ");
    }
    else if (text.equalsIgnoreCase("4") or text == "/4h"){
      Serial.println("enviando 4H fan");
      time_4h(); // TODO: cambiarlo a una operacion (operation = OP_FAN_4H)
      bot.sendMessage(msg_chat_id, "Enviado 4H fan ");
    }
    else if (text.equalsIgnoreCase("8") or text == "/8h"){
      Serial.println("enviando 8H fan");
      time_8h(); // TODO: cambiarlo a una operacion (operation = OP_FAN_8H)
      bot.sendMessage(msg_chat_id, "Enviado 8H fan ");
    }
    else if (text.equalsIgnoreCase("brillo-") or text == "/brillo_menos"){
      turn_down_bright();
      bot.sendMessage(msg_chat_id, "Brillo bajado");
    }
    else if (text.equalsIgnoreCase("brillo+") or text == "/brillo_mas"){
      turn_up_bright();
      bot.sendMessage(msg_chat_id, "Brillo subido");
    }
    else if (text.equalsIgnoreCase("Source") or text == "/source"){
      tv_source();
      bot.sendMessage(msg_chat_id, "TV Source pulsado");
    }
    else if (text.equalsIgnoreCase("teleoff") or text == "/teleoff"){
      shutdown_tv();
      bot.sendMessage(msg_chat_id, "Apagando/encendiendo tele");
    }
    else if (text.equalsIgnoreCase("led") or text == "/leds"){
      led_shut_down();
      bot.sendMessage(msg_chat_id, "Apagando/encendiendo LED");
    }
    else if (text.startsWith("/nec ")){
      // /nec 0x0a;0x0b
      String hexes = text.substring(5,15);
      String addr = hexes.substring(0, 4);
      String cmd = hexes.substring(5, 9);
      Serial.println("HEXES: ");
      Serial.println(addr);
      Serial.println(cmd);
      String encoded_hex; 
      //encoded_hex=String(encode_NEC(strtol(addr.c_str(), NULL, 0), strtol(cmd.c_str(), NULL, 0)));
      char buffer[75];
      sprintf(buffer, "%08ul o 0x%08X",  encode_NEC(strtol(addr.c_str(), NULL, 0), strtol(cmd.c_str(), NULL, 0)), encode_NEC(strtol(addr.c_str(), NULL, 0), strtol(cmd.c_str(), NULL, 0)));
      encoded_hex=String(encode_NEC(strtol(addr.c_str(), NULL, 0), strtol(cmd.c_str(), NULL, 0)));
      bot.sendMessage(msg_chat_id, buffer);  // TODO mejorar el texto, se debería devolver un string en formato hex 0x50AFD02F
      //bot.sendMessage(msg_chat_id, "NEX: "+ String(encode_NEC(strtol(addr.c_str(), NULL, 0), strtol(cmd.c_str(), NULL, 0))));
      //bot.sendMessage(msg_chat_id, "NEX: "+ encode_NEC(addr.toInt(), cmd.toInt()));
    }
    else if (text.equalsIgnoreCase("test") or text == "/test"){
      // send_message_TV("0x04000300");
      test_light();
      bot.sendMessage(msg_chat_id, "comando test");
    }

    else {
      Serial.println("comando no interpretado: "+text);
      bot.sendMessage(msg_chat_id, "comando no interpretado: "+text);
    }

  } else {
    Serial.println("El usuario con CHATID: "+msg_chat_id+" (no admin) está intentando usar el bot.");
    bot.sendMessage(admin1_chat_id, "El usuario con CHATID: "+msg_chat_id+" (no admin) está intentando usar el bot.");
  }
  return numNewMessages;
}
