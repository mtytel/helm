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

#pragma once
#ifndef TWYTCH_MOD_MATRIX_H
#define TWYTCH_MOD_MATRIX_H

#include "value.h"

#include <vector>

namespace mopo {

  // A modulation matrix source entry.
  class MatrixSourceValue : public Value {
    public:
      MatrixSourceValue(TwytchVoiceHandler* handler) :
          Value(0), handler_(handler), mod_index_(0) { }

      virtual Processor* clone() const { return new MatrixSourceValue(*this); }

      void setSources(const std::vector<std::string> &sources) {
        sources_ = sources;
      }

      void setModulationIndex(int mod_index) {
        mod_index_ = mod_index;
      }

      void set(mopo_float value) {
        Value::set(static_cast<int>(value));
        handler_->setModulationSource(mod_index_, sources_[value_]);
      }

    private:
      TwytchVoiceHandler* handler_;
      std::vector<std::string> sources_;
      int mod_index_;
  };

  // A modulation matrix destination entry.
  class MatrixDestinationValue : public Value {
    public:
      MatrixDestinationValue(TwytchVoiceHandler* handler) :
          Value(0), handler_(handler), mod_index_(0) { }

      virtual Processor* clone() const {
        return new MatrixDestinationValue(*this);
      }

      void setDestinations(const std::vector<std::string> &destinations) {
        destinations_ = destinations;
      }

      void setModulationIndex(int mod_index) {
        mod_index_ = mod_index;
      }

      void set(mopo_float value) {
        Value::set(static_cast<int>(value));
        handler_->setModulationDestination(mod_index_, destinations_[value_]);
      }

    private:
      TwytchVoiceHandler* handler_;
      std::vector<std::string> destinations_;
      int mod_index_;
  };
} // namespace mopo

#endif // TWYTCH_MOD_MATRIX_H
