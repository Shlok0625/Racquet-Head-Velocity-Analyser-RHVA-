/*
 * Racket Head Velocity Analyser
 * Board: ESP32-C3 Super Mini
 * Sensor: MPU6050 (I2C: SDA=8, SCL=9)
 * Display: Serial Monitor + Blynk App
 */
#define BLYNK_TEMPLATE_ID "TMPL3dcvDUZZY"
#define BLYNK_TEMPLATE_NAME "Racket Head Velocity Analyzer"
#define BLYNK_AUTH_TOKEN "G4B4rJ7mJC5Ej4F3H4KGNYjr0ZgLCgQY"

// Comment this out if you use local Blynk server
#define BLYNK_FIRMWARE_VERSION "1.0.0"

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>     // ← New Blynk library for ESP32 (2024+)

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
// ──────────────────────────────────────────────
// PIN DEFINITIONS for ESP32-C3 Super Mini
// ──────────────────────────────────────────────
#define I2C_SDA  8
#define I2C_SCL  9

// ──────────────────────────────────────────────
// WiFi Credentials
// ──────────────────────────────────────────────
char ssid[] = "Shlok's M12";
char pass[] = "ghva2778";

// ──────────────────────────────────────────────
// Global objects & variables
// ──────────────────────────────────────────────
Adafruit_MPU6050 mpu;

float velocityX = 0, velocityY = 0, velocityZ = 0;
float velocityTotal = 0;
unsigned long lastTime = 0;

float offsetX = 0, offsetY = 0, offsetZ = 0;
const float NOISE_THRESHOLD = 0.4;     // m/s² - tune this
const float DECAY_FACTOR = 0.96;       // helps reset velocity when still

// ──────────────────────────────────────────────
// SETUP
// ──────────────────────────────────────────────
void setup() 
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\nRacket Head Velocity Analyser starting...");

  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA, I2C_SCL);

  // Try to find and initialize MPU6050
  if (!mpu.begin(0x68)) {   // 0x68 = default address
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }

  Serial.println("MPU6050 Found!");

  // Configure for racket swings (high acceleration)
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("Keep device completely still for calibration (5 seconds)...");
  delay(1000);
  calibrateSensor();
  Serial.println("Calibration finished.");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // If connection fails it will keep trying automatically

  lastTime = millis();
}

// ──────────────────────────────────────────────
// Simple calibration - average 100 samples when still
// ──────────────────────────────────────────────
void calibrateSensor() 
{
  float sumX = 0, sumY = 0, sumZ = 0;
  const int samples = 120;

  for(int i = 0; i < samples; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    sumX += a.acceleration.x;
    sumY += a.acceleration.y;
    sumZ += a.acceleration.z;
    delay(40);
  }

  offsetX = sumX / samples;
  offsetY = sumY / samples;
  offsetZ = sumZ / samples;

  Serial.printf("Offsets → X:%.2f  Y:%.2f  Z:%.2f\n", offsetX, offsetY, offsetZ);
}

// ──────────────────────────────────────────────
// MAIN LOOP
// ──────────────────────────────────────────────
void loop() 
{
  Blynk.run();   // MUST be called frequently

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // Remove gravity + offset
  float ax = a.acceleration.x - offsetX;
  float ay = a.acceleration.y - offsetY;
  float az = a.acceleration.z - offsetZ;

  // Only integrate significant acceleration (reduces drift)
  if (abs(ax) > NOISE_THRESHOLD) velocityX += ax * dt;
  if (abs(ay) > NOISE_THRESHOLD) velocityY += ay * dt;
  if (abs(az) > NOISE_THRESHOLD) velocityZ += az * dt;

  // Decay velocity when almost no acceleration detected
  if (abs(ax) < NOISE_THRESHOLD && abs(ay) < NOISE_THRESHOLD && abs(az) < NOISE_THRESHOLD) {
    velocityX *= DECAY_FACTOR;
    velocityY *= DECAY_FACTOR;
    velocityZ *= DECAY_FACTOR;
  }

  velocityTotal = sqrt(velocityX*velocityX + velocityY*velocityY + velocityZ*velocityZ);

  // ─── Serial output ───────────────────────────────
  Serial.print("Acc: ");
  Serial.print(sqrt(ax*ax + ay*ay + az*az), 2);
  Serial.print(" m/s²   |  Vel: ");
  Serial.print(velocityTotal, 2);
  Serial.println(" m/s");

  // ─── Send to Blynk (every ~200ms is usually fine) ─────
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 220) {
    Blynk.virtualWrite(V0, velocityTotal);        // Main velocity
    Blynk.virtualWrite(V1, sqrt(ax*ax + ay*ay + az*az)); // Acc magnitude
    Blynk.virtualWrite(V2, ax);
    Blynk.virtualWrite(V3, ay);
    Blynk.virtualWrite(V4, az);
    lastSend = millis();
  }

  delay(30);   // ~30-35 Hz loop rate
}