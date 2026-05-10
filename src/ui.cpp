#include "ui.h"
#include "ir.h"
#include "subghz.h"
#include "wifi_tools.h"
#include "deauth.h"
#include "ble_spam.h"
#include "about.h"

void drawStars() {
  randomSeed(42);
  for (int i = 0; i < 80; i++) {
    uint16_t col = random(3) == 0 ? 0xC618 : COL_WHITE;
    tft.drawPixel(random(TFT_W), random(TFT_H), col);
  }
}

void drawHeader(const char* title) {
  tft.fillScreen(COL_BG);
  drawStars();
  tft.fillRect(0,  0, TFT_W, 18, COL_ACCENT2);
  tft.fillRect(0, 18, TFT_W, 18, COL_ACCENT);
  tft.setTextColor(COL_WHITE, COL_ACCENT);
  tft.setTextSize(2);
  tft.setCursor(8, 10);
  tft.print(title);
}

void drawButton(int x, int y, int w, int h,
                const char* label, uint16_t col) {
  tft.drawRoundRect(x-2, y-2, w+4, h+4, 8, COL_ACCENT2);
  tft.drawRoundRect(x-1, y-1, w+2, h+2, 7, COL_ACCENT);
  tft.fillRoundRect(x,   y,   w,   h,   6, col);
  tft.drawFastHLine(x+4, y+2, w-8,      COL_ACCENT2);
  tft.setTextColor(COL_TEXT, col);
  tft.setTextSize(2);
  tft.setCursor(x + (w - (int)strlen(label)*12)/2,
                y + (h - 16)/2);
  tft.print(label);
}

void btnPressAnim(int x, int y, int w, int h, const char* label) {
  tft.fillRoundRect(x, y, w, h, 6, COL_ACCENT);
  tft.setTextColor(COL_BG, COL_ACCENT);
  tft.setTextSize(2);
  tft.setCursor(x + (w - (int)strlen(label)*12)/2,
                y + (h - 16)/2);
  tft.print(label);
  delay(110);
  drawButton(x, y, w, h, label);
}

void drawStatus(const char* msg, uint16_t col) {
  tft.fillRect(0, 285, TFT_W, 25, COL_ACCENT2);
  tft.drawFastHLine(0, 285, TFT_W, COL_ACCENT);
  tft.setTextColor(col, COL_ACCENT2);
  tft.setTextSize(1);
  tft.setCursor(6, 293);
  tft.print(msg);
}

void drawProgressBar(int x, int y, int w, int h, int pct, uint16_t col) {
  tft.drawRoundRect(x, y, w, h, 4, COL_ACCENT2);
  int fill = (w - 4) * pct / 100;
  if (fill > 0)
    tft.fillRoundRect(x+2, y+2, fill, h-4, 3, col);
}

void wipeTransition() {
  for (int y = 36; y < TFT_H; y += 16) {
    tft.fillRect(0, y, TFT_W, 16, COL_ACCENT2);
    delay(5);
  }
  delay(35);
}

void goToScreen(Screen s, void(*drawFn)()) {
  wipeTransition();
  currentScreen = s;
  drawFn();
}

void drawBootScreen() {
  tft.fillScreen(COL_BG);
  drawStars();

  for (int r = 56; r > 0; r -= 4) {
    tft.drawCircle(120, 128, r, COL_ACCENT2);
    delay(14);
  }
  for (int r = 4; r <= 56; r += 4) {
    tft.drawCircle(120, 128, r, COL_ACCENT);
    delay(14);
  }

  tft.setTextColor(COL_WHITE, COL_BG);
  tft.setTextSize(3);
  tft.setCursor(40, 110);
  tft.print(BRODER_NAME);

  tft.setTextColor(COL_ACCENT, COL_BG);
  tft.setTextSize(1);
  tft.setCursor(58, 148);
  tft.print("[ galaxy edition ]");

  tft.setTextColor(COL_ACCENT2, COL_BG);
  tft.setCursor(68, 215);
  tft.print("initializing...");

  for (int p = 0; p <= 100; p += 4) {
    drawProgressBar(30, 230, 180, 12, p);
    delay(22);
  }

  tft.setTextColor(COL_ACCENT, COL_BG);
  tft.setCursor(82, 255);
  tft.print("[ ready ]");
  delay(700);
}

void drawMainMenu() {
  drawHeader(BRODER_NAME);
  tft.setTextColor(COL_ACCENT, COL_BG);
  tft.setTextSize(1);
  tft.setCursor(52, 40);
  tft.print("[ RF  //  IR  //  NET ]");

  drawButton(10,  55,  105, 48, "IR");
  drawButton(125, 55,  105, 48, "Sub-GHz");
  drawButton(10,  115, 105, 48, "WiFi");
  drawButton(125, 115, 105, 48, "Deauth");
  drawButton(10,  175, 105, 48, "BLE Spam");
  drawButton(125, 175, 105, 48, "About");

  tft.fillRect(0, 310, TFT_W, 10, COL_ACCENT2);
  tft.setTextColor(COL_ACCENT2, COL_BG);
  tft.setTextSize(1);
  tft.setCursor(60, 298);
  tft.print("BRODER v" BRODER_VER);
}

void breathingEffect() {
  if (currentScreen != MENU_MAIN) return;
  if (millis() - lastBreath < 28) return;
  lastBreath = millis();
  uint16_t col = tft.color565(breathVal/4, 0, breathVal);
  tft.fillRect(0, 310, TFT_W, 10, col);
  if (breathUp) { breathVal += 5; if (breathVal >= 250) breathUp = false; }
  else          { breathVal -= 5; if (breathVal <= 60)  breathUp = true;  }
}

bool touched() { return false; }

bool inBox(int x, int y, int w, int h) {
  return tx >= (uint16_t)x && tx <= (uint16_t)(x+w) &&
         ty >= (uint16_t)y && ty <= (uint16_t)(y+h);
}

void handleTouch() {
  if (!touched()) return;
  delay(80);

  switch (currentScreen) {

    case MENU_MAIN:
      if (inBox(10,55,105,48))   { btnPressAnim(10,55,105,48,"IR");        goToScreen(SCREEN_IR,     drawIRScreen);     }
      if (inBox(125,55,105,48))  { btnPressAnim(125,55,105,48,"Sub-GHz");  goToScreen(SCREEN_SUBGHZ, drawSubGHzScreen); }
      if (inBox(10,115,105,48))  { btnPressAnim(10,115,105,48,"WiFi");     goToScreen(SCREEN_WIFI,   drawWiFiScreen);   }
      if (inBox(125,115,105,48)) { btnPressAnim(125,115,105,48,"Deauth");  goToScreen(SCREEN_DEAUTH, drawDeauthScreen); }
      if (inBox(10,175,105,48))  { btnPressAnim(10,175,105,48,"BLE Spam"); goToScreen(SCREEN_BLESPAM,drawBLESpamScreen);}
      if (inBox(125,175,105,48)) { btnPressAnim(125,175,105,48,"About");   goToScreen(SCREEN_ABOUT,  drawAbout);        }
      break;

    case SCREEN_IR:
      if (inBox(10,55,105,48))   irCapture();
      if (inBox(125,55,105,48))  irReplay();
      if (inBox(10,115,105,48))  { irsend.sendNEC(0x20DF10EF,32);  drawStatus("NEC sent",COL_GREEN);  delay(800); }
      if (inBox(125,115,105,48)) { irsend.sendSony(0xA90,12);      drawStatus("SONY sent",COL_GREEN); delay(800); }
      if (inBox(60,175,120,40))  { btnPressAnim(60,175,120,40,"< Back"); goToScreen(MENU_MAIN,drawMainMenu); }
      break;

    case SCREEN_SUBGHZ:
      if (inBox(10,55,105,48))  subCapture();
      if (inBox(125,55,105,48)) subReplay();
      if (inBox(60,175,120,40)) { btnPressAnim(60,175,120,40,"< Back"); goToScreen(MENU_MAIN,drawMainMenu); }
      break;

    case SCREEN_WIFI:
      if (inBox(10,55,220,48))  wifiScan();
      if (inBox(60,175,120,40)) { btnPressAnim(60,175,120,40,"< Back"); goToScreen(MENU_MAIN,drawMainMenu); }
      break;

    case SCREEN_DEAUTH:
      if (inBox(10,226,105,36))  { scanForTargets(); }
      if (inBox(125,226,105,36)) { selectedTarget = (selectedTarget+1) % max(targetCount,1); drawTargetList(); }
      if (inBox(10,270,105,40))  { deauthing = !deauthing; drawDeauthScreen(); }
      if (inBox(125,270,105,40)) { deauthing = false; btnPressAnim(125,270,105,40,"< Back"); goToScreen(MENU_MAIN,drawMainMenu); }
      break;

    case SCREEN_BLESPAM:
      if (inBox(10,115,220,48)) { currentPayload = (currentPayload+1)%4; drawBLESpamScreen(); }
      if (inBox(10,175,220,48)) { bleSpamActive = !bleSpamActive; if (!bleSpamActive) bleAdv->stop(); drawBLESpamScreen(); }
      if (inBox(60,240,120,40)) { bleSpamActive = false; bleAdv->stop(); btnPressAnim(60,240,120,40,"< Back"); goToScreen(MENU_MAIN,drawMainMenu); }
      break;

    case SCREEN_ABOUT:
      if (inBox(60,240,120,40)) { btnPressAnim(60,240,120,40,"< Back"); goToScreen(MENU_MAIN,drawMainMenu); }
      break;
  }
}
