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
#ifndef BORDER_BOUNDS_CONSTRAINER_H
#define BORDER_BOUNDS_CONSTRAINER_H

#include "JuceHeader.h"
#include "delete_section.h"

class BorderBoundsConstrainer : public ComponentBoundsConstrainer {
  public:
    BorderBoundsConstrainer() : ComponentBoundsConstrainer() { }

    virtual void checkBounds(Rectangle<int>& bounds, const Rectangle<int>& previous,
                             const Rectangle<int>& limits,
                             bool stretching_top, bool stretching_left,
                             bool stretching_bottom, bool stretching_right) override;

    void setBorder(BorderSize<int> border) { border_ = border; }

  protected:
    BorderSize<int> border_;
};

#endif // BORDER_BOUNDS_CONSTRAINER_H
