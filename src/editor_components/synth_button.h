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

#ifndef SYNTH_BUTTON_H
#define SYNTH_BUTTON_H

#include "JuceHeader.h"
#include "helm_common.h"

class FullInterface;

class SynthButton : public ToggleButton {
  public:
    SynthButton(String name);

    void setStringLookup(const std::string* lookup) {
      string_lookup_ = lookup;
    }
    const std::string* getStringLookup() const { return string_lookup_; }
    String getTextFromValue(bool value);

    void setActive(bool active = true);
    bool isActive() const { return active_; }

  private:
    void buttonStateChanged() override;
    void notifyTooltip();

    bool active_;
    const std::string* string_lookup_;
    FullInterface* parent_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthButton)
};

#endif // SYNTH_BUTTON_H
