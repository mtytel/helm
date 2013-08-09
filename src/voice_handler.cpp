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

#include "voice_handler.h"

#include "utils.h"

namespace laf {

  Voice::Voice(Processor* voice) : note_(0), velocity_(0), voice_(voice) { }

  void Voice::activate(laf_sample new_note, laf_sample new_velocity) {
    note_ = new_note;
    velocity_ = new_velocity;
    voice_->trigger(true);
  }

  void Voice::deactivate() {
    voice_->trigger(false);
  }

  VoiceHandler::VoiceHandler(int polyphony) :
      Processor(0, 1), polyphony_(0), sustain_(false),
      voice_output_(0), voice_killer_(0) {
    setPolyphony(polyphony);
  }

  void VoiceHandler::processVoice(Voice* voice) {
    note_.set(voice->note());
    velocity_.set(voice->velocity());
    voice->voice()->process();
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] += voice_output_->output()->buffer[i];
  }

  void VoiceHandler::process() {
    memset(outputs_[0]->buffer, 0, BUFFER_SIZE * sizeof(laf_sample));

    std::list<Voice*>::iterator iter = active_voices_.begin();

    while (iter != active_voices_.end()) {
      Voice* voice = *iter;
      processVoice(voice);
      if (voice_killer_ &&
          utils::isSilent(voice_killer_->output()->buffer, BUFFER_SIZE)) {
        free_voices_.push_back(voice);
        iter = active_voices_.erase(iter);
      }
      else
        iter++;
    }
  }

  void VoiceHandler::setSampleRate(int sample_rate) {
    std::set<Voice*>::iterator iter = all_voices_.begin();
    for (; iter != all_voices_.end(); ++iter)
      (*iter)->voice()->setSampleRate(sample_rate);
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

  void VoiceHandler::noteOn(laf_sample note, laf_sample velocity) {
    Voice* voice = 0;
    if (free_voices_.size()) {
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

  void VoiceHandler::noteOff(laf_sample note) {
    std::list<Voice*>::iterator iter = active_voices_.begin();
    for (; iter != active_voices_.end(); ++iter) {
      Voice* voice = *iter;
      if (voice->note() == note) {
        if (sustain_)
          sustained_voices_.push_back(voice);
        else
          voice->deactivate();
      }
    }
  }

  void VoiceHandler::setPolyphony(int polyphony) {
    for (int i = polyphony_; i < polyphony; ++i) {
      Voice* new_voice = createVoice();
      all_voices_.insert(new_voice);
      free_voices_.push_back(new_voice);
    }

    polyphony_ = polyphony;
  }

  void VoiceHandler::addProcessor(Processor* processor) {
    router_.addProcessor(processor);
  }

  Voice* VoiceHandler::createVoice() {
    return new Voice(router_.clone());
  }
} // namespace laf
