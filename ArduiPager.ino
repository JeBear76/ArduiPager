#include <SoftwareSerial.h>
#include <KeyMapper.h>
#include <LiquidCrystal2.h>
#include <Keypad.h>
#include <SMS.h>

#include <String.h>
#define MAIN_MENU  0x01
#define SMS_SCREEN  0x02
#define SMS_SEND  0x04
#define SMS_SENDING_SCREEN 0x08
#define SMS_STATUS_SCREEN 0x10
#define SMS_COUNT_SCREEN 0x20
#define SMS_READ_SCREEN 0x40
#define SMS_CLEAR_SCREEN 0x80
#define SMS_DELETE_SCREEN 0x100

#define MODEM_TIMEOUT 2000
#define LONG_TIMEOUT 30000
#define USER_TIMEOUT 10000

#define SCREEN_COLUMNS 16
#define SCREEN_ROWS 2

#define RX 2
#define TX 3
#define RST 7

//For lcd
const int rs = 8, en = 9, clk = 6, latch = 5, data = 4;
//For keypad
const byte rows = 4;
const byte cols = 4;
//Screen messages
const char* SMSMessageHeader = "Write Message:";
const char* SMSCountHeader = " message(s) received";
const char* SMSClearHeader = "Deleting All...";
const char* SMSDeleteHeader = "Deleting...";
const char* SMSSendHeader = "Send to Number:";
const char* SMSSendingHeader = "Sending...";
const char* SMSSentHeader = "Message Sent";
const char* MainMenuA = "A: Send message";
const char* MainMenuB = "B: Read Messages";
const char* SMSMenu = "A: Read *: Del";

//init lcd
LiquidCrystal2 lcd(rs, en, clk, latch, data);
short currentScreen = MAIN_MENU;

//init keypad
char keys[rows][cols] = {
  {'1','2','3','>'},
  {'4','5','6','<'},
  {'7','8','9',(char)8},
  {'*','0','#',(char)18}
};

byte rowPins[rows] = {14, 15, 16, 17};
byte colPins[cols] = {10, 11, 12, 13};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

//init modem
SoftwareSerial modem(RX, TX);
int smsCount=0;
uint8_t smsID = 0;
String modemResponse;

short phoneNumberLength = 0;
short messageLength = 0;
String phoneNumber;
String message;
long smsLastCheck = 0;

void setup() {
  Serial.begin(9600);
  modem.begin(9600);
  pinMode(RST, OUTPUT);
  lcd.begin(SCREEN_COLUMNS, SCREEN_ROWS);  lcd.print("Welcome!");
  delay(1000);
  displayScreen(MAIN_MENU);
}

void loop() {
  if(millis() - smsLastCheck > LONG_TIMEOUT * 2){
    smsCount = getSMSCount();
    smsLastCheck = millis();
  }
  if(smsCount != 0)
    lcd.blink();
  else
    lcd.noBlink();
  //Serial.println(currentScreen);
  char key = keypad.getKey();
  if(key != NO_KEY){
    doKeyPadAction(key);
  }
}

void doMemoryBackSpace(String& obj){
  obj = obj.substring(0, obj.length()-1);
}

