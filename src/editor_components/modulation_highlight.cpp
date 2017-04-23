/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modulation_highlight.h"

ModulationHighlight::ModulationHighlight() {
  setInterceptsMouseClicks(false, false);
}

ModulationHighlight::~ModulationHighlight() {
}

void ModulationHighlight::paint(Graphics& g) {
  g.setColour(Colour(0x44ffffff));
  g.fillEllipse(1, 2, getWidth() - 2, getHeight() - 2);
}

void ModulationHighlight::resized() {
  Component::resized();
  repaint();
}
