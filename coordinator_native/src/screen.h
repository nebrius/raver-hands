/*
Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

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

#ifndef SCREEN_HEADER
#define SCREEN_HEADER

#include "codes.h"

namespace Screen {

  void init();
  void loop();

  void updateControl(Codes::Control::Control control);
  void updateBrightness(int brightness);
  void updatePreset(Codes::Preset::Preset preset);
  void updateValue(Codes::Preset::Preset preset, int code, int number);
  void updateClientCount(int count);
  void updateIdleState(Codes::IdleState::IdleState idleState);

}

#endif
