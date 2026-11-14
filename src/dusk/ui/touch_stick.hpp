#pragma once

#include <cmath>

// Pure geometry for the on-screen touch joystick. This header deliberately has no engine, RmlUi or
// SDL dependencies so the maths can be reasoned about (and unit tested) in isolation.
//
// All "Dp" values are density-independent pixels at the stick's design size. The on-screen size is
// the design size multiplied by the user's layout scale, which is applied to the element as a CSS
// transform, so the stick, ring and knob always scale together.
namespace dusk::ui::stick {

inline constexpr float kBaseSizeDp = 124.f;
inline constexpr float kBaseRadiusDp = kBaseSizeDp * 0.5f;
inline constexpr float kKnobRadiusDp = 24.f;

// The layout scale the user can choose for the stick.
inline constexpr float kMinScale = 0.5f;
inline constexpr float kMaxScale = 2.0f;

// A locked stick also accepts touches slightly outside its ring (as a multiple of its radius), so
// it is easy to grab with a thumb without looking at it.
inline constexpr float kLockedGrabRatio = 1.35f;

struct Vec2 {
    float x = 0.f;
    float y = 0.f;
};

// Clamps a layout scale into the supported range. Non-finite values fall back to the default.
constexpr float clamp_scale(float scale) noexcept {
    if (scale != scale) {
        return 1.f;
    }
    return scale < kMinScale ? kMinScale : (scale > kMaxScale ? kMaxScale : scale);
}

// Outer radius of the stick in physical pixels.
constexpr float radius_px(float scale, float dpScale) noexcept {
    return kBaseRadiusDp * clamp_scale(scale) * dpScale;
}

// Offset of `current` from `origin`, limited to a circle of `radius`.
inline Vec2 clamped_delta(Vec2 origin, Vec2 current, float radius) noexcept {
    if (!(radius > 0.f)) {
        return {};
    }
    Vec2 delta{current.x - origin.x, current.y - origin.y};
    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (length > radius) {
        const float ratio = radius / length;
        delta.x *= ratio;
        delta.y *= ratio;
    }
    return delta;
}

// Whether a touch lands close enough to a locked stick to grab it.
constexpr bool within_grab_area(Vec2 point, Vec2 center, float radius) noexcept {
    const float dx = point.x - center.x;
    const float dy = point.y - center.y;
    const float grab = radius * kLockedGrabRatio;
    return dx * dx + dy * dy <= grab * grab;
}

// Normalised pad axes in [-1, 1] for a clamped delta. Screen Y points down, pad Y points up.
constexpr Vec2 axes(Vec2 clampedDelta, float radius) noexcept {
    if (!(radius > 0.f)) {
        return {};
    }
    return {clampedDelta.x / radius, -clampedDelta.y / radius};
}

// Knob displacement in the element's own (unscaled) dp space, given a displacement in pixels.
constexpr Vec2 knob_offset_dp(Vec2 deltaPx, float dpScale, float scale) noexcept {
    const float divisor = dpScale * clamp_scale(scale);
    if (!(divisor > 0.f)) {
        return {};
    }
    return {deltaPx.x / divisor, deltaPx.y / divisor};
}

}  // namespace dusk::ui::stick
