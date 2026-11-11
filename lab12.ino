#include "M5Unified.h"

const size_t BUFFER_SIZE = 256;
const int SAMPLE_RATE = 4000;
int16_t buffer[BUFFER_SIZE];
int buffer_old[BUFFER_SIZE];
float rms_max = 0.0f;

void setup() {
  M5.begin();
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);

  if (!M5.Mic.begin()){
    M5.Lcd.println("Mic init fail!");
    while (1) delay(100);
    }
    M5.Mic.setSampleRate(SAMPLE_RATE);
}

void changeColor() {
  long long sum_sqr = 0;
  long sum_raw = 0;
  M5.update();
  
  for (size_t i = 0; i < BUFFER_SIZE; i++){
    sum_raw += buffer[i];
    }
    
  int16_t dc_offset = sum_raw / BUFFER_SIZE;
  
  for (size_t i = 0; i < BUFFER_SIZE; i++) {
    int32_t corrected_sample = buffer[i] - dc_offset;
    sum_sqr += (int64_t)corrected_sample * corrected_sample;
    }

  float mean_square = (float)sum_sqr / BUFFER_SIZE;
  float rms = sqrt(mean_square);

  if (M5.BtnA.wasPressed()) {
      rms_max = 0.0f;
    }
  
  if (rms > rms_max){
    rms_max = rms;
   }
  M5.Lcd.clear();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.printf("RMS: %.2f", rms);
  M5.Lcd.printf("RMS max: %.2f", rms_max);
 
  if (rms > 16000.){
    M5.Lcd.fillScreen(YELLOW);
  }
}

void showSignal() {
  int y;
  for (int n = 0; n < 239; n++){
    y = buffer[n];
    y = map(y, INT16_MIN, INT16_MAX, 10, 130);
    M5.Lcd.drawPixel(n, buffer_old[n], BLACK);
    M5.Lcd.drawPixel(n, y, WHITE);
    buffer_old[n] = y;
    }
}

void loop() {
  M5.update();
  bool ok = M5.Mic.record(buffer, BUFFER_SIZE);
  if (!ok) return;
  while (M5.Mic.isRecording()) delay(5);
  changeColor();
}
