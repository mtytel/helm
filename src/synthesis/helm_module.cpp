/* Copyright 2013-2015 Matt Tytel
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

#include "helm_module.h"

#include "switch.h"
#include "helm_common.h"

namespace mopo {

  HelmModule::HelmModule() { }

  Value* HelmModule::createBaseControl(std::string name, bool smooth_value) {
    mopo_float default_value = Parameters::getDetails(name).default_value;
    Value* val = 0;
    if (smooth_value) {
      val = new cr::SmoothValue(default_value);
      getMonoRouter()->addProcessor(val);
    }
    else {
      val = new Value(default_value);
      val->setControlRate();
    }

    controls_[name] = val;
    return val;
  }

  Processor* HelmModule::createBaseModControl(std::string name, bool smooth_value) {
    Processor* base_val = createBaseControl(name, smooth_value);

    cr::VariableAdd* mono_total = new cr::VariableAdd();
    mono_total->plugNext(base_val);
    getMonoRouter()->addProcessor(mono_total);
    mono_mod_destinations_[name] = mono_total;
    mono_modulation_readout_[name] = mono_total->output();
    return mono_total;
  }

  Processor* HelmModule::createMonoModControl(std::string name, bool control_rate,
                                              bool smooth_value) {
    ProcessorRouter* mono_owner = getMonoRouter();
    ValueDetails details = Parameters::getDetails(name);

    Processor* base_control = createBaseModControl(name, smooth_value);
    Processor* control_rate_total = base_control;

    if (details.display_skew == ValueDetails::kQuadratic) {
      control_rate_total = new Square();
      control_rate_total->plug(base_control);
      control_rate_total->setControlRate();
      mono_owner->addProcessor(control_rate_total);
    }
    else if (details.display_skew == ValueDetails::kExponential) {
      control_rate_total = new ExponentialScale(2.0);
      control_rate_total->plug(base_control);
      control_rate_total->setControlRate();
      mono_owner->addProcessor(control_rate_total);
    }

    if (control_rate)
      return control_rate_total;

    LinearSmoothBuffer* audio_rate = new LinearSmoothBuffer();
    audio_rate->plug(control_rate_total);
    mono_owner->addProcessor(audio_rate);
    return audio_rate;
  }

  Processor* HelmModule::createPolyModControl(std::string name, bool control_rate,
                                              bool smooth_value) {
    ValueDetails details = Parameters::getDetails(name);
    Processor* base_control = createBaseModControl(name, smooth_value);
    ProcessorRouter* poly_owner = getPolyRouter();

    cr::VariableAdd* poly_total = new cr::VariableAdd();
    poly_owner->addProcessor(poly_total);
    poly_owner->setControlRate(true);
    poly_mod_destinations_[name] = poly_total;

    cr::Add* modulation_total = new cr::Add();
    modulation_total->plug(base_control, 0);
    modulation_total->plug(poly_total, 1);
    poly_owner->addProcessor(modulation_total);

    poly_modulation_readout_[name] = poly_total->output();

    Processor* control_rate_total = modulation_total;
    if (details.display_skew == ValueDetails::kQuadratic) {
      control_rate_total = new Square();
      control_rate_total->setControlRate(true);
      control_rate_total->plug(modulation_total);
      poly_owner->addProcessor(control_rate_total);
    }
    else if (details.display_skew == ValueDetails::kExponential) {
      control_rate_total = new ExponentialScale(2.0);
      control_rate_total->setControlRate(true);
      control_rate_total->plug(modulation_total);
      poly_owner->addProcessor(control_rate_total);
    }

    if (control_rate)
      return control_rate_total;

    SampleAndHoldBuffer* audio_rate = new SampleAndHoldBuffer();
    audio_rate->plug(control_rate_total);
    poly_owner->addProcessor(audio_rate);
    return audio_rate;
  }

  Processor* HelmModule::createTempoSyncSwitch(std::string name, Processor* frequency,
                                               Processor* bps, bool poly) {
    static const Value dotted_ratio(2.0 / 3.0);
    static const Value triplet_ratio(3.0 / 2.0);

    ProcessorRouter* owner = poly ? getPolyRouter() : getMonoRouter();
    Processor* tempo = nullptr;
    if (poly)
      tempo = createPolyModControl(name + "_tempo", 6, false);
    else
      tempo = createMonoModControl(name + "_tempo", 6, false);

    Switch* choose_tempo = new Switch();
    choose_tempo->setControlRate(frequency->isControlRate());
    choose_tempo->plug(tempo, Switch::kSource);

    for (int i = 0; i < sizeof(synced_freq_ratios) / sizeof(Value); ++i)
      choose_tempo->plugNext(&synced_freq_ratios[i]);

    Switch* choose_modifier = new Switch();
    choose_modifier->setControlRate(frequency->isControlRate());
    Value* sync = new Value(1);
    choose_modifier->plug(sync, Switch::kSource);
    choose_modifier->plugNext(&utils::value_one);
    choose_modifier->plugNext(&utils::value_one);
    choose_modifier->plugNext(&dotted_ratio);
    choose_modifier->plugNext(&triplet_ratio);

    Processor* modified_tempo = nullptr;
    Processor* tempo_frequency = nullptr;
    if (frequency->isControlRate()) {
      modified_tempo = new cr::Multiply();
      tempo_frequency = new cr::Multiply();
    }
    else {
      modified_tempo = new Multiply();
      tempo_frequency = new Multiply();
    }

    modified_tempo->plug(choose_tempo, 0);
    modified_tempo->plug(choose_modifier, 1);

    tempo_frequency->plug(modified_tempo, 0);
    tempo_frequency->plug(bps, 1);

    owner->addProcessor(choose_modifier);
    owner->addProcessor(choose_tempo);
    owner->addProcessor(modified_tempo);
    owner->addProcessor(tempo_frequency);

    Switch* choose_frequency = new Switch();
    choose_frequency->setControlRate(frequency->isControlRate());
    choose_frequency->plug(sync, Switch::kSource);
    choose_frequency->plugNext(frequency);
    choose_frequency->plugNext(tempo_frequency);
    choose_frequency->plugNext(tempo_frequency);
    choose_frequency->plugNext(tempo_frequency);

    owner->addProcessor(choose_frequency);
    controls_[name + "_sync"] = sync;
    return choose_frequency;
  }

  void HelmModule::init() {
    for (HelmModule* sub_module : sub_modules_)
      sub_module->init();
  }

  control_map HelmModule::getControls() {
    control_map all_controls = controls_;
    for (HelmModule* sub_module : sub_modules_) {
      control_map sub_controls = sub_module->getControls();
      all_controls.insert(sub_controls.begin(), sub_controls.end());
    }

    return all_controls;
  }

  Processor::Output* HelmModule::getModulationSource(std::string name) {
    if (mod_sources_.count(name))
      return mod_sources_[name];

    for (HelmModule* sub_module : sub_modules_) {
      Processor::Output* source = sub_module->getModulationSource(name);
      if (source)
        return source;
    }

    return 0;
  }

  Processor* HelmModule::getModulationDestination(std::string name, bool poly) {
    if (poly)
      return getPolyModulationDestination(name);
    return getMonoModulationDestination(name);
  }

  Processor* HelmModule::getMonoModulationDestination(std::string name) {
    if (mono_mod_destinations_.count(name))
      return mono_mod_destinations_[name];

    for (HelmModule* sub_module : sub_modules_) {
      Processor* destination = sub_module->getMonoModulationDestination(name);
      if (destination)
        return destination;
    }

    return 0;
  }

  Processor* HelmModule::getPolyModulationDestination(std::string name) {
    if (poly_mod_destinations_.count(name))
      return poly_mod_destinations_[name];

    for (HelmModule* sub_module : sub_modules_) {
      Processor* destination = sub_module->getPolyModulationDestination(name);
      if (destination)
        return destination;
    }

    return 0;
  }

  output_map HelmModule::getModulationSources() {
    output_map all_sources = mod_sources_;
    for (HelmModule* sub_module : sub_modules_) {
      output_map sub_sources = sub_module->getModulationSources();
      all_sources.insert(sub_sources.begin(), sub_sources.end());
    }

    return all_sources;
  }

  output_map HelmModule::getMonoModulations() {
    output_map all_readouts = mono_modulation_readout_;
    for (HelmModule* sub_module : sub_modules_) {
      output_map sub_readouts = sub_module->getMonoModulations();
      all_readouts.insert(sub_readouts.begin(), sub_readouts.end());
    }

    return all_readouts;
  }

  output_map HelmModule::getPolyModulations() {
    output_map all_readouts = poly_modulation_readout_;
    for (HelmModule* sub_module : sub_modules_) {
      output_map sub_readouts = sub_module->getPolyModulations();
      all_readouts.insert(sub_readouts.begin(), sub_readouts.end());
    }

    return all_readouts;
  }

  void HelmModule::correctToTime(mopo_float samples) {
    for (HelmModule* sub_module : sub_modules_)
      sub_module->correctToTime(samples);
  }
} // namespace mopo
