/* Copyright 2013 Little IO
 *
 * laf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * laf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with laf.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "laf.h"

#include <string.h>
#include <vector>

namespace laf {

  class ProcessorRouter;

  class Processor {
    public:
      // An output port from the Processor.
      struct Output {
        Output() {
          owner = 0;
          memset(buffer, 0, BUFFER_SIZE * sizeof(laf_sample));
          clearTrigger();
        }

        void trigger(laf_sample value, int offset = 0) {
          triggered = true;
          trigger_offset = offset;
          trigger_value = value;
        }

        void clearTrigger() {
          triggered = false;
          trigger_offset = 0;
          trigger_value = 0.0;
        }

        const Processor* owner;
        laf_sample buffer[BUFFER_SIZE];

        bool triggered;
        int trigger_offset;
        laf_sample trigger_value;
      };

      // An input port to the Processor. You can plug an Output into on of
      // these inputs.
      struct Input {
        Input() { source = 0; }

        const Output* source;

        laf_sample at(int i) const { return source->buffer[i]; }
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

      // Makes a connection between two processors.
      void plug(const Output* source, unsigned int input_index = 0);
      void plug(const Processor* source, unsigned int input_index = 0);

      // Sets the ProcessorRouter that will own this Processor.
      void router(ProcessorRouter* router) { router_ = router; }

      // Returns the ProcessorRouter that owns this Processor.
      const ProcessorRouter* router() const { return router_; }

      virtual void registerInput(Input* input);
      virtual void registerOutput(Output* output);

      virtual int numInputs() const { return inputs_.size(); }
      virtual int numOutputs() const { return outputs_.size(); }

      // Returns the Input port corresponding to the passed in index.
      Input* input(unsigned int index = 0) const;

      // Returns the Output port corresponding to the passed in index.
      Output* output(unsigned int index = 0) const;

    protected:
      int sample_rate_;

      std::vector<Input*> inputs_;
      std::vector<Output*> outputs_;

      ProcessorRouter* router_;

      static const Output null_source_;
  };
} // namespace laf

#endif // PROCESSOR_H
