#include "subghz.h"
#include "ui.h"

void drawSubGHzScreen() {
  drawHeader("Sub-GHz");
  drawButton(10,  55,  105, 48, "Capture");
  drawButton(125, 55,  105, 48, "Replay");
  drawButton(60,  115, 120, 48, "Scan");
  drawButton(60,  175, 120, 40, "< Back");

  if (lastCapturedCode) {
    tft.setTextColor(COL_WARN, COL_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 228);
    tft.printf("Saved: %lu (%dbit P%d)",
               lastCapturedCode, lastCapturedBits, lastCapturedProto);
  }
}

void subCapture() {
  drawStatus("Listening 433MHz...", COL_WARN);
  rc.enableReceive(CC1101_GDO0);
  unsigned long timeout = millis() + 8000;
  while (millis() < timeout) {
    if (rc.available()) {
      lastCapturedCode  = rc.getReceivedValue();
      lastCapturedBits  = rc.getReceivedBitlength();
      lastCapturedProto = rc.getReceivedProtocol();
      rc.resetAvailable();
      char buf[40];
      snprintf(buf, sizeof(buf), "Got: %lu", lastCapturedCode);
      drawStatus(buf, COL_GREEN);
      delay(1500);
      rc.disableReceive();
      drawSubGHzScreen();
      return;
    }
  }
  rc.disableReceive();
  drawStatus("Nothing received", COL_RED);
  delay(1200);
  drawSubGHzScreen();
}

void subReplay() {
  if (!lastCapturedCode) {
    drawStatus("Nothing captured!", COL_RED);
    delay(1200);
    return;
  }
  rc.enableTransmit(CC1101_GDO0);
  rc.setProtocol(lastCapturedProto ? lastCapturedProto : 1);
  rc.send(lastCapturedCode, lastCapturedBits ? lastCapturedBits : 24);
  rc.disableTransmit();
  drawStatus("Replayed!", COL_GREEN);
  delay(1000);
}
