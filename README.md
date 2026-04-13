# FIT_FishFeeder System V.1.3
## 💡 An Intelligent IoT Solution by IoTES LAB
![IoTES LAB - Nakhon Phanom University](https://i0.wp.com/fit.rmuti.ac.th/wp-content/uploads/2019/06/FIT2-1.png?fit=992%2C1162&ssl=1)

---

### 🏛️ Research Unit Information
**Developed by:** [IoTES LAB (Internet of Things and Embedded Systems Laboratory)](https://www.npu.ac.th)
**Affiliation:** Faculty of Industrial Technology (FIT), Nakhon Phanom University (NPU)
**Project Lead:** Apirak Tooltham

---

### 📝 Overview
**FIT_FishFeeder** คือระบบแจ้งเตือนระดับอาหารปลาอัตโนมัติที่ถูกออกแบบมาเพื่อความทนทานและความปลอดภัยสูงสุด โดยมุ่งเน้นการแก้ปัญหาการดูแลปลาในระยะยาวผ่านเครือข่ายอินเทอร์เน็ต (IoT) ซึ่งเป็นหนึ่งในผลงานนวัตกรรมต้นแบบของหน่วยวิจัย **IoTES LAB**

### 🚀 Key Technical Innovations
- **Advanced State Management:** ใช้ระบบสถานะแบบ Finite State Machine (Enum-based) เพื่อการทำงานที่แม่นยำ
- **Signal Processing:** เสริมระบบ **Moving Average Filter** สำหรับกรองสัญญาณ Noise จากเซ็นเซอร์ Ultrasonic (HC-SR04) ทำให้วัดค่าได้เสถียรกว่าระบบทั่วไป
- **Self-Healing Connectivity:** ระบบกู้คืนการเชื่อมต่อเครือข่ายอัตโนมัติ (WiFi Auto-Reconnect) มั่นใจได้ว่าข้อมูลจะไม่ขาดหาย
- **Secure Configuration:** สถาปัตยกรรมแบบแยกส่วนข้อมูลลับ (Secret Separation) เพื่อความปลอดภัยตามมาตรฐานความปลอดภัยทางไซเบอร์

### 🔌 Hardware & Wiring
| Module | Component | Pin (ESP32) | Function |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | ESP32 DevKit | - | Processing Unit |
| **Input Sensor** | HC-SR04 Ultrasonic | Trig(5), Echo(18) | Water/Food Level Detection |
| **Notification** | Line Notify API | Cloud | Remote Alerting |

### 🛠️ Installation Guide for Researchers
1. **Prerequisites:** 
   - ติดตั้ง Library: `TridentTD_LineNotify`, `millisDelay`
2. **Configuration:** 
   - สร้างไฟล์ `secrets.h` ในโฟลเดอร์โครงการ 
   - ใส่ข้อมูล SSID, Password และ Line Token ของคุณ
3. **Deployment:** 
   - Compile และ Upload ไปยังบอร์ด ESP32 ผ่าน Arduino IDE

---
### 🖼️ Design Documentation
![Library Reference 1](./images/Library-1.jpg)
![Library Reference 2](./images/Library-2.jpg)

### 📞 Contact & Collaboration
หากสนใจร่วมวิจัยหรือพัฒนาระบบร่วมกับเรา สามารถติดต่อได้ที่ **IoTES LAB, Nakhon Phanom University**

---
*Maintained and Audited by Mori Minami (Mina) - Engineering Advisor to IoTES LAB*
