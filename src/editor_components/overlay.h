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

#ifndef OVERLAY_H
#define OVERLAY_H

#include "JuceHeader.h"
#include <set>

class Overlay : public Component {
  public:
    class Listener {
      public:
        Listener() { }
        virtual ~Listener() { }

        virtual void overlayShown(Overlay* component) = 0;
        virtual void overlayHidden(Overlay* component) = 0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Listener)
    };

    Overlay(String name) : Component(name), size_ratio_(1.0f) { }
    virtual ~Overlay() { }

    void setVisible(bool should_be_visible) override {
      for (Listener* listener : listeners_) {
        if (should_be_visible)
          listener->overlayShown(this);
        else
          listener->overlayHidden(this);
      }
      Component::setVisible(should_be_visible);
    }

    void addListener(Listener* listener) { listeners_.insert(listener); }
    void removeListener(Listener* listener) { listeners_.erase(listener); }
    void setSizeRatio(float ratio) { size_ratio_ = ratio;}

  protected:
    float size_ratio_;
    std::set<Listener*> listeners_;
};

#endif // OVERLAY_LISTENER_H
