#pragma once

#include "controls.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_touch.h>

#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

namespace dusk::ui {

constexpr size_t kTouchLayoutControlCount = 10;

struct TouchLayoutControlInfo {
    std::string_view layoutId;
    const char* elementId = nullptr;
    ControlProps props;
    Control control = Control::COUNT;
    bool hasControl = false;
    // Scales uniformly and always keeps its design size, so only `scale` is user-editable.
    bool uniform = false;
};

std::string_view touch_controls_rml_fragment() noexcept;
std::span<const TouchLayoutControlInfo> touch_layout_controls() noexcept;
const TouchLayoutControlInfo* find_touch_layout_control(std::string_view layoutId) noexcept;
const TouchLayoutControlInfo* find_touch_layout_control(Control control) noexcept;

inline bool is_stick_layout_control(const TouchLayoutControlInfo& info) noexcept {
    return info.layoutId == kStickLayoutId;
}

// Normalizes props loaded from user config for the given control (e.g. keeps the joystick round
// and its scale within range). Controls without special rules are returned unchanged.
ControlProps sanitize_touch_control_props(
    const TouchLayoutControlInfo& info, ControlProps props) noexcept;

// The joystick's size as a percentage of its default (50-200), stored in the saved touch layout so
// the settings slider and the layout editor always agree.
int touch_stick_size_percent() noexcept;
void set_touch_stick_size_percent(int percent);

SDL_FingerID touch_event_id(const Rml::Event& event) noexcept;
Rml::Vector2f touch_event_position(const Rml::Event& event) noexcept;
Rml::Vector2f mouse_event_position(const Rml::Event& event) noexcept;
float touch_dp_scale(Rml::Context* context = nullptr) noexcept;
ControlLayoutSize touch_document_size_dp(Rml::Context* context) noexcept;
ControlAnchor touch_control_dock_anchor(ControlRect visual, ControlLayoutSize docSize) noexcept;

bool control_float_near(float a, float b) noexcept;
bool control_rect_near(ControlRect a, ControlRect b) noexcept;
void apply_control_box_if_changed(
    Rml::Element* element, std::optional<ControlRect>& appliedBox, ControlRect box) noexcept;
void apply_control_transform_if_changed(
    Rml::Element* element, std::optional<float>& appliedTransform, float scale) noexcept;
void apply_control_dock_classes(Rml::Element* element, ControlAnchor anchor) noexcept;

}  // namespace dusk::ui
