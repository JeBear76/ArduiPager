
bool sendSMS(){
  
  resetModem();

  Serial.println("AT+CMGF=1");
  modem.write("AT+CMGF=1\r");
  if(waitForResponse(false)){ 
    Serial.write("AT+CSCS=\"GSM\"\r");
    modem.write("AT+CSCS=\"GSM\"\r");   
    if(waitForResponse(false)){
      Serial.write("AT+CMGS=\"+");
      modem.write("AT+CMGS=\"+");
      Serial.print(phoneNumber);
      modem.print(phoneNumber);
      Serial.print("\"");
      modem.print("\"");
      Serial.print("\r");
      modem.print("\r");
      delay(1000); //Last command returns a >. Not waiting for that.
      Serial.print(message);
      modem.print(message);
      waitForResponse(false);
      modem.write((char)26);
      waitForResponse(true);      
    }
  }
}

void deleteSMS(bool all){
  resetModem();
  Serial.println("AT+CMGF=1");
  modem.write("AT+CMGF=1\r");
  uint8_t flag = 0;
  if(all) flag = 4;
  if(waitForResponse(false)){ 
    Serial.write("AT+CMGD=");
    modem.write("AT+CMGD=");
    Serial.print(smsID);
    modem.print(smsID);
    Serial.write(",");
    modem.write(",");
    Serial.print(flag);
    modem.print(flag);
    Serial.write("\r");
    modem.write("\r");
    waitForResponse(true);
  } 
  smsID = 0;
}

void getSMSList(){
  Serial.println("AT+CMGF=1");
  modem.write("AT+CMGF=1\r");
  if(waitForResponse(false)){ 
    Serial.write("AT+CMGL=\"ALL\",1\r");
    modem.write("AT+CMGL=\"ALL\",1\r");    
    waitForResponse(true);
  }
}

uint8_t getSMSCount(){
  resetModem();
  getSMSList();
  return countSMS();
}

SMS buildSMS(){
  SMS result;
  result.phoneNumber = modemResponse.substring(modemResponse.indexOf('+',modemResponse.lastIndexOf("+CMGL:") + 1), modemResponse.indexOf('"',modemResponse.indexOf('+',modemResponse.lastIndexOf("+CMGL:") + 1)) - 1);
  Serial.println(result.phoneNumber);
  String tmp =modemResponse.substring(modemResponse.lastIndexOf("\"") + 1, modemResponse.lastIndexOf("OK"));
  tmp.replace((char)13,(char)32);
  tmp.replace((char)10,(char)32);
  tmp.trim();
  result.message = tmp;
  Serial.println(result.message);
  return result;
}

SMS getSMS(){
  resetModem();
  getSMSList();
  return buildSMS();
}

uint8_t countSMS(){
  uint8_t count = 0;
  int lastloc = modemResponse.indexOf("+CMGL:");
  
  while(lastloc != -1){
    count++;
    lastloc = modemResponse.indexOf("+CMGL:", ++lastloc);
  }
  
  if(count != 0){
    String tmp;
    tmp = modemResponse.substring(modemResponse.lastIndexOf("+CMGL: ") + 7, modemResponse.indexOf(',',modemResponse.lastIndexOf("+CMGL: ") + 7));
    smsID = tmp.toInt();
  }
  return count;
}

bool waitForResponse(bool longwait){
  modemResponse = "";
  modem.flush();
  Serial.println();
  long timeout = MODEM_TIMEOUT;
  if(longwait) timeout = LONG_TIMEOUT;  
  bool dataReceived = false;
  long t = millis();
  while(millis() - t < timeout || modem.available()){
    if(modem.available()){
      Serial.println();
      Serial.println("Received response");
      while(modem.available()) {
        modemResponse += (char)modem.read();
      }
      dataReceived = true;
      for(int i = 0; i < 25; i++){
        if(modem.available()){
          break;
        }
        delay(10);
      }
      if(!modem.available()) break;
    }else{
      Serial.print(".");
      delay(250);
    }    
  }
  Serial.println(modemResponse);
  return dataReceived;
}

void resetModem(){
  Serial.println("Resetting Modem");
  digitalWrite(RST, HIGH);
  delay(100);
  digitalWrite(RST, LOW);
  delay(2000);
  Serial.println("Modem Reset");
}

