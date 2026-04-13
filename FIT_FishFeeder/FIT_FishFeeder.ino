// Developer: Apirak Tooltham
// IoTES LAB, Nakhon Phanom University
// 21 May 2023
// Version 1.3

// Timer 
#include "secrets.h"
#include <millisDelay.h>
millisDelay feederDelay;

// Line Notify
#include <TridentTD_LineNotify.h>

// --- Configuration Constants ---
const char* WIFI_NAME = WIFI_SSID;
const char* WIFI_PASS = WIFI_PASSWORD;

// --- Food Level Thresholds (cm) ---
const float DIST_LIMIT_LOW   = 8.0;   // ปริมาณเริ่มน้อย
const float DIST_LIMIT_EMPTY = 15.0;  // ปริมาณหมด

// --- Hardware Pins ---
const int PIN_TRIG = 5;
const int PIN_ECHO = 18;

// --- Constants ---
#define SOUND_SPEED 0.034     // cm/uS
#define CM_TO_INCH 0.393701

// --- Moving Average ---
const int AVG_SAMPLES = 5;
float distanceSamples[AVG_SAMPLES];
int sampleIndex = 0;

// --- Global Variables ---
long duration;
float distanceCm;
float distanceAvg;

// --- State Management ---
enum FoodLevel { LEVEL_OK, LEVEL_LOW, LEVEL_EMPTY, LEVEL_UNKNOWN };
FoodLevel currentLevel = LEVEL_UNKNOWN;
FoodLevel lastLevel = LEVEL_UNKNOWN;


void setup() {
  Serial.begin(115200);
  Serial.println(LINE.getVersion());
  
  initWiFi();

  LINE.setToken(LINE_NOTIFY_TOKEN);
  LINE.notify("Fish Feeder: เริ่มการทำงาน");

  feederDelay.start(2000);
  
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void initWiFi() {
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  Serial.printf("WiFi connecting to %s\n", WIFI_NAME);
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
    Serial.print(".");
    delay(500);
    retryCount++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWiFi connected\nIP : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed! Will retry in loop.");
  }
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Reconnecting...");
    WiFi.disconnect();
    WiFi.begin(WIFI_NAME, WIFI_PASS);
  }
}

void checkDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  duration = pulseIn(PIN_ECHO, HIGH, 30000); 
  if (duration == 0) {
    Serial.println("Sensor Error: Pulse timeout");
    return;
  }
  
  distanceCm = duration * SOUND_SPEED / 2;
  
  // Update Moving Average
  distanceSamples[sampleIndex] = distanceCm;
  sampleIndex = (sampleIndex + 1) % AVG_SAMPLES;
  
  float sum = 0;
  for(int i=0; i<AVG_SAMPLES; i++) sum += distanceSamples[i];
  distanceAvg = sum / AVG_SAMPLES;

  Serial.printf("Distance: %.2f cm (Avg: %.2f cm)\n", distanceCm, distanceAvg);
}

void checkFoodLevel() {
  // Determine current level using distanceAvg
  if (distanceAvg < DIST_LIMIT_LOW) {
    currentLevel = LEVEL_OK;
  } else if (distanceAvg <= DIST_LIMIT_EMPTY) {
    currentLevel = LEVEL_LOW;
  } else {
    currentLevel = LEVEL_EMPTY;
  }

  // Notify only when level changes
  if (currentLevel != lastLevel) {
    switch (currentLevel) {
      case LEVEL_OK:
        Serial.println("State: Food Level OK");
        LINE.notifySticker("อาหารปลาอยู่ในระดับปกติ", 2, 179);
        break;
      case LEVEL_LOW:
        Serial.println("State: Food Level LOW");
        LINE.notifySticker("อาหารปลาเหลือน้อยแล้ว", 2, 39);
        break;
      case LEVEL_EMPTY:
        Serial.println("State: Food Level EMPTY");
        LINE.notifySticker("อาหารปลาหมดแล้ว", 2, 43);
        break;
    }
    lastLevel = currentLevel;
  }
}

void loop() {
  if (feederDelay.justFinished()) {
    feederDelay.repeat();
    checkWiFi();      // ตรวจสอบการเชื่อมต่อ WiFi
    checkDistance();  // วัดระยะ
    checkFoodLevel(); // วิเคราะห์ระดับอาหารและแจ้งเตือน
  }
}

