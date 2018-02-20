void doKeyPadAction(char key){
  switch(key){
    case '>':  {
      if(currentScreen & MAIN_MENU){
        displayScreen(SMS_SCREEN); 
        return;
      }
      if(currentScreen & SMS_SCREEN){
        displayScreen(SMS_SEND);
        return;
      }
      if(currentScreen & SMS_SEND){
        displayScreen(SMS_SENDING_SCREEN);
        return;
      }
      if(currentScreen & SMS_COUNT_SCREEN){
        displayScreen(SMS_READ_SCREEN);
        return;
      }
      if(currentScreen & SMS_READ_SCREEN){
        displayScreen(SMS_DELETE_SCREEN, SMS_COUNT_SCREEN);
        return;
      }      
      break;
    }
    case '<':{
      if(currentScreen & MAIN_MENU){
        displayScreen(SMS_COUNT_SCREEN);
        return;
      }
      if(currentScreen & SMS_SCREEN){
        displayScreen(MAIN_MENU);
        return;
      }
      if(currentScreen & SMS_SEND){
        displayScreen(SMS_SCREEN);
        return;
      }
      if(currentScreen & SMS_COUNT_SCREEN){
        displayScreen(MAIN_MENU);
        return;
      }
      if(currentScreen & SMS_READ_SCREEN){
        displayScreen(SMS_DELETE_SCREEN, SMS_COUNT_SCREEN);
        return;
      }   
      break;
    }
    case (char)8:{
      if(currentScreen & SMS_SCREEN){
        doBackSpace(message, messageLength);
      }
      if(currentScreen & SMS_SEND){
        doBackSpace(phoneNumber, phoneNumberLength);
      }
      if(currentScreen & SMS_COUNT_SCREEN){
        displayScreen(SMS_COUNT_SCREEN);
      }
      break;
    }
    case (char)18:{
      if(currentScreen & SMS_READ_SCREEN){
        displayScreen(SMS_DELETE_SCREEN, MAIN_MENU);
      }
      if(!(currentScreen & SMS_SENDING_SCREEN)){
        displayScreen(MAIN_MENU);    
        return;
      }
      break;
    }
    default:{
      if(currentScreen & SMS_SCREEN || currentScreen & SMS_SEND){
        if(messageLength == 0) writeBlankLine(1);
        if(currentScreen & SMS_SCREEN){
          writeMappedCharacter(key);
        }
        if(currentScreen & SMS_SEND){
          printKey(key);
        }
      }
      if(currentScreen & SMS_COUNT_SCREEN){
        switch(key){
          case '*':{
            displayScreen(SMS_CLEAR_SCREEN);
            break;
          }
          default:{
            return;
          }
        }
      }
      break;
    }
  } 
}


