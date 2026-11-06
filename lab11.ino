#include "M5Unified.h"

int lastX = 67;
int lastY = 120;
const int centerX = 67;
const int centerY = 120;
const int sensitivity = 150; 

bool soundEnabled = true; 

void setup(void) {
  M5.begin();
  M5.Lcd.drawCircle(centerX, centerY, 25, GREEN);
}

void loop(void) {
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    soundEnabled = !soundEnabled;
    if (!soundEnabled) {
      M5.Speaker.stop();
    }
  }

  if (M5.Imu.update()){
    auto ImuData = M5.Imu.getImuData();
    int newX = centerX - (ImuData.accel.x * sensitivity);
    int newY = centerY + (ImuData.accel.y * sensitivity);
    
    if (newX != lastX || newY != lastY){
      M5.Lcd.fillCircle(lastX, lastY, 24, BLACK);
      M5.Lcd.drawCircle(centerX, centerY, 25, GREEN);
      M5.Lcd.fillCircle(newX, newY, 24, YELLOW);
      
      int distanceFromCenter = sqrt(pow(newX - centerX, 2) + pow(newY - centerY, 2));
      
      if (distanceFromCenter > 10 && soundEnabled) {
        M5.Speaker.tone(7000, 50);
        delay(100);
        M5.Speaker.tone(4000, 50);
        delay(100);
      }

      lastX = newX;
      lastY = newY;
    }
  }
  
  delay(20);
}
