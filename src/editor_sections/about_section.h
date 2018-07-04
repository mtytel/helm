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

#pragma once
#ifndef ABOUT_SECTION_H
#define ABOUT_SECTION_H

#include "JuceHeader.h"
#include "overlay.h"

class AboutSection : public Overlay, public Button::Listener {
  public:
    AboutSection(String name);
    ~AboutSection() { }
    void paint(Graphics& g) override;
    void resized() override;

    Rectangle<int> getInfoRect();

    void mouseUp(const MouseEvent& e) override;
    void setVisible(bool should_be_visible) override;
    void buttonClicked(Button* clicked_button) override;

  private:
    void setGuiSize(float multiplier);

    ScopedPointer<HyperlinkButton> developer_link_;
    ScopedPointer<HyperlinkButton> free_software_link_;
    ScopedPointer<AudioDeviceSelectorComponent> device_selector_;
    ScopedPointer<Button> check_for_updates_;
    ScopedPointer<Button> animate_;

    ScopedPointer<Button> size_button_small_;
    ScopedPointer<Button> size_button_normal_;
    ScopedPointer<Button> size_button_large_;
    ScopedPointer<Button> size_button_extra_large_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AboutSection)
};

#endif // ABOUT_SECTION_H
