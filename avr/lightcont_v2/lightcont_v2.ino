#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>

// include the library code:
#include <LiquidCrystal.h>

#define BUTTON_SELECT    5
#define BUTTON_LEFT      4
#define BUTTON_DOWN      3
#define BUTTON_UP        2
#define BUTTON_RIGHT     1
#define BUTTON_NONE      0

#define LCD_RS  46
#define LCD_RW  48

#define LCD_D4  38
#define LCD_D5  40
#define LCD_D6  42
#define LCD_D7  44
#define LCD_BL 2

#define OUT_R   6
#define OUT_G   7
#define OUT_B   8

byte redLevel = 255;
byte greenLevel = 255;
byte blueLevel = 255;

boolean useEthernet = true;

int currentChannel = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

byte mac[] = { 0x00, 0x13, 0x37, 0x01, 0x02, 0x03};
byte ip[] = {172,22,0,59};
byte gateway[] = {172,22,0,1};
byte subnet[] = {255,255,255,0};

EthernetUDP Udp;
EthernetClient client;

int checkButton() {
  int ButtonVoltage = analogRead(0);
  int ButtonPressed = BUTTON_NONE;
  
  if (ButtonVoltage > 800) ButtonPressed = BUTTON_NONE;    // No button pressed should be 1023
  else if (ButtonVoltage > 500) ButtonPressed = BUTTON_SELECT;   
  else if (ButtonVoltage > 400) ButtonPressed = BUTTON_LEFT;   
  else if (ButtonVoltage > 250) ButtonPressed = BUTTON_DOWN;   
  else if (ButtonVoltage > 100) ButtonPressed = BUTTON_UP; 
  else ButtonPressed = BUTTON_RIGHT;
  return ButtonPressed;
}

void setup() {
  lcd.begin(16, 2);
  
  pinMode(OUT_R, OUTPUT);
  pinMode(OUT_G, OUTPUT);
  pinMode(OUT_B, OUTPUT);
   
  pinMode(LCD_BL, OUTPUT);
  analogWrite(LCD_BL, 255);

  lcd.clear();
  
  if (checkButton() != BUTTON_NONE) {
    useEthernet = false;
  }
  
  if (useEthernet) {
    lcd.print("Waiting for DHCP");
    
    //Ethernet.begin(mac, ip, gateway, subnet);
    Ethernet.begin(mac);
    
    if (Ethernet.begin(mac) == 0) {
      lcd.print("failed...");
      for(;;) {};
    }
    lcd.clear();
    lcd.print(Ethernet.localIP()[0], DEC);
    lcd.print("."); 
    lcd.print(Ethernet.localIP()[1], DEC);
    lcd.print(".");
    lcd.print(Ethernet.localIP()[2], DEC);
    lcd.print(".");
    lcd.print(Ethernet.localIP()[3], DEC);
    lcd.setCursor(0,1);
    lcd.print("press any key");
    
    long pressTime = millis();
    
    while (checkButton() == BUTTON_NONE && (millis() - pressTime) < 5000 ) {
      lcd.setCursor(14,1);
      lcd.print(5-((millis() - pressTime) / 1000));
    };
    Udp.begin(54127);
  }
  

}

long lastUpdateTime = 0;
long dimTime = 0;
int brightness = 255;

void updateLCD() {
  if ((millis() - lastUpdateTime) < 100) return;
  lastUpdateTime = millis();
  lcd.clear();
  
  lcd.setCursor(0,0);
  (currentChannel == 0) ? lcd.print("<") : lcd.print(" ");
  lcd.print(redLevel);
  lcd.setCursor(4,0);
  (currentChannel == 0) ? lcd.print(">") : lcd.print(" ");
  
  lcd.setCursor(5,0);
  (currentChannel == 1) ? lcd.print("<") : lcd.print(" ");
  lcd.print(greenLevel);
  lcd.setCursor(9,0);
  (currentChannel == 1) ? lcd.print(">") : lcd.print(" ");
  
  lcd.setCursor(10,0);
  (currentChannel == 2) ? lcd.print("<") : lcd.print(" ");
  lcd.print(blueLevel);
  lcd.setCursor(14,0);
  (currentChannel == 2) ? lcd.print(">") : lcd.print(" ");
  
  lcd.setCursor(1,1);
  lcd.print("R");
  lcd.setCursor(6,1);
  lcd.print("G");
  lcd.setCursor(11,1);
  lcd.print("B");
  
  analogWrite(OUT_R, redLevel);
  analogWrite(OUT_G, greenLevel);
  analogWrite(OUT_B, blueLevel);
  
}

void fullBright() {
    dimTime = millis() + 4000;
    brightness = 255;
    analogWrite(LCD_BL, brightness);
}

void loop() {
  if (useEthernet) {
    int packetLen = Udp.parsePacket();
    if (packetLen == 3) {
      redLevel = Udp.read();
      greenLevel = Udp.read();
      blueLevel = Udp.read();
    }
  }

  //lcd.write(client.read());
  updateLCD();
  
  if (checkButton() == BUTTON_LEFT) {
    (currentChannel > 0) ? currentChannel-- : currentChannel = 2;
    fullBright();
    while (checkButton() != BUTTON_NONE) { delay(10);};
  }
  
  if (checkButton() == BUTTON_RIGHT) {
    (currentChannel < 2) ? currentChannel++ : currentChannel = 0;
    fullBright();
    while (checkButton() != BUTTON_NONE) { delay(10);};
  }
  
  if (checkButton() == BUTTON_DOWN) {
    fullBright();
    switch(currentChannel) {
      case 0:
        (redLevel > 4) ? redLevel -= 5 : redLevel = 0;
        analogWrite(OUT_R, redLevel);
        break;
      case 1:
        (greenLevel > 4) ? greenLevel -= 5 : greenLevel = 0;
        analogWrite(OUT_G, greenLevel);
        break;
      case 2:
        (blueLevel > 4) ? blueLevel -= 5 : blueLevel = 0;
        analogWrite(OUT_B, blueLevel);
        break;
    }
    unsigned long pushTime = millis();
    while ((checkButton() != BUTTON_NONE) && ((millis() - pushTime) < 50 )) { delay(10);};
  }
  
  if (checkButton() == BUTTON_UP) {
    fullBright();
    switch(currentChannel) {
      case 0:
        (redLevel < 251) ? redLevel += 5 : redLevel = 255;
        analogWrite(OUT_R, redLevel);
        break;
      case 1:
        (greenLevel < 251) ? greenLevel += 5 : greenLevel = 255;
        analogWrite(OUT_G, greenLevel);
        break;
      case 2:
        (blueLevel < 251) ? blueLevel += 5 : blueLevel = 255;
        analogWrite(OUT_B, blueLevel);
        break;
    }
    unsigned long pushTime = millis();
    while ((checkButton() != BUTTON_NONE) && ((millis() - pushTime) < 50 )) { delay(10);};
  }
  
  if ((millis() > dimTime) && brightness > 10) {
      brightness--;
      analogWrite(LCD_BL, brightness);
  }
}

