/* Copyright 2013-2015 Matt Tytel
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

#include "retrigger_selector.h"
#include "synth_gui_interface.h"

namespace {
  enum MenuIds {
    kCancel = 0,
    kFree,
    kRetrigger,
    kSyncToPlayhead
  };

} // namespace

RetriggerSelector::RetriggerSelector(String name) : SynthSlider(name) { }

void RetriggerSelector::mouseDown(const MouseEvent& e) {
  if (e.mods.isPopupMenu()) {
    SynthSlider::mouseDown(e);
    return;
  }
  PopupMenu m;
  m.addItem(kFree, TRANS("Free"));
  m.addItem(kRetrigger, TRANS("Retrigger"));
  m.addItem(kSyncToPlayhead, TRANS("Sync to Playhead"));

  int result = m.showAt(this);
  if (result > 0)
    setValue(result - 1);
}

void RetriggerSelector::paint(Graphics& g) {
  static Font retrigger_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
                                                               BinaryData::DroidSansMono_ttfSize));
  g.setColour(Colour(0xffbbbbbb));
  g.fillRect(0, 0, getWidth(), getHeight());

  g.setColour(Colour(0xff222222));
  g.fillPath(arrow_);

  g.setFont(retrigger_font);
  int value = getValue() + 1;
  if (value == kFree)
    g.drawText("F", getLocalBounds(), Justification::centred);
  else if (value == kRetrigger)
    g.drawText("R", getLocalBounds(), Justification::centred);
  else if (value == kSyncToPlayhead)
    g.drawText("S", getLocalBounds(), Justification::centred);
}

void RetriggerSelector::resized() {
  arrow_.clear();
  arrow_.startNewSubPath(getWidth() / 8.0f, getHeight() / 8.0f);
  arrow_.lineTo(3.0f * getWidth() / 8.0f, getHeight() / 8.0f);
  arrow_.lineTo(2.0f * getWidth() / 8.0f, 2.0f * getHeight() / 8.0f);
}
