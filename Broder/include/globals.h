#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <RCSwitch.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <NimBLEDevice.h>
#include "config.h"

// Hardware objects
extern TFT_eSPI  tft;
extern IRsend    irsend;
extern IRrecv    irrecv;
extern RCSwitch  rc;
extern NimBLEAdvertising* bleAdv;

// Signal storage (shared IR + SubGHz)
extern unsigned long  lastCapturedCode;
extern unsigned int   lastCapturedBits;
extern unsigned int   lastCapturedProto;
extern decode_results irResults;

// Screen state
enum Screen {
  MENU_MAIN,
  SCREEN_IR,
  SCREEN_SUBGHZ,
  SCREEN_WIFI,
  SCREEN_DEAUTH,
  SCREEN_BLESPAM,
  SCREEN_ABOUT
};
extern Screen currentScreen;

// Touch
extern uint16_t tx, ty;

// Breath
extern unsigned long lastBreath;
extern int  breathVal;
extern bool breathUp;

// Deauth
struct APTarget {
  String  ssid;
  uint8_t bssid[6];
  int32_t rssi;
  uint8_t channel;
};
extern APTarget targets[20];
extern int      targetCount;
extern int      selectedTarget;
extern bool     deauthing;
extern uint8_t  deauthPacket[26];

// BLE
extern const uint8_t applePayloads[][17];
extern const char*   payloadNames[];
extern int  currentPayload;
extern bool bleSpamActive;
