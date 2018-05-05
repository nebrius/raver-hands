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

#include <list>
#include <Arduino.h>
#include "./codes.h"
#include "./screen.h"
#include "./messaging/messaging_server.h"
#include "./events.h"

namespace Events {

struct ListenerEntry {
 public:
  int eventType;
  EventListenerInterface* listener;
};
std::list<ListenerEntry> listeners;

void on(int eventType, EventListenerInterface* listener) {
  ListenerEntry entry = { eventType, listener };
  listeners.push_back(entry);
}

void emit(int eventType) {
  for (auto& listener : listeners) {
    if (listener.eventType == eventType) {
      listener.listener->onEvent();
    }
  }
}

}  // namespace Events
