#include "M5Unified.h"

int lastX = 67;
int lastY = 120;
const int centerX = 67;
const int centerY = 120;
const int sensitivity = 150; 

void setup(void) {
  M5.begin();
  M5.Lcd.drawCircle(centerX, centerY, 25, GREEN);
}

void loop(void) {
  M5.update();
  if (M5.Imu.update()){
    
    auto ImuData = M5.Imu.getImuData();
    int newX = centerX - (ImuData.accel.x * sensitivity);
    int newY = centerY + (ImuData.accel.y * sensitivity);
    
    if (newX != lastX || newY != lastY){
      M5.Lcd.fillCircle(lastX, lastY, 24, BLACK);
      M5.Lcd.drawCircle(centerX, centerY, 25, GREEN);
      M5.Lcd.fillCircle(newX, newY, 24, YELLOW);

      lastX = newX;
      lastY = newY;
    }
  }
  
  delay(20);
}
