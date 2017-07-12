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

#include "voice_handler.h"

#include "utils.h"

namespace mopo {

  Voice::Voice(Processor* processor) : event_sample_(-1),
      aftertouch_sample_(-1), aftertouch_(0.0), processor_(processor) {
    state_.event = kVoiceOff;
    state_.note = 0;
    state_.velocity = 0;
    state_.last_note = 0;
    state_.note_pressed = 0;
    state_.channel = 0;
    key_state_ = kReleased;
  }

  Voice::~Voice() {
    delete processor_;
  }

  VoiceHandler::VoiceHandler(size_t polyphony) :
      ProcessorRouter(kNumInputs, 0), polyphony_(0), sustain_(false),
      legato_(false), voice_killer_(0), last_played_note_(-1.0) {
    pressed_notes_.reserve(MIDI_SIZE);
    all_voices_.reserve(MAX_POLYPHONY);
    free_voices_.reserve(MAX_POLYPHONY);
    active_voices_.reserve(MAX_POLYPHONY);

    setPolyphony(polyphony);
    voice_router_.router(this);
    global_router_.router(this);
}

  VoiceHandler::~VoiceHandler() {
    voice_router_.destroy();
    global_router_.destroy();

    for (Voice* voice : all_voices_)
      delete voice;

    for (auto& output : accumulated_outputs_)
      delete output.second;

    for (auto& output : last_voice_outputs_)
      delete output.second;
  }

  void VoiceHandler::prepareVoiceTriggers(Voice* voice) {
    note_.clearTrigger();
    last_note_.clearTrigger();
    note_pressed_.clearTrigger();
    channel_.clearTrigger();
    velocity_.clearTrigger();
    voice_event_.clearTrigger();
    aftertouch_.clearTrigger();
    channel_.buffer[0] = voice->state().channel;

    if (voice->hasNewEvent()) {
      voice_event_.trigger(voice->state().event, voice->event_sample());
      if (voice->state().event == kVoiceOn) {
        note_.trigger(voice->state().note, 0);
        last_note_.trigger(voice->state().last_note, 0);
        velocity_.trigger(voice->state().velocity, 0);
        note_pressed_.trigger(voice->state().note_pressed, 0);
        channel_.trigger(voice->state().channel, 0);
      }
    }

    if (voice->hasNewAftertouch())
      aftertouch_.trigger(voice->aftertouch(), voice->aftertouch_sample());

    voice->clearEvents();
  }

  void VoiceHandler::processVoice(Voice* voice) {
    voice->processor()->process();
  }

  void VoiceHandler::clearAccumulatedOutputs() {
    for (auto& output : accumulated_outputs_)
      utils::zeroBuffer(output.second->buffer, MAX_BUFFER_SIZE);
  }

  void VoiceHandler::clearNonaccumulatedOutputs() {
    for (auto& output : last_voice_outputs_)
      utils::zeroBuffer(output.second->buffer, MAX_BUFFER_SIZE);
  }

  void VoiceHandler::accumulateOutputs() {
    for (auto& output : accumulated_outputs_) {
      int buffer_size = output.first->owner->getBufferSize();
      mopo_float* dest = output.second->buffer;
      const mopo_float* source = output.first->buffer;

      VECTORIZE_LOOP
      for (int i = 0; i < buffer_size; ++i)
        dest[i] += source[i];
    }
  }

  void VoiceHandler::writeNonaccumulatedOutputs() {
    for (auto& output : last_voice_outputs_) {
      int buffer_size = output.first->owner->getBufferSize();
      mopo_float* dest = output.second->buffer;
      const mopo_float* source = output.first->buffer;

      utils::copyBuffer(dest, source, buffer_size);
    }
  }

  bool VoiceHandler::shouldAccumulate(Output* output) {
    return !output->owner->isControlRate();
  }

  void VoiceHandler::process() {
    global_router_.process();

    int num_voices = active_voices_.size();
    if (num_voices == 0) {
      if (last_num_voices_) {
        clearNonaccumulatedOutputs();
        clearAccumulatedOutputs();
      }

      last_num_voices_ = num_voices;
      return;
    }

    int polyphony = static_cast<int>(input(kPolyphony)->at(0));
    setPolyphony(utils::iclamp(polyphony, 1, polyphony));
    clearAccumulatedOutputs();

    auto iter = active_voices_.begin();
    while (iter != active_voices_.end()) {
      Voice* voice = *iter;
      prepareVoiceTriggers(voice);
      processVoice(voice);
      accumulateOutputs();

      // Remove voice if the right processor has a full silent buffer.
      if (voice_killer_ && voice->state().event != kVoiceOn &&
          utils::isSilent(voice_killer_->buffer, buffer_size_)) {
        free_voices_.push_back(voice);
        iter = active_voices_.erase(iter);
      }
      else
        iter++;
    }

    if (active_voices_.size())
      writeNonaccumulatedOutputs();

    last_num_voices_ = num_voices;
  }

  void VoiceHandler::setSampleRate(int sample_rate) {
    ProcessorRouter::setSampleRate(sample_rate);
    voice_router_.setSampleRate(sample_rate);
    global_router_.setSampleRate(sample_rate);
    for (int i = 0; i < all_voices_.size(); ++i)
      all_voices_[i]->processor()->setSampleRate(sample_rate);
  }

  void VoiceHandler::setBufferSize(int buffer_size) {
    ProcessorRouter::setBufferSize(buffer_size);
    voice_router_.setBufferSize(buffer_size);
    global_router_.setBufferSize(buffer_size);
    for (int i = 0; i < all_voices_.size(); ++i)
      all_voices_[i]->processor()->setBufferSize(buffer_size);
  }

  int VoiceHandler::getNumActiveVoices() {
    return active_voices_.size();
  }

  bool VoiceHandler::isNotePlaying(mopo_float note) {
    for (Voice* voice : active_voices_) {
      if (voice->state().note == note)
        return true;
    }
    return false;
  }

  void VoiceHandler::sustainOn() {
    sustain_ = true;
  }

  void VoiceHandler::sustainOff(int sample) {
    sustain_ = false;
    for (Voice* voice : active_voices_) {
      if (voice->key_state() == Voice::kSustained)
        voice->deactivate(sample);
    }
  }

  void VoiceHandler::allNotesOff(int sample) {
    pressed_notes_.clear();

    for (Voice* voice : active_voices_)
      voice->deactivate(sample);
  }

  Voice* VoiceHandler::grabVoice() {
    Voice* voice = 0;

    // First check free voices.
    if (free_voices_.size() &&
       (!legato_ || pressed_notes_.size() < polyphony_ || active_voices_.size() < polyphony_)) {
      voice = free_voices_.front();
      free_voices_.pop_front();
      return voice;
    }

    // Next check released voices.
    for (auto iter = active_voices_.begin(); iter != active_voices_.end(); ++iter) {
      voice = *iter;
      if (voice->key_state() == Voice::kReleased) {
        active_voices_.erase(iter);
        return voice;
      }
    }

    // Then check sustained voices.
    for (auto iter = active_voices_.begin(); iter != active_voices_.end(); ++iter) {
      voice = *iter;
      if (voice->key_state() == Voice::kSustained) {
        active_voices_.erase(iter);
        return voice;
      }
    }

    // If all are active just grab the oldest voice.
    MOPO_ASSERT(active_voices_.size());
    voice = active_voices_.front();
    active_voices_.pop_front();
    return voice;
  }

  Voice* VoiceHandler::getVoiceToKill() {
    int excess_voices = active_voices_.size() - polyphony_;
    Voice* oldest_released = 0;
    Voice* oldest_sustained = 0;
    Voice* oldest_held = 0;

    for (auto iter = active_voices_.begin(); iter != active_voices_.end(); ++iter) {
      Voice* voice = *iter;
      if (voice->state().event == kVoiceKill)
        excess_voices--;
      else if (oldest_released == 0 && voice->key_state() == Voice::kReleased)
        oldest_released = voice;
      else if (oldest_sustained == 0 && voice->key_state() == Voice::kSustained)
        oldest_sustained = voice;
      else if (oldest_held == 0)
        oldest_held = voice;
    }

    // Return null if we've killed enough voices.
    if (excess_voices <= 0)
      return 0;

    // If there were any released notes kill the oldest.
    if (oldest_released)
      return oldest_released;

    // Then if there were any sustained notes kill the oldest.
    if (oldest_sustained)
      return oldest_sustained;

    // If all are active just grab the oldest held voice.
    if (oldest_held)
      return oldest_held;

    return 0;
  }

  void VoiceHandler::noteOn(mopo_float note, mopo_float velocity, int sample, int channel) {
    MOPO_ASSERT(sample >= 0 && sample < buffer_size_);
    MOPO_ASSERT(channel >= 0 && channel < NUM_MIDI_CHANNELS);

    Voice* voice = grabVoice();
    pressed_notes_.remove(note);
    pressed_notes_.push_front(note);

    if (last_played_note_ < 0)
      last_played_note_ = note;
    voice->activate(note, velocity, last_played_note_, pressed_notes_.size(), sample, channel);
    active_voices_.push_back(voice);
    last_played_note_ = note;
  }

  VoiceEvent VoiceHandler::noteOff(mopo_float note, int sample) {
    pressed_notes_.removeAll(note);

    VoiceEvent voice_event = kVoiceOff;

    for (Voice* voice : active_voices_) {
      if (voice->state().note == note) {
        if (sustain_)
          voice->sustain();
        else {
          if (polyphony_ <= pressed_notes_.size() && voice->state().event != kVoiceKill) {
            voice->kill();

            Voice* new_voice = grabVoice();
            active_voices_.push_back(new_voice);
            mopo_float old_note = pressed_notes_.back();
            pressed_notes_.pop_back();
            pressed_notes_.push_front(old_note);
            new_voice->activate(old_note, voice->state().velocity, last_played_note_,
                                pressed_notes_.size() + 1, sample);
            last_played_note_ = old_note;

            voice_event = kVoiceReset;
          }
          else
            voice->deactivate(sample);
        }
      }
    }
    return voice_event;
  }

  void VoiceHandler::setAftertouch(mopo_float note, mopo_float aftertouch, int sample) {
    for (Voice* voice : active_voices_) {
      if (voice->state().note == note)
        voice->setAftertouch(aftertouch, sample);
    }
  }

  void VoiceHandler::setPolyphony(size_t polyphony) {
    while (all_voices_.size() < polyphony) {
      Voice* new_voice = createVoice();
      all_voices_.push_back(new_voice);
      active_voices_.push_back(new_voice);
    }

    int num_voices_to_kill = active_voices_.size() - polyphony;
    for (int i = 0; i < num_voices_to_kill; ++i) {
      Voice* sacrifice = getVoiceToKill();
      if (sacrifice)
        sacrifice->kill();
    }

    polyphony_ = polyphony;
  }

  mopo_float VoiceHandler::getLastActiveNote() const {
    if (active_voices_.size())
      return active_voices_.back()->state().note;
    return 0.0;
  }

  void VoiceHandler::addProcessor(Processor* processor) {
    processor->setBufferSize(getBufferSize());
    processor->setSampleRate(getSampleRate());
    voice_router_.addProcessor(processor);
  }

  void VoiceHandler::removeProcessor(const Processor* processor) {
    voice_router_.removeProcessor(processor);
  }

  void VoiceHandler::addGlobalProcessor(Processor* processor) {
    global_router_.addProcessor(processor);
  }

  void VoiceHandler::removeGlobalProcessor(Processor* processor) {
    global_router_.removeProcessor(processor);
  }

  Output* VoiceHandler::registerOutput(Output* output) {
    Output* new_output = new Output();
    new_output->owner = this;
    ProcessorRouter::registerOutput(new_output);
    if (shouldAccumulate(output))
      accumulated_outputs_[output] = new_output;
    else
      last_voice_outputs_[output] = new_output;
    return new_output;
  }

  Output* VoiceHandler::registerOutput(Output* output, int index) {
    MOPO_ASSERT(false);
    return output;
  }

  bool VoiceHandler::isPolyphonic(const Processor* processor) const {
    return processor == &voice_router_;
  }

  Voice* VoiceHandler::createVoice() {
    return new Voice(voice_router_.clone());
  }
} // namespace mopo
