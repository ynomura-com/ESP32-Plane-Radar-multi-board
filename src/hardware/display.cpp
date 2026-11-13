#include "hardware/display.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "config.h"
#include "hardware/display_font.h"

LGFX tft;

namespace {
SemaphoreHandle_t s_display_mutex = nullptr;
}

void displayLock() {
  if (s_display_mutex != nullptr) {
    xSemaphoreTakeRecursive(s_display_mutex, portMAX_DELAY);
  }
}

void displayUnlock() {
  if (s_display_mutex != nullptr) {
    xSemaphoreGiveRecursive(s_display_mutex);
  }
}

void displayInit() {
  if (s_display_mutex == nullptr) {
    s_display_mutex = xSemaphoreCreateRecursiveMutex();
  }
  const DisplayLock lock;
  tft.init();
  tft.setRotation(0);
  tft.setBrightness(static_cast<uint8_t>(config::kDisplayBrightness));
  tft.setTextWrap(false);
  displayFontInit();
}
