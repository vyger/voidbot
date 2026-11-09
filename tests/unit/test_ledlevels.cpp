#include "harness.hpp"
#include "LedLevels.hpp"

// 2-bit button LEDs (B32 manual §6.9; DB8E observed on hardware, issue #112):
// a 0..1 brightness snaps to the nearest of {0, 1/3, 2/3, 1}.

using voidbot::quantizeLed2bit;

TEST(ledlevels_reported_db8e_cases) {
    // The user report: 0.05 and 0.1 stay dark, 0.2 already lights "low".
    CHECK_NEAR(quantizeLed2bit(0.05f), 0.0, 1e-6);
    CHECK_NEAR(quantizeLed2bit(0.1f), 0.0, 1e-6);
    CHECK_NEAR(quantizeLed2bit(0.2f), 1.0 / 3.0, 1e-6);
}

TEST(ledlevels_four_levels) {
    CHECK_NEAR(quantizeLed2bit(0.0f), 0.0, 1e-6);
    CHECK_NEAR(quantizeLed2bit(0.4f), 1.0 / 3.0, 1e-6);
    CHECK_NEAR(quantizeLed2bit(0.5f), 2.0 / 3.0, 1e-6);   // 1.5 rounds half away from zero
    CHECK_NEAR(quantizeLed2bit(0.6f), 2.0 / 3.0, 1e-6);
    CHECK_NEAR(quantizeLed2bit(0.8f), 2.0 / 3.0, 1e-6);   // 0.8 is still "medium"
    CHECK_NEAR(quantizeLed2bit(0.9f), 1.0, 1e-6);
    CHECK_NEAR(quantizeLed2bit(1.0f), 1.0, 1e-6);
}

TEST(ledlevels_clamps) {
    CHECK_NEAR(quantizeLed2bit(-0.5f), 0.0, 1e-6);
    CHECK_NEAR(quantizeLed2bit(3.0f), 1.0, 1e-6);
}
