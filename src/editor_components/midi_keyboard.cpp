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

#include "midi_keyboard.h"

void MidiKeyboard::drawBlackNote(int midiNoteNumber, Graphics& g,
                                 int x, int y, int w, int h,
                                 bool isDown, bool isOver,
                                 const Colour& noteFillColour) {
  Colour color(noteFillColour);

  if (isDown)
    color = color.overlaidWith(findColour(keyDownOverlayColourId));
  if (isOver)
    color = color.overlaidWith(findColour(mouseOverKeyOverlayColourId));

  g.setColour(color);
  g.fillRect(x, y, w, h);
}
