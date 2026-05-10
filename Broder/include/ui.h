#pragma once
#include "globals.h"

void drawStars();
void drawHeader(const char* title);
void drawButton(int x, int y, int w, int h,
                const char* label, uint16_t col = COL_BTN);
void btnPressAnim(int x, int y, int w, int h, const char* label);
void drawStatus(const char* msg, uint16_t col = COL_TEXT);
void drawProgressBar(int x, int y, int w, int h,
                     int pct, uint16_t col = COL_ACCENT);
void wipeTransition();
void goToScreen(Screen s, void(*drawFn)());
void drawBootScreen();
void drawMainMenu();
void breathingEffect();
bool touched();
bool inBox(int x, int y, int w, int h);
void handleTouch();
