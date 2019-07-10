/*
Copyright (c) 2018 Bryan Hughes <bryan@nebri.us>

This file is part of Raver Lights.

Raver Lights is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Raver Lights is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Raver Lights.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <rvl-esp.h>
#include "./ui/ui_state.h"
#include "./state.h"
#include "./event.h"
#include "./codes.h"
#include "./presets/preset_control_set.h"
#include "./presets/rainbow.h"
#include "./presets/pulse.h"
#include "./presets/wave.h"
#include "./presets/solid.h"
#include "./presets/color_cycle.h"

namespace UIState {

uint8_t currentControl = 0;
uint8_t preset = DEFAULT_PRESET;

PresetControlSet* presets[] = {
  &Rainbow::rainbow,
  &Pulse::pulse,
  &Wave::wave,
  &ColorCycle::colorCycle,
  &Solid::solid
};

void updateBrightnessValue(uint8_t newValue) {
  State::setBrightness(newValue);
  State::getLogger()->info("Changing brightness to %d", newValue);
  Event::emit(Codes::EventType::AnimationChange);
}
Control::RangeControl brightnessControl(
  "BRT",
  0,
  MAX_BRIGHTNESS,
  DEFAULT_BRIGHTNESS,
  updateBrightnessValue);

void updateChannelValue(uint8_t selectedValueIndex) {
  if (RVLESPGetChannel() != selectedValueIndex) {
    RVLESPSetChannel(selectedValueIndex);
  }
}
Control::ListControl channelControl(
  "CHNL",
  { "0", "1", "2", "3", "4", "5", "6", "7" },
  DEFAULT_CHANNEL,
  updateChannelValue);

void updateModeValue(uint8_t selectedValueIndex) {
  if (RVLESPGetMode() != static_cast<RVLDeviceMode>(selectedValueIndex)) {
    switch (static_cast<RVLDeviceMode>(selectedValueIndex)) {
      case RVLDeviceMode::Controller:
        RVLESPSetMode(RVLDeviceMode::Controller);
        presets[preset]->updateWave();
        break;
      case RVLDeviceMode::Receiver:
        RVLESPSetMode(RVLDeviceMode::Receiver);
        break;
    }
  }
}
Control::ListControl modeControl(
  "MODE",
  { "Controller", "Receiver" },
  1,
  updateModeValue);

void updatePresetValue(uint8_t selectedValueIndex) {
  if (UIState::preset != selectedValueIndex) {
    UIState::preset = selectedValueIndex;
    presets[preset]->updateWave();
  }
}
Control::ListControl presetControl(
  "PRST",
  { "Rainbow", "Pulse", "Wave", "Color Cycle", "Solid" },
  2,
  updatePresetValue);

std::vector<Control::Control*> controls = {
  &brightnessControl,
  &channelControl,
  &modeControl
};

void update() {
  while (controls.size() > 3) {
    controls.pop_back();
  }
  if (RVLESPGetMode() == RVLDeviceMode::Controller) {
    controls.push_back(&presetControl);
    for (auto& control : presets[preset]->controls) {
      controls.push_back(control);
    }
  }
  Event::emit(Codes::EventType::UIStateChange);
}

void init() {
  Event::on(Codes::EventType::ConnectedStateChange, update);
  Event::on(Codes::EventType::AnimationChange, update);
  Event::on(Codes::EventType::ModeChange, update);
  update();
  controls.reserve(10);
  presets[preset]->updateWave();
}

void nextControl() {
  if (currentControl < controls.size() - 1) {
    currentControl++;
    State::getLogger()->debug("Setting control to %d", currentControl);
    Event::emit(Codes::EventType::UIStateChange);
  }
}

void previousControl() {
  if (currentControl > 0) {
    currentControl--;
    State::getLogger()->debug("Setting control to %d", currentControl);
    Event::emit(Codes::EventType::UIStateChange);
  }
}

void controlIncrease() {
  controls[currentControl]->increaseValue();
  Event::emit(Codes::EventType::UIStateChange);
}

void controlDecrease() {
  controls[currentControl]->decreaseValue();
  Event::emit(Codes::EventType::UIStateChange);
}

bool isCurrentControlRange() {
  return controls[currentControl]->type == Control::ControlType::Range;
}

}  // namespace UIState
