/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VALUE_CHANGE_MANAGER_H
#define VALUE_CHANGE_MANAGER_H

#include "value.h"
#include <string>

class ValueChangeManager {
  public:
    virtual ~ValueChangeManager() { }

    virtual void valueChanged(std::string name, mopo::mopo_float value) = 0;
    virtual void connectModulation(std::string from, std::string to, mopo::Value* scale) = 0;
    virtual void disconnectModulation(std::string to, mopo::Value* scale) = 0;
};

#endif // VALUE_CHANGE_MANAGER_H
