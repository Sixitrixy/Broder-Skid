#include "ble_spam.h"
#include "ui.h"

void initBLE() {
  NimBLEDevice::init("Broder");
  bleAdv = NimBLEDevice::getAdvertising();
}

void drawBLESpamScreen() {
  drawHeader("BLE Apple Spam");
  drawButton(10,  55,  220, 48, payloadNames[currentPayload]);
  drawButton(10,  115, 220, 48, "Next Payload");
  drawButton(10,  175, 220, 48,
             bleSpamActive ? "  STOP  " : "  START ",
             bleSpamActive ? COL_RED : COL_BTN);
  drawButton(60,  240, 120, 40, "< Back");

  tft.setTextSize(1);
  tft.setTextColor(bleSpamActive ? COL_GREEN : COL_ACCENT2, COL_BG);
  tft.setCursor(4, 228);
  tft.printf("Status: %s", bleSpamActive ? "BROADCASTING" : "idle");
}

void sendAppleAdvert(int idx) {
  if (!bleAdv) return;
  bleAdv->stop();
  NimBLEAdvertisementData advData;
  std::string payload(
    reinterpret_cast<const char*>(applePayloads[idx]),
    17
  );
  advData.addData(payload);
  bleAdv->setAdvertisementData(advData);
  bleAdv->setMinInterval(0x20);
  bleAdv->setMaxInterval(0x20);
  bleAdv->start();
}
