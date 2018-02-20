void printKey(char key){
  lcd.print(key);
  phoneNumber += key;
  phoneNumberLength++;
}

void doBackSpace(String& obj, short& objLength){
  objLength--;
  lcd.setCursor(objLength,1);
  lcd.print(" ");
  lcd.setCursor(objLength,1);
  doMemoryBackSpace(obj);
}

void writeMappedCharacter(char key){
  KeyMapper keymapper(key);
  char toPrint;
  char newKey;
  for(;;){
    toPrint = keymapper.getNextChar();
    lcd.print(toPrint);
    lcd.setCursor(messageLength,1);
    newKey = keypad.waitForKey(500);
    if(newKey == NO_KEY || newKey != key)
      break;
  } 
  message += toPrint;
  lcd.setCursor(++messageLength,1);
  if(messageLength > SCREEN_COLUMNS){
    lcd.scrollDisplayLeft();     
  }
}

void writeBlankLine(int row){
  lcd.setCursor(0, row);
  lcd.print("                ");
  lcd.setCursor(0, row);
}

int displayScreen(int screen){
  return displayScreen(screen, 0);
}

int displayScreen(int screen, int forward){
  lcd.home();
  lcd.noBlink();
  writeBlankLine(0);
  switch(screen){
    case (MAIN_MENU):{
      messageLength = 0;
      message = "";
      phoneNumberLength = 0;
      phoneNumber = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(MainMenuA); 
      lcd.setCursor(0, 1); 
      lcd.print(MainMenuB);
      smsCount = getSMSCount();
      break;
    }
    case (SMS_SCREEN):{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(SMSMessageHeader);
      messageLength = 0;
      message = "";
      lcd.blink();
      break;
    }
    case (SMS_SEND):{
      lcd.setCursor(0, 0);
      lcd.print(SMSSendHeader);
      phoneNumberLength = 0;
      phoneNumber = "";
      lcd.blink();
      break;
    }
    case (SMS_SENDING_SCREEN):{
      lcd.setCursor(0, 0);
      lcd.print(SMSSendingHeader);
      Serial.print("Sending :");
      Serial.println(message);
      Serial.print("Sending to :");
      Serial.println(phoneNumber);
      sendSMS();    
      return displayScreen(SMS_STATUS_SCREEN);
    }    
    case (SMS_STATUS_SCREEN):{
      lcd.setCursor(0, 0);
      lcd.print(SMSSentHeader);
      keypad.waitForKey(USER_TIMEOUT);
      return displayScreen(MAIN_MENU);
    }
    case (SMS_COUNT_SCREEN):
      lcd.clear();{
      lcd.setCursor(0, 0);
      smsCount = getSMSCount();
      lcd.print(smsCount);
      lcd.print(SMSCountHeader);
      lcd.setCursor(0, 1);
      lcd.print(SMSMenu);
      break;   
    }
    case (SMS_CLEAR_SCREEN):{
      writeBlankLine(1);
      lcd.setCursor(0, 0);
      lcd.print(SMSClearHeader);
      deleteSMS(true);
      return displayScreen(SMS_COUNT_SCREEN);
    }
    case (SMS_READ_SCREEN):{
      lcd.clear();
      SMS sms = getSMS(); 
      lcd.setCursor(0, 0);
      Serial.print("rec number: ");
      Serial.println(sms.phoneNumber);
      lcd.print(sms.phoneNumber);
      lcd.setCursor(0, 1);
      Serial.print("rec message: ");
      Serial.println(sms.message);
      lcd.print(sms.message.substring(0,16));
      lcd.home();
      break;
    }
    case (SMS_DELETE_SCREEN):{
      lcd.clear();
      lcd.print(SMSDeleteHeader);
      deleteSMS(false);
      return displayScreen(forward);
      break;      
    }
  }
  lcd.setCursor(0, 1);
  currentScreen = screen;
  return currentScreen;
}

