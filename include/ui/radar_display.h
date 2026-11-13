#pragma once

#include <LovyanGFX.hpp>

namespace ui {

/** Draw the static sonar/radar grid (black disc, green overlay, labels). */
void radarDisplayDraw();

/** Redraw aircraft only (blits cached grid; no full-screen clear). */
void radarDisplayRefreshAircraft();

/**
 * drawWideLine that is safe on the band canvas used by the radar renderer.
 * LovyanGFX's wide-line code overrides and then clears the clip rectangle, which
 * would let it write outside the band buffer; on the band canvas the line is
 * drawn into the real-size band sprite (coordinates shifted) instead.
 */
void drawWideLine(lgfx::LGFXBase& gfx, int x0, int y0, int x1, int y1,
                  float half_width, uint16_t color);

/** Create the sweep task. Call once after displayInit(). */
void radarSweepStart();

/** Enable/disable the sweep (disabled while status screens own the panel). */
void radarSweepSetActive(bool active);

}  // namespace ui
