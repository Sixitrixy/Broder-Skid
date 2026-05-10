#include "ir.h"
#include "ui.h"

void drawIRScreen() {
  drawHeader("IR Blaster");
  drawButton(10,  55,  105, 48, "Capture");
  drawButton(125, 55,  105, 48, "Replay");
  drawButton(10,  115, 105, 48, "NEC Test");
  drawButton(125, 115, 105, 48, "SONY Test");
  drawButton(60,  175, 120, 40, "< Back");

  if (lastCapturedCode) {
    tft.setTextColor(COL_WARN, COL_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 228);
    tft.printf("Saved: 0x%08lX (%db P%d)",
               lastCapturedCode, lastCapturedBits, lastCapturedProto);
  }
}

void irCapture() {
  drawStatus("Waiting for IR...", COL_WARN);
  irrecv.enableIRIn();
  unsigned long timeout = millis() + 8000;
  while (millis() < timeout) {
    if (irrecv.decode(&irResults)) {
      lastCapturedCode  = irResults.value;
      lastCapturedBits  = irResults.bits;
      lastCapturedProto = (int)irResults.decode_type;
      irrecv.resume();
      char buf[48];
      snprintf(buf, sizeof(buf), "Got: 0x%08lX %db",
               lastCapturedCode, lastCapturedBits);
      drawStatus(buf, COL_GREEN);
      delay(1500);
      drawIRScreen();
      return;
    }
  }
  drawStatus("Timeout - no signal", COL_RED);
  delay(1200);
  drawIRScreen();
}

void irReplay() {
  if (!lastCapturedCode) {
    drawStatus("Nothing captured yet!", COL_RED);
    delay(1200);
    return;
  }
  drawStatus("Sending...", COL_WARN);
  irsend.sendNEC(lastCapturedCode, lastCapturedBits);
  drawStatus("Sent!", COL_GREEN);
  delay(1000);
}
