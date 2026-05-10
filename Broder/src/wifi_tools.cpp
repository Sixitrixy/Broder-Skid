#include "wifi_tools.h"
#include "ui.h"

void drawWiFiScreen() {
  drawHeader("WiFi Scanner");
  drawButton(10,  55,  220, 48, "Scan Networks");
  drawButton(60,  175, 120, 40, "< Back");
}

void wifiScan() {
  drawStatus("Scanning...", COL_WARN);
  WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks();
  tft.fillRect(0, 45, TFT_W, 240, COL_BG);
  tft.setTextSize(1);
  int shown = min(n, 14);
  for (int i = 0; i < shown; i++) {
    uint16_t col = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
                   ? COL_WARN : COL_TEXT;
    tft.setTextColor(col, COL_BG);
    tft.setCursor(4, 48 + i*17);
    tft.printf("%-16s %4ddBm %s",
               WiFi.SSID(i).substring(0,14).c_str(),
               WiFi.RSSI(i),
               WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "OPEN" : "ENC");
  }
  if (n > 14) {
    tft.setTextColor(COL_ACCENT2, COL_BG);
    tft.setCursor(4, 48 + 14*17);
    tft.printf("...+%d more", n - 14);
  }
  drawStatus("Done. Touch to return.", COL_GREEN);
  delay(5000);
  drawWiFiScreen();
}
