#pragma once

#include "hardware/lgfx_config.hpp"

extern LGFX tft;

void displayInit();

/**
 * Recursive lock around every use of `tft` after displayInit(). The radar sweep
 * runs in its own task, so all panel drawing must hold this lock.
 */
void displayLock();
void displayUnlock();

class DisplayLock {
 public:
  DisplayLock() { displayLock(); }
  ~DisplayLock() { displayUnlock(); }
  DisplayLock(const DisplayLock&) = delete;
  DisplayLock& operator=(const DisplayLock&) = delete;
};
