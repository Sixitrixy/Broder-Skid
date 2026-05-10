#include "about.h"
#include "ui.h"

void drawAbout() {
  drawHeader(BRODER_NAME);

  tft.setTextColor(COL_ACCENT, COL_BG);
  tft.setTextSize(1);
  tft.setCursor(8, 50);
  tft.println("  BRODER v" BRODER_VER);
  tft.println("  ────────────────────────");

  tft.setTextColor(COL_TEXT, COL_BG);
  tft.println("  ESP32-2432S028  (CYD)");
  tft.println("  IR TX / RX");
  tft.println("  CC1101 Sub-GHz 433MHz");
  tft.println("  WiFi Scanner");
  tft.println("  Deauth (own network)");
  tft.println("  BLE Apple Proximity");
  tft.println("");

  tft.setTextColor(COL_ACCENT, COL_BG);
  tft.println("  [ galaxy edition ]");

  tft.fillRect(0, 310, TFT_W, 10, COL_ACCENT2);
  drawButton(60, 240, 120, 40, "< Back");
}
