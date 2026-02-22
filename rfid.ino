#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <MFRC522.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RST_PIN 9
#define SS_PIN 10
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MFRC522 rfid(SS_PIN, RST_PIN);

int textIteration = 0;
int textScreenTime = 1000;
String currentString = "Good morning!";
String personalUID = "96:0B:6C:96";

unsigned long cardDetectedAt = 0;
bool cardPresent = false;
const unsigned long CONNECTED_DURATION = 5000 * 60;

String wordGenerator() {
  float randomNumber = random(0, 5);
  if (randomNumber < 1) {
    return "Have a great day!";
  } else if (randomNumber < 2) {
    return "Have you drank water yet?";
  } else if (randomNumber < 3) {
    return "What's one thing you're proud you did today?";
  } else if (randomNumber < 4) {
    return "Hi there!!";
  } else {
    return "How are you?";
  }
 
    /// when connection becomes a thing:
  if (randomNumber < 1) {
    return "OMG, hello!" ;
  } else if (randomNumber < 2) {
    return "Hi there!!";
  } else {
    return "Woah, is that a friend? :D";
  }

  return "Hiii!!";
}

void textSide() {

  display.println(currentString.substring(0, min(textIteration, currentString.length())));
  textIteration = textIteration + 1;
  if (textIteration >= textScreenTime) {
    textIteration = 0;
    currentString = wordGenerator();
  }
  //Serial.println(textIteration);
  //Serial.println(currentString);
}


void characterSide() {
}

void connectivity() {

  if (cardPresent && millis - cardDetectedAt >= CONNECTED_DURATION) {
    cardPresent = false;
  }


  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = getUID();
    Serial.println(F("Card UID"));
    Serial.println(uid);


    if (uid != personalUID) {
        cardPresent = true;
        cardDetectedAt = millis();
        textIteration = textScreenTime;
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  ///Serial.println(textIteration);
}

String getUID() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) uid += ":";
  }
  uid.toUpperCase();
  return uid;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  textIteration = textScreenTime;

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  connectivity();

  display.clearDisplay();
  display.setCursor(5, 5);
  textSide();
  ///display.println("hi");
  characterSide();
  display.display();
  delay(10);
}









