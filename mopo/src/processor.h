/* Copyright 2013-2015 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "mopo.h"

#include <cstring>
#include <vector>

namespace mopo {

  class ProcessorRouter;

  class Processor {
    public:
      virtual ~Processor() { }

      // An output port from the Processor.
      struct Output {
        Output() {
          owner = nullptr;
          clearBuffer();
          clearTrigger();
        }

        void trigger(mopo_float value, int offset = 0) {
          triggered = true;
          trigger_offset = offset;
          trigger_value = value;
        }

        void clearTrigger() {
          triggered = false;
          trigger_offset = 0;
          trigger_value = 0.0;
        }

        void clearBuffer() {
          memset(buffer, 0, MAX_BUFFER_SIZE * sizeof(mopo_float));
        }

        const Processor* owner;
        mopo_float buffer[MAX_BUFFER_SIZE];

        bool triggered;
        int trigger_offset;
        mopo_float trigger_value;
      };

      // An input port to the Processor. You can plug an Output into on of
      // these inputs.
      struct Input {
        Input() { source = nullptr; }

        const Output* source;

        inline mopo_float at(int i) const { return source->buffer[i]; }
        inline const mopo_float& operator[](std::size_t i) {
          return source->buffer[i];
        }
      };

      Processor(int num_inputs, int num_outputs);

      // Currently need to override this boiler plate clone.
      // TODO(mtytel): Should probably make a macro for this.
      virtual Processor* clone() const = 0;

      // Subclasses override this for main processing code.
      virtual void process() = 0;

      // Subclasses should override this if they need to adjust for change in
      // sample rate.
      virtual void setSampleRate(int sample_rate) {
        sample_rate_ = sample_rate;
      }

      virtual void setBufferSize(int buffer_size) {
        buffer_size_ = buffer_size;
      }

      // Attaches an output to an input in this processor.
      void plug(const Output* source);
      void plug(const Output* source, unsigned int input_index);
      void plug(const Processor* source);
      void plug(const Processor* source, unsigned int input_index);

      // Attaches an output to the first available input in this processor.
      void plugNext(const Output* source);
      void plugNext(const Processor* source);

      // Remove a connection between two processors.
      virtual void unplugIndex(unsigned int input_index);
      virtual void unplug(const Output* source);
      virtual void unplug(const Processor* source);

      // Sets the ProcessorRouter that will own this Processor.
      void router(ProcessorRouter* router) { router_ = router; }

      // Returns the ProcessorRouter that owns this Processor.
      const ProcessorRouter* router() const { return router_; }

      virtual void registerInput(Input* input, int index);
      virtual void registerOutput(Output* output, int index);
      virtual void registerInput(Input* input);
      virtual void registerOutput(Output* output);

      virtual int numInputs() const { return inputs_->size(); }
      virtual int numOutputs() const { return outputs_->size(); }

      // Input sample access.
      inline mopo_float getInputSample(int input, int sample) {
        return inputs_->operator[](input)->at(sample);
      }

      // Output sample writin.
      inline void writeOutputSample(int output, int sample, mopo_float value) {
        outputs_->operator[](output)->buffer[sample] = value;
      }

      // Returns the Input port corresponding to the passed in index.
      Input* input(unsigned int index = 0) const;

      // Returns the Output port corresponding to the passed in index.
      Output* output(unsigned int index = 0) const;

    protected:
      int sample_rate_;
      int buffer_size_;

      std::vector<Input*>* inputs_;
      std::vector<Output*>* outputs_;

      ProcessorRouter* router_;

      static const Output null_source_;
  };
} // namespace mopo

#endif // PROCESSOR_H
