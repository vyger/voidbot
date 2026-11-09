#pragma once
// 2-bit button LEDs: the B32 (manual/hardware.md §6.9) and, observed on
// hardware but undocumented, the DB8E's eight button LEDs (§6.12 voidbot note,
// issue #112) render only four brightness levels — off, low, medium, full —
// a fast-serial-link design choice. Display-only: the L register keeps the
// exact value the patch wrote.
//
// Rack-free on purpose: the headless unit suite compiles this directly
// (tests/unit/test_ledlevels.cpp).
#include <algorithm>
#include <cmath>

namespace voidbot {

// Snap a 0..1 brightness to the nearest of {0, 1/3, 2/3, 1}.
// E.g. 0.1 -> 0 (off), 0.2 -> 1/3, 0.8 -> 2/3, 0.9 -> 1.
inline float quantizeLed2bit(float v) {
    return std::round(std::clamp(v, 0.f, 1.f) * 3.f) / 3.f;
}

}  // namespace voidbot
