# Racquet-Head-Velocity-Analyser-RHVA-
This is what an incredible first project feels like! Combining passion for sports with technical learning is the best way to innovate. The **Racquet Head Velocity Analyser [RHVA]** addresses a crucial aspect of badminton—differentiating between power shots and drop shots—that is typically only analyzed by expensive, high-frame-rate video systems.

Here's a summary of the technical aspects:

**💡 Project Overview: RHVA**

**Objective:** Measure badminton racquet head velocity, acceleration, and orientation (x, y, z axes) during a swing to distinguish between power shots and drop shots.

**Domain:** IoT Sports Analytics / Smart Wearables/ Wireless Communication

**🛠️ Components & Implementation**

**Sensor (MPU6050):** Measures 3-axis acceleration and angular velocity (gyroscope). Essential for detecting the racquet's rotation (roll, pitch, yaw) and speed.

**Microcontroller (ESP32-C3 Super Mini):** Ideal for this project due to its compact size, low weight, and built-in Wi-Fi/Bluetooth for sending data to the Blynk app.

**Integration:** MPU6050 reads raw motion data ESP32-C3 processes data and transmits to Blynk app via Wi-Fi. 

**Li-Po battery:** Powers the system   

**TP4056 Module:** Provides safe charging for the Li-Po battery   

---

**🧠 Core Concepts & Algorithm**

**Sensor Fusion:** For accurate orientation and acceleration, the system combines accelerometer data (gravity) and gyroscope data (rotation). This separates gravity's effect to isolate actual motion.

**Velocity Estimation:** The system integrates acceleration data over time to estimate instantaneous velocity and peak swing speed. 

**⚠️ Challenges & Limitations**

**Weight Constraints:** Adding components to a racquet alters its balance. A lighter, more compact 3D-printed enclosure is essential.

**Calibration:** The MPU6050's accuracy depends heavily on proper calibration to remove sensor offsets.

**Connectivity:** Real-time data visualization via Blynk requires a stable Wi-Fi connection, which may lag during fast-paced play. 

**🚀 Future Enhancements**

**ESP32-CAM:** Integrate a camera alongside the motion sensor to visually track swing technique, creating a "smart racket" view.

**PVDF Sensors:** Thin, flexible polyvinylidene fluoride (PVDF) sensors on the strings can detect vibration upon impact to determine the precise moment of contact, not just swing speed. 

I finally would like to thank my teammates for bearing me and my mood swings, but altogether we did it as a team, everybody had given their at most to this project and it couldn't have been done if it weren’t done together.
Team Members of this projects were :

