#include "USB.h"
#include "USBHIDKeyboard.h"
#include "M5Unified.h"

USBHIDKeyboard Keyboard;

const char* fileName = "Poem-by-SakhibgareevV.A.";
const char* poem = 
    "U lukomorya dub zelyony;\n"
    "Zlataya cep na dube tom:\n"
    "I dnyom i nochyu kot uchyony\n"
    "Vsyo hodit po cepi krugom;\n"
    "Idyot napravo - pesn zavodit,\n"
    "Nalevo - skazku govorit.\n"
    "\n"
    "Tam chudesa, tam leshiy brodit,\n"
    "Rusalka na vetvyah sidit;\n"
    "Tam na nevedomyh dorozhkah\n"
    "Sledy nevidannyh zverey;\n"
    "Izbushka tam na kuryih nozhkah\n"
    "Stoit bez okon, bez dverey.\n"
    "\n"
    "           A.S. Pushkin\n";

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Keyboard.begin();
  USB.begin();

  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Display.setCursor(0, 10);
  M5.Display.println("HID Keyboard ready");
  M5.Display.println("Press [A]");
}

void typeText(const char* text, int delayMs) {
  while (*text) {
    Keyboard.write(*text);
    text++;
    delay(delayMs);
  }
}

void openNotepad() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
//  Keyboard.press(KEY_LEFT_ALT);
//  Keyboard.press(KEY_F2);
  delay(100);
  Keyboard.releaseAll();
  delay(800);

  typeText("notepad", 50);
//  typeText("gedit", 50);
  delay(300);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(2000);
}

void printPoem() {
  typeText(poem, 25);
  delay(500);
}

void saveFile(const char* Filename) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('s');
  delay(100);
  Keyboard.releaseAll();
  delay(1000);
  
  typeText(Filename, 50);
  delay(300);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(500);
}

void loop() {
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 10);
    M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Display.println("Working...");
    
    delay(1500);
    
    openNotepad();
    printPoem();
    saveFile(fileName);

    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 10);
    M5.Display.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Display.println("Done!");
    M5.Display.println("Press [A] again");
  }

  delay(20);
}
