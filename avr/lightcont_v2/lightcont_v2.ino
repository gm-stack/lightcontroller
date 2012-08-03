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

#define LCD_RS  7
#define LCD_RW  8
#define LCD_D4  A0
#define LCD_D5  A1
#define LCD_D6  A2
#define LCD_D7  A3

#define BUTTON_1 2
#define BUTTON_2 4
#define BUTTON_3 1

#define OUT_R   5
#define OUT_G   9
#define OUT_B   3
#define OUT_AUX 6

byte redLevel = 255;
byte greenLevel = 255;
byte blueLevel = 255;
byte auxLevel = 0;

int currentChannel = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

byte mac[] = { 0x00, 0x13, 0x37, 0x01, 0x02, 0x03};
byte ip[] = {172,22,0,59};
byte gateway[] = {172,22,0,1};
byte subnet[] = {255,255,255,0};

byte ircserver1[] = {172,22,0,52};

EthernetUDP Udp;
EthernetClient client;

void setup() {
  lcd.begin(20, 2);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  digitalWrite(BUTTON_1, HIGH);
  digitalWrite(BUTTON_2, HIGH);
  digitalWrite(BUTTON_3, HIGH);
  
  pinMode(OUT_R, OUTPUT);
  pinMode(OUT_G, OUTPUT);
  pinMode(OUT_B, OUTPUT);
  pinMode(OUT_AUX, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  Udp.begin(54127);
  //client.connect(ircserver1,6667);
  //client.print("JOIN #beepboop\n");
  //client.print("NICK lightsbot\n");
  //client.print("USER lightsbot lightsbot * lightsbot\n");
}

long lastUpdateTime = 0;

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
  
  lcd.setCursor(15,0);
  (currentChannel == 3) ? lcd.print("<") : lcd.print(" ");
  lcd.print(auxLevel);
  lcd.setCursor(19,0);
  (currentChannel == 3) ? lcd.print(">") : lcd.print(" ");
  
  lcd.setCursor(1,1);
  lcd.print("R");
  lcd.setCursor(6,1);
  lcd.print("G");
  lcd.setCursor(11,1);
  lcd.print("B");
  lcd.setCursor(16,1);
  lcd.print("X");
  
  analogWrite(OUT_R, redLevel);
  analogWrite(OUT_G, greenLevel);
  analogWrite(OUT_B, blueLevel);
  analogWrite(OUT_AUX, auxLevel);
}

void loop() {
  int packetLen = Udp.parsePacket();
  if (packetLen == 3) {
    redLevel = Udp.read();
    greenLevel = Udp.read();
    blueLevel = Udp.read();
  }

  //lcd.write(client.read());
  updateLCD();
  if (!digitalRead(BUTTON_2)) {
    (currentChannel < 3) ? currentChannel++ : currentChannel = 0;
    while (!digitalRead(BUTTON_2)) { delay(10);};
  }
  
  if (!digitalRead(BUTTON_1)) {
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
      case 3:
        (auxLevel > 4) ? auxLevel -= 5 : auxLevel = 0;
        analogWrite(OUT_AUX, auxLevel);
        break;
    }
    unsigned long pushTime = millis();
    while (!digitalRead(BUTTON_1) && ((millis() - pushTime) < 50 )) { delay(10);};
  }
  
  if (!digitalRead(BUTTON_3)) {
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
      case 3:
        (auxLevel < 251) ? auxLevel += 5 : auxLevel = 255;
        analogWrite(OUT_AUX, auxLevel);
        break;
    }
    unsigned long pushTime = millis();
    while (!digitalRead(BUTTON_3) && ((millis() - pushTime) < 50 )) { delay(10);};
  }
  
}

