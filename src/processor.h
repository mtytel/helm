/* Copyright 2013-2017 Matt Tytel
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

#include "common.h"

#include <cstring>
#include <vector>

namespace mopo {

  class Processor;
  class ProcessorRouter;

  // An output port from the Processor.
  struct Output {
    Output(int size = MAX_BUFFER_SIZE) {
      owner = 0;
      buffer = new mopo_float[size];
      buffer_size = size;
      clearBuffer();
      clearTrigger();
    }

    virtual ~Output() {
      delete[] buffer;
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
      VECTORIZE_LOOP
      for (int i = 0; i < buffer_size; ++i)
        buffer[i] = 0.0;
    }

    mopo_float* buffer;
    Processor* owner;

    int buffer_size;
    bool triggered;
    int trigger_offset;
    mopo_float trigger_value;
  };

  // An input port to the Processor. You can plug an Output into one of
  // these inputs.
  struct Input {
    Input() { source = 0; }

    const Output* source;

    inline mopo_float at(int i) const { return source->buffer[i]; }
    inline const mopo_float& operator[](std::size_t i) {
      return source->buffer[i];
    }
  };

  namespace cr {
    struct Output : public ::mopo::Output {
      Output() : ::mopo::Output(1) { }
    };
  } // namespace cr

  class Processor {
    public:
      Processor(int num_inputs, int num_outputs, bool control_rate = false);

      virtual ~Processor() { }

      // Currently need to override this boiler plate clone.
      // TODO(mtytel): Should probably make a macro for this.
      virtual Processor* clone() const = 0;

      // Cleanup shared memory.
      virtual void destroy();

      // Subclasses override this for main processing code.
      virtual void process() = 0;

      // Subclasses should override this if they need to adjust for change in
      // sample rate.
      virtual void setSampleRate(int sample_rate) {
        sample_rate_ = sample_rate;
      }

      virtual void setBufferSize(int buffer_size) {
        if (control_rate_)
          buffer_size_ = 1;
        else
          buffer_size_ = buffer_size;
        samples_to_process_ = buffer_size;
      }

      virtual void setControlRate(bool control_rate = true) {
        control_rate_ = control_rate;
        if (control_rate)
          buffer_size_ = 1;
      }

      inline bool enabled() const {
        return *enabled_;
      }

      inline void enable(bool enable) {
        *enabled_ = enable;
      }

      int getSampleRate() const {
        return sample_rate_;
      }

      int getBufferSize() const {
        return buffer_size_;
      }

      int getSamplesToProcess() const {
        return samples_to_process_;
      }

      bool isControlRate() const {
        return control_rate_;
      }

      bool inputMatchesBufferSize(int input = 0);

      virtual bool isPolyphonic() const;

      // Attaches an output to an input in this processor.
      void plug(const Output* source);
      void plug(const Output* source, unsigned int input_index);
      void plug(const Processor* source);
      void plug(const Processor* source, unsigned int input_index);

      // Attaches an output to the first available input in this processor.
      void plugNext(const Output* source);
      void plugNext(const Processor* source);

      // Count how many inputs are connected to processors
      int connectedInputs();

      // Remove a connection between two processors.
      virtual void unplugIndex(unsigned int input_index);
      virtual void unplug(const Output* source);
      virtual void unplug(const Processor* source);

      // Sets the ProcessorRouter that will own this Processor.
      inline void router(ProcessorRouter* router) { router_ = router; }

      // Returns the ProcessorRouter that owns this Processor.
      inline ProcessorRouter* router() const { return router_; }

      // Returns the ProcessorRouter that owns this Processor.
      ProcessorRouter* getTopLevelRouter() const;

      virtual void registerInput(Input* input, int index);
      virtual Output* registerOutput(Output* output, int index);
      virtual void registerInput(Input* input);
      virtual Output* registerOutput(Output* output);

      inline int numInputs() const { return inputs_->size(); }
      inline int numOutputs() const { return outputs_->size(); }

      // Input sample access.
      inline mopo_float getInputSample(int input, int sample) {
        return inputs_->operator[](input)->at(sample);
      }

      // Output sample writin.
      inline void writeOutputSample(int output, int sample, mopo_float value) {
        outputs_->operator[](output)->buffer[sample] = value;
      }

      // Returns the Input port corresponding to the passed in index.
      inline Input* input(unsigned int index = 0) const {
        MOPO_ASSERT(index < inputs_->size());

        return inputs_->operator[](index);
      }

      // Returns the Output port corresponding to the passed in index.
      inline Output* output(unsigned int index = 0) const {
        MOPO_ASSERT(index < outputs_->size());

        return outputs_->operator[](index);
      }

    protected:
      Output* addOutput();
      Input* addInput();
    
      int sample_rate_;
      int buffer_size_;
      int samples_to_process_;
      bool control_rate_;
      bool* enabled_;

      std::vector<Input*> owned_inputs_;
      std::vector<Output*> owned_outputs_;

      std::vector<Input*>* inputs_;
      std::vector<Output*>* outputs_;

      ProcessorRouter* router_;

      static const Output null_source_;
  };
} // namespace mopo

#endif // PROCESSOR_H
