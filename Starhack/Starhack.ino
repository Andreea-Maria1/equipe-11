#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include <DHT11.h>
#include <SPI.h>
#include <MFRC522.h>

// Pin configuration
#define SS_PIN 3   // RFID SS (SDA)
#define RST_PIN 6   // RFID Reset
#define RED_PIN 43  // Red LED
#define GREEN_PIN 45 // Green LED
#define BLUE_PIN 47  // Blue LED

#define RS 7
#define E 8
#define D4 9
#define D5 10
#define D6 11
#define D7 12

#define DHT_PIN 2

MFRC522 rfid(SS_PIN,RST_PIN);
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
DHT_nonblocking dht(DHT_PIN, DHT_TYPE_11);

byte validUID[] = {0x6D, 0x20, 0xA8, 0x82};

byte separator[8] = {
  0b01100,
  0b01100,
  0b01100,
  0b01100,
  0b01100,
  0b01100,
  0b01100,
  0b01100
};
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};
byte cross[8] = {
  0b00000,
  0b10001,
  0b10001,
  0b01010,
  0b00100,
  0b01010,
  0b10001,
  0b10001
};
byte top1Cat[8] = {
  0b00000,
  0b00001,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b00010
};
byte top2Cat[8] = {
  0b00000,
  0b11000,
  0b01110,
  0b00101,
  0b00010,
  0b00011,
  0b00111,
  0b01111
};
byte top3Cat[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10000,
  0b11111,
  0b11010,
  0b10010
};
byte top4Cat[8] = {
  0b00000,
  0b00000,
  0b00011,
  0b00101,
  0b01010,
  0b11110,
  0b11111,
  0b01111
};
byte top5Cat[8] = {
  0b00000,
  0b11100,
  0b10010,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b10010
};
byte bottom1Cat[8] = {
  0b00001,
  0b00001,
  0b00000,
  0b00001,
  0b00001,
  0b00111,
  0b00001,
  0b01111
};
byte bottom2Cat[8] = {
  0b10111,
  0b00010,
  0b10111,
  0b01000,
  0b10011,
  0b10011,
  0b01000,
  0b00111
};
byte bottom3Cat[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b10000,
  0b01000,
  0b01000,
  0b10000,
  0b00010
};
byte bottom4Cat[8] = {
  0b00111,
  0b00010,
  0b00111,
  0b01000,
  0b10110,
  0b10110,
  0b01000,
  0b00111
};
byte bottom5Cat[8] = {
  0b01100,
  0b00100,
  0b01000,
  0b10100,
  0b01100,
  0b01111,
  0b10100,
  0b00111
};
void displayCat(){
  //top
  lcd.createChar(2, top1Cat);
  lcd.setCursor(13, 0);
  lcd.write(byte(2));

  lcd.createChar(3, top2Cat);
  lcd.setCursor(14, 0);
  lcd.write(byte(3));

  lcd.createChar(4, top3Cat);
  lcd.setCursor(15, 0);
  lcd.write(byte(4));
  // bottom
  lcd.createChar(5, bottom1Cat);
  lcd.setCursor(13, 1);
  lcd.write(byte(5));

  lcd.createChar(6, bottom2Cat);
  lcd.setCursor(14, 1);
  lcd.write(byte(6));

  lcd.createChar(7, bottom3Cat);
  lcd.setCursor(15, 1);
  lcd.write(byte(7));
}
void displaySetup(){
  lcd.begin(16,2);
  lcd.createChar(1, separator);
  lcd.setCursor(4, 1);
  lcd.write(byte(1));
  lcd.setCursor(4, 0);
  lcd.write(byte(1));
  displayCat();
}

void setup() {
  Serial.begin(9600);
  // SPI.begin();
  // rfid.PCD_Init();
  displaySetup();
}

void displayIcon(uint8_t col, uint8_t row, byte* icon){
  lcd.createChar(0,icon);
  lcd.setCursor(col, row);
  lcd.write(byte(0));
}

void updateTemp(float temp){
  char buffer[10];
  lcd.setCursor(0, 0);
  lcd.print("T:");
  dtostrf(temp, 2, 0, buffer);
  lcd.print(buffer);
}

void updateHum(float hum){
  char buffer[10];
  lcd.setCursor(0, 1);
  lcd.print("H:");
  dtostrf(hum, 2, 0, buffer);
  lcd.print(buffer);
}

bool isPotatoOkay(float temp, float hum){
  return (temp <= 30.0 && temp >= 15.0) && ( hum <= 95.0 && hum >= 85.0);
}

void displayPotato(){
  lcd.setCursor(5, 0);
  lcd.print("Pot:");
}

void dhtTask(){
  float temp = 0.0;
  float hum = 0.0;

  if (dht.measure(&temp, &hum)) {
  updateTemp(temp);
  updateHum(hum);
  }

  displayPotato();

  if(isPotatoOkay(temp,hum)){
    displayIcon(9, 0, heart);
  }else{
    displayIcon(9,0, cross);
  }
}

void rfIdTask(){
  if (!rfid.PICC_IsNewCardPresent()) return;

  if (!rfid.PICC_ReadCardSerial()) return;

  bool isValid = true;

  for (byte i = 0; i < rfid.uid.size; i++) 
    if (rfid.uid.uidByte[i] != validUID[i])
      isValid = false;

  if (isValid) {
    setLEDColor(0, 255, 0);
    delay(4000);
  } else {
    setLEDColor(255, 0, 0);
    delay(1000);
  }
  
  setLEDColor(255, 0, 0);

  rfid.PICC_HaltA();
}

void setLEDColor(int red, int green, int blue) {
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}

void loop() {
  delay(1000);
  // rfIdTask();
  delay(500);
  dhtTask();

}
