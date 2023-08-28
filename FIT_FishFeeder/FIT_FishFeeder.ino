// Developer: Apirak Tooltham
// IoTES LAB, Nakhon Phanom University
// 21 May 2023
// Version 1.3

// Timer 
#include <millisDelay.h>
millisDelay feederDelay;

// Line Notify
#include <TridentTD_LineNotify.h>
// ------------------------ ส่วนที่ต้องแก้ไขข้อมูล ------------------------
#define SSID        "IoT-LAB"                                       // Access Point ที่จะเชื่อมต่อ
#define PASSWORD    "1a2b3c4d5e"                                    // รหัส Wifi
#define LINE_TOKEN  "MOzwWOk18IvRssfXygic4fI4yQ4GAIT1pyTPGZ7citM"   // รหัส TOKEN ของ Line
// -----------------------------------------------------------------

// Ultrasonic Sensor
const int trigPin = 5;        // กำหนดขา Trigger เป็น GPIO 5
const int echoPin = 18;       // กำหนดขา Echo เป็น GPIO 18
#define SOUND_SPEED 0.034     // กำหนด sound speed หน่วย cm/uS
#define CM_TO_INCH 0.393701   // กำหนดค่าหน่วยเซนติเมตรแปลงเป็นนิ้ว
long duration;
float distanceCm;
float distanceInch;
boolean status_1 = 0;         // กำหนดสถานะเริ่มต้นอาหาร OK
boolean status_2 = 0;         // กำหนดสถานะเริ่มต้นอาหาร LOW
boolean status_3 = 0;         // กำหนดสถานะเริ่มต้นอาหาร EMPTY

void setup() {
  Serial.begin(115200);
  // Line Notify
  Serial.println(LINE.getVersion());
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);                  // กำหนด Line Token
  LINE.notify("Fish Feeder: เริ่มการทำงาน");     // ส่ง Line Totify แจ้งว่าระบบพร้อมทำงาน

  feederDelay.start(2000);                    // กำหนดเวลาในการ delay ของ Timer 2 วินาที
  
  pinMode(trigPin, OUTPUT);                   // กำหนด trigPin เป็น Output
  pinMode(echoPin, INPUT);                    // กำหนด echoPin เป็น Input
}

void checkDistance() {
  digitalWrite(trigPin, LOW);                 // เคลีย trigPin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);                // กำหนด trigPin เป็น HIGH 10 mS
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);          // อ่านค่าจาก echoPin หน่วยเป็น mS
  distanceCm = duration * SOUND_SPEED / 2;    // คำนวณระยะทาง หน่วยเป็นเซนติเมตร
  distanceInch = distanceCm * CM_TO_INCH;     // คำนวณระยะทาง หน่วยเป็นนิ้ว
  Serial.print("Distance (cm): ");            // แสดงผลออกทาง Serial Monitor
  Serial.println(distanceCm);
  // Serial.print("Distance (inch): ");
  // Serial.println(distanceInch);
}

void checkFoodLevel() {
  if ((distanceCm < 8) and (status_1 == 0)) {                                     // เงื่อนไขของการแจ้งเตือนอาหารเหลือปริมาณน้อย หน่วยเป็น cm
    Serial.println("Food Level is OK!!!");
    LINE.notifySticker("อาหารปลาอยู่ในระดับปกติ", 2, 179);                        
    status_1 = 1;
    status_2 = 0;
    status_3 = 0;
  } else if ((distanceCm >= 8) and (distanceCm <= 15) and (status_2 == 0)) {      // เงื่อนไขของการแจ้งเตือนอาหารเหลือน้อย หน่วยเป็น cm
    Serial.println("Food Level is LOW!!!");
    LINE.notifySticker("อาหารปลาเหลือน้อยแล้ว", 2, 39);                          
    status_1 = 0;
    status_2 = 1;
    status_3 = 0;
  } else if ((distanceCm > 15) and (status_3 == 0)) {                              // เงื่อนไขของการแจ้งเตือนอาหารหมด หน่วยเป็น cm
    Serial.println("Food Level is EMPTY!!!");
    LINE.notifySticker("อาหารปลาหมดแล้ว", 2, 43);                                   
    status_1 = 0;
    status_2 = 0;
    status_3 = 1;
  }
}

void loop() {
  if (feederDelay.justFinished()) {
    feederDelay.repeat();       // เริ่มการจับเวลาอีกครั้ง
    checkDistance();            // เรียกใช้ฟังก์ชั่นเพื่อเช็คระยะ
    checkFoodLevel();           // เรียกใช้ฟังก์ชั่นเช็คปริมาณอาหาร
  }
}
