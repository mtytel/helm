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

  Voice::Voice(Processor* processor) : processor_(processor) { }

  VoiceHandler::VoiceHandler(size_t polyphony) :
      Processor(kNumInputs, 1), polyphony_(0), sustain_(false),
      voice_output_(0), voice_killer_(0) {
    setPolyphony(polyphony);
  }

  void VoiceHandler::prepareVoiceTriggers(Voice* voice) {
    note_.clearTrigger();
    velocity_.clearTrigger();
    voice_event_.clearTrigger();

    if (voice->hasNewEvent()) {
      voice_event_.trigger(voice->state()->event);
      if (voice->state()->event == kVoiceOn) {
        note_.trigger(voice->state()->note);
        velocity_.trigger(voice->state()->velocity);
      }

      voice->clearEvent();
    }
  }

  void VoiceHandler::processVoice(Voice* voice) {
    voice->processor()->process();
    for (int i = 0; i < buffer_size_; ++i)
      outputs_[0]->buffer[i] += voice_output_->buffer[i];
  }

  void VoiceHandler::process() {
    global_router_.process();

    size_t polyphony = static_cast<size_t>(inputs_[kPolyphony]->at(0));
    setPolyphony(CLAMP(polyphony, 1, polyphony));
    memset(outputs_[0]->buffer, 0, buffer_size_ * sizeof(mopo_float));

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
    std::set<Voice*>::iterator iter = all_voices_.begin();
    for (; iter != all_voices_.end(); ++iter)
      (*iter)->processor()->setSampleRate(sample_rate);
  }

  void VoiceHandler::setBufferSize(int buffer_size) {
    Processor::setBufferSize(buffer_size);
    voice_router_.setBufferSize(buffer_size);
    global_router_.setBufferSize(buffer_size);
    std::set<Voice*>::iterator iter = all_voices_.begin();
    for (; iter != all_voices_.end(); ++iter)
      (*iter)->processor()->setBufferSize(buffer_size);
  }

  void VoiceHandler::sustainOn() {
    sustain_ = true;
  }

  void VoiceHandler::sustainOff() {
    sustain_ = false;
    std::list<Voice*>::iterator iter = sustained_voices_.begin();
    for (; iter != sustained_voices_.end(); ++iter) {
      Voice* voice = *iter;
      voice->deactivate();
    }
    sustained_voices_.clear();
  }

  void VoiceHandler::noteOn(mopo_float note, mopo_float velocity) {
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

    voice->activate(note, velocity);
    active_voices_.push_back(voice);
  }

  void VoiceHandler::noteOff(mopo_float note) {
    std::list<mopo_float>::iterator note_iter = pressed_notes_.begin();
    while (note_iter != pressed_notes_.end()) {
      if (*note_iter == note)
        note_iter = pressed_notes_.erase(note_iter);
      else
        note_iter++;
    }

    std::list<Voice*>::iterator iter = active_voices_.begin();
    for (; iter != active_voices_.end(); ++iter) {
      Voice* voice = *iter;
      if (voice->state()->note == note) {
        if (sustain_)
          sustained_voices_.push_back(voice);
        else {
          if (polyphony_ == 1 && pressed_notes_.size())
            voice->activate(pressed_notes_.back(), voice->state()->velocity);
          else
            voice->deactivate();
        }
      }
    }
  }

  void VoiceHandler::setPolyphony(size_t polyphony) {
    while (all_voices_.size() < polyphony) {
      Voice* new_voice = createVoice();
      all_voices_.insert(new_voice);
      free_voices_.push_back(new_voice);
    }

    while (active_voices_.size() > polyphony) {
      active_voices_.front()->deactivate();
      active_voices_.pop_front();
    }

    polyphony_ = polyphony;
  }

  void VoiceHandler::addProcessor(Processor* processor) {
    voice_router_.addProcessor(processor);
  }

  void VoiceHandler::addGlobalProcessor(Processor* processor) {
    global_router_.addProcessor(processor);
  }

  Voice* VoiceHandler::createVoice() {
    return new Voice(voice_router_.clone());
  }
} // namespace mopo
