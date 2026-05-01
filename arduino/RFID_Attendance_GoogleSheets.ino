// Smart Attendance System with Face Trigger + RFID + Google Sheets
// ✅ ESP8266 + RFID + I2C LCD (Clean Serial for Python Integration)

#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

//-----------------------------------------
#define RST_PIN  D3
#define SS_PIN   D4
#define BUZZER   D8
//-----------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;  
MFRC522::StatusCode status;      
//-----------------------------------------
int blockNum = 2;  
byte bufferLen = 18;
byte readBlockData[18];
//-----------------------------------------
bool isLoggedIn = false;
String currentUser = "";
unsigned long loginTime = 0;
//-----------------------------------------
unsigned long lastScanTime = 0;
//-----------------------------------------
void ReadDataFromBlock(int blockNum, byte readBlockData[]);
//-----------------------------------------
String authorizedUser = "";
bool sessionCompleted = false;
//-----------------------------------------
String card_holder_name;
const String sheet_url = "https://script.google.com/macros/s/AKfycbykMLbhr4gLAfqN1zOt327U9YLm2KMK9XM-RjSmnkTAjEvzLiKttjogOYa3b1gqBQg1xg/exec";

//-----------------------------------------
#define WIFI_SSID "Vineeth's S24"
#define WIFI_PASSWORD "vineethmenon"

//-----------------------------------------
LiquidCrystal_PCF8574 lcd(0x27);  // I2C LCD address
bool rfidEnabled = false;         // Controlled by Python

/****************************************************************************************************
 * setup() function
 ****************************************************************************************************/
void setup()
{
  delay(500);                 
  Serial.begin(115200);        
  Serial.setDebugOutput(false); // ✅ Disable WiFi debug logs
  Serial.flush();               // ✅ Clear any previous buffer

  Serial.println("\n================================");
  Serial.println("✅ NodeMCU Booting...");
  Serial.println("================================");

  // ✅ Initialize LCD
  lcd.begin(16, 2);      
  lcd.setBacklight(255); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Initializing  ");
  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(400);
  }

  // ✅ WiFi Setup with timeout
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint32_t startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    Serial.print(".");
    delay(300);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n⚠️ WiFi not connected! Will retry in loop.");
  }

  // ✅ Setup buzzer
  pinMode(BUZZER, OUTPUT);

  // ✅ Initialize SPI + RFID
  SPI.begin();
  mfrc522.PCD_Init();
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for Face");
  lcd.setCursor(0, 1);
  lcd.print("Verification...");

  Serial.println("NODEMCU_READY"); // 🔹 Python will wait for this
}

/****************************************************************************************************
 * loop() function
 ****************************************************************************************************/
void loop()
{
  // Step 1: Check Serial for START command from Python
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.length() > 0) {
  authorizedUser = command;
  authorizedUser.toUpperCase();  // ensure uppercase

// 🔥 RESET SESSION HERE
  sessionCompleted = false;
  rfidEnabled = true;

  Serial.println("Authorized User: " + authorizedUser);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your Card");
  lcd.setCursor(0, 1);
  lcd.print(authorizedUser);  // show name below
}
  }

  // Step 2: If RFID is enabled, scan card
  if (rfidEnabled && !sessionCompleted) {
  // 📡 CHECK CARD
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
      return;  // No card yet
    }
    if (millis() - lastScanTime < 7000) { //delay of 7 seconds after scanning
  Serial.println("Wait before next scan");
  return;
   }

    // ✅ Card detected: print UID
    Serial.print("Card UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(" ");
      if (mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card Detected!");

    Serial.println(F("Reading last data from RFID..."));
    ReadDataFromBlock(blockNum, readBlockData);

    Serial.print(F("Last data in RFID block "));
    Serial.print(blockNum);
    Serial.print(F(" --> "));
    for (int j = 0; j < 16; j++) {
      Serial.write(readBlockData[j]);
    }
    Serial.println();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hey " + String((char*)readBlockData) + "!");

    // Beep Buzzer
    for(int i=0; i<2; i++) {
      digitalWrite(BUZZER, HIGH); delay(200);
      digitalWrite(BUZZER, LOW);  delay(200);
    }

    // Step 3: Send to Google Sheets
    if (WiFi.status() == WL_CONNECTED) {
      String userName = "";
for (int i = 0; i < 16; i++) {
  if (readBlockData[i] != 0) {
    userName += (char)readBlockData[i];
  }
}
userName.trim();

// UNAUTHORIZED ACCESS STOPPAGE 

userName.toUpperCase();
authorizedUser.toUpperCase();

// 🔐 Identity check
if (userName != authorizedUser) {
  Serial.println("Identity Mismatch!");

  lcd.setCursor(0, 1);
  lcd.print("Not Authorized   ");

  return;  // stop further execution
}

// 🔁 HYBRID LOGIN-LOGOUT LOGIC
if (!isLoggedIn) {
  // ================= LOGIN =================
  currentUser = userName;
  loginTime = millis();
  lastScanTime = millis();
  isLoggedIn = true;

  Serial.println("[INFO] LOGIN SUCCESS");

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Login Success");

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  String url = sheet_url + "?name=" + userName + "&type=login";
  Serial.println("Sending data to Google Sheets...");

  HTTPClient https;
  if (https.begin(*client, url)) {
    int httpCode = https.GET();
    Serial.printf("Login Code: %d\n", httpCode);
    https.end();
  }
}

else if (userName == currentUser) {
  // ================= LOGOUT =================
  unsigned long logoutTime = millis();
  unsigned long duration = (logoutTime - loginTime) / 1000;

  lastScanTime = millis();
  

  Serial.println("[INFO] LOGOUT SUCCESS");
  Serial.print("Duration (sec): ");
  Serial.println(duration);
 

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Logout Success");

  isLoggedIn = false;
  currentUser = "";
  sessionCompleted = true;   // 🔥 STOP further scanning
  rfidEnabled = false;

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  String url = sheet_url + "?name=" + userName + "&type=logout&duration=" + String(duration);
  Serial.println("Sending data to Google Sheets...");

  HTTPClient https;
  if (https.begin(*client, url)) {
    int httpCode = https.GET();
    Serial.printf("Logout Code: %d\n", httpCode);
    https.end();
  }
    Serial.println("SESSION_COMPLETE");
    lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Session Complete");
lcd.setCursor(0, 1);
lcd.print("Restart System");
}

else {
  // ================= WRONG USER =================
  Serial.println("Different user detected!");

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Wrong Card!");
}
    }

    // ✅ Inform via Serial for debugging
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    // Step 4: Disable RFID until next face verification
    // rfidEnabled = false;
// Keep RFID ON for continuous scanning
   // Serial.println("Ready for next scan...");
    delay(1000);
  }
}

/****************************************************************************************************
 * ReadDataFromBlock() function
 ****************************************************************************************************/
void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK){
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  } else {
    Serial.println("Authentication success");
  }

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else {
    Serial.println("Block was read successfully");  
  }
}