#pragma once

#include <cstdint>

#include <driver/gpio.h>

namespace config {

// --- Wi-Fi portal ---
constexpr char kPortalApName[] = "PlaneRadar-Setup";
constexpr char kPortalIp[] = "192.168.4.1";
/** mDNS host (no ".local" suffix); browser: http://plane-radar.local */
constexpr char kPortalHostname[] = "plane-radar";
constexpr char kPortalHostUrl[] = "plane-radar.local";

/** Per-attempt STA connect wait (ms); retried kWifiConnectAttempts times. */
constexpr unsigned long kWifiConnectAttemptMs = 15000;
constexpr uint8_t kWifiConnectAttempts = 3;
constexpr unsigned long kWifiPortalTimeoutSec = 0;  // 0 = no timeout while configuring
constexpr unsigned long kWifiConnectingFrameMs = 50;
/** Wait after disconnect before reconnecting (avoids portal on brief drops). */
constexpr unsigned long kWifiDownGraceMs = 4000;
/** Minimum interval between background reconnect tries. */
constexpr unsigned long kWifiReconnectIntervalMs = 15000;

// --- Board-specific pins ---------------------------------------------------
// Select the board with a build flag (see platformio.ini):
//   -DBOARD_ESP32_2424S012   ESP32-2424S012 (ESP32-C3 + built-in GC9A01 1.28" round)
//   -DBOARD_XIAO_ESP32C3     Seeed Studio XIAO ESP32C3 + external GC9A01 module
//   (nothing)                ESP32-D1 mini + external GC9A01 module
#if defined(BOARD_ESP32_2424S012)

// BOOT button on the board (GPIO9, active LOW).
constexpr gpio_num_t kBootPin = GPIO_NUM_9;

// Built-in GC9A01. RST is not wired (software reset); backlight on GPIO3.
// (Touch controller CST816S: SDA=4, SCL=5, INT=0, RST=1 — not used by this firmware.)
constexpr gpio_num_t kDisplayPinRst = GPIO_NUM_NC;
constexpr gpio_num_t kDisplayPinCs = GPIO_NUM_10;
constexpr gpio_num_t kDisplayPinDc = GPIO_NUM_2;
constexpr gpio_num_t kDisplayPinMosi = GPIO_NUM_7;  // display SDA
constexpr gpio_num_t kDisplayPinSclk = GPIO_NUM_6;  // display SCL
constexpr gpio_num_t kDisplayPinBacklight = GPIO_NUM_3;
/** Backlight brightness 0-255 (PWM). */
constexpr int kDisplayBrightness = 200;

#elif defined(BOARD_XIAO_ESP32C3)

// Seeed Studio XIAO ESP32C3 + external GC9A01 module.
// BOOT button on the board (GPIO9 = D9, active LOW).
constexpr gpio_num_t kBootPin = GPIO_NUM_9;

// Wiring (XIAO label -> GPIO): SCL=D8(8), SDA=D10(10), CS=D0(2), DC=D1(3), RST=D2(4)
constexpr gpio_num_t kDisplayPinRst = GPIO_NUM_4;
constexpr gpio_num_t kDisplayPinCs = GPIO_NUM_2;
constexpr gpio_num_t kDisplayPinDc = GPIO_NUM_3;
constexpr gpio_num_t kDisplayPinMosi = GPIO_NUM_10;  // display SDA
constexpr gpio_num_t kDisplayPinSclk = GPIO_NUM_8;   // display SCL
/** No backlight pin on the module (always on). */
constexpr gpio_num_t kDisplayPinBacklight = GPIO_NUM_NC;
constexpr int kDisplayBrightness = 255;

#else  // ESP32-D1 mini

// GPIO0 = BOOT button on boards that have one. If your D1 mini has none,
// wire a push button between GPIO0 and GND (or change to e.g. GPIO_NUM_4).
constexpr gpio_num_t kBootPin = GPIO_NUM_0;

constexpr gpio_num_t kDisplayPinRst = GPIO_NUM_27;
constexpr gpio_num_t kDisplayPinCs = GPIO_NUM_22;
constexpr gpio_num_t kDisplayPinDc = GPIO_NUM_21;
constexpr gpio_num_t kDisplayPinMosi = GPIO_NUM_17;  // display SDA
constexpr gpio_num_t kDisplayPinSclk = GPIO_NUM_16;  // display SCL
/** GPIO_NUM_NC = backlight wired straight to 3V3 (no brightness control). */
constexpr gpio_num_t kDisplayPinBacklight = GPIO_NUM_NC;
constexpr int kDisplayBrightness = 255;

#endif

// --- BOOT button behavior (active LOW, internal pull-up) ---
constexpr unsigned long kBootResetHoldMs = 3000UL;
/** Ignore BOOT taps shorter than this (debounce). */
constexpr unsigned long kBootTapMinMs = 40UL;

// --- Display: GC9A01 1.28" round 240×240 (SPI) ---
constexpr int kDisplayWidth = 240;
constexpr int kDisplayHeight = 240;

constexpr uint32_t kDisplaySpiWriteHz = 40000000;
// GC9A01 modules often need invert + BGR for correct black/green output
constexpr bool kDisplayInvert = true;
constexpr bool kDisplayRgbOrder = true;
/**
 * Panel rotation offset 0..7 (4..7 = flipped). If the picture is mirrored or
 * rotated on your board (e.g. N/E/S/W letters mirrored), try 4, 2 or 6.
 */
constexpr int kDisplayOffsetRotation = 0;

// --- Radar sweep line (rotating, with fading trail) ---
constexpr bool kSweepEnabled = true;
/** Time for one full rotation (ms). */
constexpr unsigned long kSweepPeriodMs = 10000;
/** Length of the fading trail behind the leading line (degrees, 10..80). */
constexpr float kSweepTrailDeg = 45.0f;
/** Minimum time between sweep frames (ms); ~30 fps. */
constexpr unsigned long kSweepFrameMs = 33;
/** Sweep color (keep R == B so it looks the same on RGB and BGR panels). */
constexpr uint8_t kSweepColorR = 90;
constexpr uint8_t kSweepColorG = 255;
constexpr uint8_t kSweepColorB = 90;
/** Opacity (0-255) of the leading line and of the trail right behind it. */
constexpr int kSweepHeadAlpha = 230;
constexpr int kSweepTrailAlpha = 110;

// --- Radar center defaults (overridden via WiFi setup portal) ---
constexpr double kDefaultRadarLat = 52.3676;
constexpr double kDefaultRadarLon = 4.9041;

/** Poll adsb.fi (API public limit: 1 req/s). */
constexpr unsigned long kAdsbFetchIntervalMs = 3000;
/** Legacy scale unused — fetch uses radar::fetchRadiusKm() to screen edge. */
constexpr float kAdsbFetchRadiusScale = 1.0f;
/** false = hide aircraft with alt_baro "ground"; true = show them too. */
constexpr bool kAdsbShowGroundAircraft = false;

// --- Flight enrichment (origin/destination and detailed aircraft type) ---
constexpr char kFlightDataApiBase[] = "https://api.adsbdb.com/v0/";
/** One lookup at a time; successful results remain cached for six hours. */
constexpr unsigned long kFlightLookupMinIntervalMs = 750UL;
constexpr unsigned long kFlightLookupTimeoutMs = 5000UL;
constexpr unsigned long kFlightLookupFailureBackoffMs = 30000UL;
constexpr unsigned long kFlightCacheSuccessMs = 6UL * 60UL * 60UL * 1000UL;
constexpr unsigned long kFlightCacheMissMs = 10UL * 60UL * 1000UL;

// --- Weather and local time ---
constexpr char kWeatherApiBase[] = "https://api.open-meteo.com/v1/forecast";
constexpr unsigned long kWeatherFetchIntervalMs = 15UL * 60UL * 1000UL;
constexpr unsigned long kWeatherRequestTimeoutMs = 6000UL;

// --- User-facing defaults ---
constexpr char kOtaUsername[] = "admin";
/** Change this in the web settings before exposing the device to other users. */
constexpr char kDefaultOtaPassword[] = "plane-radar";

// --- UI colors (RGB565) — status screens ---
constexpr uint16_t kColorBlack = 0x0000;
constexpr uint16_t kColorYellow = 0xFFE0;
constexpr uint16_t kTextOnYellow = kColorBlack;
constexpr uint16_t kTextOnBlack = 0xFFFF;

}  // namespace config
