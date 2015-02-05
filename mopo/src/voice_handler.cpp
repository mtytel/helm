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

#include "voice_handler.h"

#include "utils.h"

namespace mopo {

  Voice::Voice(Processor* processor) : event_sample_(-1),
      aftertouch_sample_(-1), aftertouch_(0.0), processor_(processor) { }

  VoiceHandler::VoiceHandler(size_t polyphony) :
      Processor(kNumInputs, 1), polyphony_(0), sustain_(false),
      voice_output_(0), voice_killer_(0) {
    setPolyphony(polyphony);
  }

  void VoiceHandler::prepareVoiceTriggers(Voice* voice) {
    note_.clearTrigger();
    velocity_.clearTrigger();
    voice_event_.clearTrigger();
    aftertouch_.clearTrigger();

    if (voice->hasNewEvent()) {
      voice_event_.trigger(voice->state()->event, voice->event_sample());
      if (voice->state()->event == kVoiceOn) {
        note_.trigger(voice->state()->note, 0);
        velocity_.trigger(voice->state()->velocity, 0);
      }
    }

    if (voice->hasNewAftertouch())
      aftertouch_.trigger(voice->aftertouch(), voice->aftertouch_sample());

    voice->clearEvents();
  }

  void VoiceHandler::processVoice(Voice* voice) {
    voice->processor()->process();
    for (int i = 0; i < buffer_size_; ++i)
      output(0)->buffer[i] += voice_output_->buffer[i];
  }

  void VoiceHandler::process() {
    global_router_.process();

    size_t polyphony = static_cast<size_t>(input(kPolyphony)->at(0));
    setPolyphony(CLAMP(polyphony, 1, polyphony));
    memset(output(0)->buffer, 0, buffer_size_ * sizeof(mopo_float));

    std::list<Voice*>::iterator iter = active_voices_.begin();
    while (iter != active_voices_.end()) {
      Voice* voice = *iter;
      prepareVoiceTriggers(voice);
      processVoice(voice);

      // Remove voice if the right processor has a full silent buffer.
      if (voice_killer_ && voice->state()->event != kVoiceOn &&
          utils::isSilent(voice_killer_->buffer, buffer_size_)) {
        free_voices_.push_back(voice);
        iter = active_voices_.erase(iter);
      }
      else
        iter++;
    }
  }

  void VoiceHandler::setSampleRate(int sample_rate) {
    Processor::setSampleRate(sample_rate);
    voice_router_.setSampleRate(sample_rate);
    global_router_.setSampleRate(sample_rate);
    for (int i = 0; i < all_voices_.size(); ++i)
      all_voices_[i]->processor()->setSampleRate(sample_rate);
  }

  void VoiceHandler::setBufferSize(int buffer_size) {
    Processor::setBufferSize(buffer_size);
    voice_router_.setBufferSize(buffer_size);
    global_router_.setBufferSize(buffer_size);
    for (int i = 0; i < all_voices_.size(); ++i)
      all_voices_[i]->processor()->setBufferSize(buffer_size);
  }

  void VoiceHandler::sustainOn() {
    sustain_ = true;
  }

  void VoiceHandler::sustainOff(int sample) {
    sustain_ = false;
    std::list<Voice*>::iterator iter = sustained_voices_.begin();
    for (; iter != sustained_voices_.end(); ++iter) {
      Voice* voice = *iter;
      voice->deactivate(sample);
    }
    sustained_voices_.clear();
  }

  void VoiceHandler::allNotesOff(int sample) {
    pressed_notes_.clear();

    std::list<Voice*>::iterator iter = active_voices_.begin();
    for (; iter != active_voices_.end(); ++iter) {
      Voice* voice = *iter;
      voice->deactivate(sample);
    }
  }

  void VoiceHandler::noteOn(mopo_float note, mopo_float velocity, int sample) {
    Voice* voice = 0;
    pressed_notes_.push_back(note);
    if (free_voices_.size() && active_voices_.size() < polyphony_) {
      voice = free_voices_.front();
      free_voices_.pop_front();
    }
    else {
      voice = active_voices_.front();
      active_voices_.pop_front();
    }

    voice->activate(note, velocity, sample);
    active_voices_.push_back(voice);
  }

  void VoiceHandler::noteOff(mopo_float note, int sample) {
    pressed_notes_.remove(note);

    std::list<Voice*>::iterator iter = active_voices_.begin();
    for (; iter != active_voices_.end(); ++iter) {
      Voice* voice = *iter;
      if (voice->state()->note == note) {
        if (sustain_)
          sustained_voices_.push_back(voice);
        else {
          if (polyphony_ == 1 && pressed_notes_.size()) {
            mopo_float old_note = pressed_notes_.back();
            voice->activate(old_note, voice->state()->velocity, sample);
          }
          else
            voice->deactivate(sample);
        }
      }
    }
  }

  void VoiceHandler::setAftertouch(mopo_float note, mopo_float aftertouch, int sample) {
    std::list<Voice*>::iterator iter = active_voices_.begin();
    for (; iter != active_voices_.end(); ++iter) {
      Voice* voice = *iter;
      if (voice->state()->note == note)
        voice->setAftertouch(aftertouch, sample);
    }
  }

  void VoiceHandler::setPolyphony(size_t polyphony) {
    while (all_voices_.size() < polyphony) {
      Voice* new_voice = createVoice();
      all_voices_.push_back(std::unique_ptr<Voice>(new_voice));
      free_voices_.push_back(new_voice);
    }

    while (active_voices_.size() > polyphony) {
      active_voices_.front()->deactivate();
      active_voices_.pop_front();
    }

    polyphony_ = polyphony;
  }

  void VoiceHandler::addProcessor(Processor* processor) {
    processor->setBufferSize(getBufferSize());
    processor->setSampleRate(getSampleRate());
    voice_router_.addProcessor(processor);
  }

  void VoiceHandler::removeProcessor(Processor* processor) {
    voice_router_.removeProcessor(processor);
  }

  void VoiceHandler::addGlobalProcessor(Processor* processor) {
    global_router_.addProcessor(processor);
  }

  void VoiceHandler::removeGlobalProcessor(Processor* processor) {
    global_router_.removeProcessor(processor);
  }

  Voice* VoiceHandler::createVoice() {
    return new Voice(voice_router_.clone());
  }
} // namespace mopo
