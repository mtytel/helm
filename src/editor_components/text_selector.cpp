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

#include "text_selector.h"

#include "default_look_and_feel.h"
#include "fonts.h"

namespace {
  static void textSelectedCallback(int result, TextSelector* text_selector) {
    if (text_selector != nullptr && result != 0)
      text_selector->setValue(result - 1);
  }
} // namespace

TextSelector::TextSelector(String name) : SynthSlider(name), long_lookup_(nullptr) { }

void TextSelector::mouseDown(const juce::MouseEvent &e) {
  if (e.mods.isPopupMenu()) {
    SynthSlider::mouseDown(e);
    return;
  }

  const std::string* lookup = string_lookup_;
  if (long_lookup_)
    lookup = long_lookup_;

  PopupMenu m;
  m.setLookAndFeel(DefaultLookAndFeel::instance());

  for (int i = 0; i <= getMaximum(); ++i)
    m.addItem(i + 1, lookup[i]);


  m.showMenuAsync(PopupMenu::Options().withTargetComponent(this),
                  ModalCallbackFunction::forComponent(textSelectedCallback, this));
}

void TextSelector::mouseUp(const MouseEvent& e) {
  if (e.mods.isPopupMenu()) {
    SynthSlider::mouseDown(e);
    return;
  }
}
