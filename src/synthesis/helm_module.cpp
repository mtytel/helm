/* Copyright 2013-2017 Matt Tytel
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

  HelmModule::OutputSwitch::OutputSwitch() : Processor(0, 1) {
    raw_source_ = nullptr;
    processed_source_ = nullptr;
  }

  void HelmModule::OutputSwitch::destroy() {
    output()->buffer = original_buffer_;
  }

  void HelmModule::OutputSwitch::enable(bool enable) {
    for (Processor* processor : processors_)
      processor->enable(enable);

    if (enable && processed_source_)
      outputs_->at(0) = processed_source_;
    else if (raw_source_)
      outputs_->at(0) = raw_source_;
  }

  HelmModule::HelmModule() { }

  Value* HelmModule::createBaseControl(std::string name, bool smooth_value) {
    mopo_float default_value = Parameters::getDetails(name).default_value;
    Value* val = 0;
    if (smooth_value) {
      val = new cr::SmoothValue(default_value);
      getMonoRouter()->addProcessor(val);
    }
    else {
      val = new cr::Value(default_value);
      getMonoRouter()->addIdleProcessor(val);
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

    OutputSwitch* control_switch = new OutputSwitch();
    control_switch->setRawSource(controls_[name]->output());
    control_switch->setProcessedSource(control_rate_total->output());
    control_switch->addProcessor(base_control);
    control_switch->enable(true);
    mono_owner->addIdleProcessor(control_switch);

    control_rate_total = control_switch;
    if (details.display_skew == ValueDetails::kQuadratic) {
      control_rate_total = new cr::Square();
      control_rate_total->plug(base_control);
      mono_owner->addProcessor(control_rate_total);
    }
    else if (details.display_skew == ValueDetails::kExponential) {
      control_rate_total = new cr::ExponentialScale(2.0);
      control_rate_total->plug(base_control);
      mono_owner->addProcessor(control_rate_total);
    }

    if (control_rate)
      return control_switch;

    SampleAndHoldBuffer* audio_rate = new SampleAndHoldBuffer();
    audio_rate->plug(control_switch);
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
    poly_mod_destinations_[name] = poly_total;

    cr::Add* modulation_total = new cr::Add();
    modulation_total->plug(base_control, 0);
    modulation_total->plug(poly_total, 1);
    poly_owner->addProcessor(modulation_total);

    poly_modulation_readout_[name] = poly_total->output();

    OutputSwitch* control_switch = new OutputSwitch();
    control_switch->setRawSource(controls_[name]->output());
    control_switch->setProcessedSource(modulation_total->output());
    control_switch->addProcessor(base_control);
    control_switch->addProcessor(poly_total);
    control_switch->addProcessor(modulation_total);
    control_switch->enable(true);
    poly_owner->addIdleProcessor(control_switch);

    Processor* control_rate_total = control_switch;
    if (details.display_skew == ValueDetails::kQuadratic) {
      control_rate_total = new cr::Square();
      control_rate_total->plug(modulation_total);
      poly_owner->addProcessor(control_rate_total);
    }
    else if (details.display_skew == ValueDetails::kExponential) {
      control_rate_total = new cr::ExponentialScale(2.0);
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
      tempo = createPolyModControl(name + "_tempo", frequency->isControlRate());
    else
      tempo = createMonoModControl(name + "_tempo", frequency->isControlRate());

    Switch* choose_tempo = new Switch();
    choose_tempo->plug(tempo, Switch::kSet);

    for (int i = 0; i < sizeof(synced_freq_ratios) / sizeof(Value); ++i)
      choose_tempo->plugNext(&synced_freq_ratios[i]);

    Switch* choose_modifier = new Switch();
    Value* sync = new cr::Value(1);
    owner->addIdleProcessor(sync);
    choose_modifier->plug(sync, Switch::kSet);
    choose_modifier->plugNext(&utils::value_one);
    choose_modifier->plugNext(&utils::value_one);
    choose_modifier->plugNext(&dotted_ratio);
    choose_modifier->plugNext(&triplet_ratio);

    Processor* modified_tempo = new cr::Multiply();
    Processor* tempo_frequency = new cr::Multiply();

    modified_tempo->plug(choose_tempo, 0);
    modified_tempo->plug(choose_modifier, 1);

    tempo_frequency->plug(modified_tempo, 0);
    tempo_frequency->plug(bps, 1);

    getMonoRouter()->addProcessor(choose_modifier);
    getMonoRouter()->addProcessor(choose_tempo);
    owner->addProcessor(modified_tempo);
    owner->addProcessor(tempo_frequency);

    Switch* choose_frequency = new Switch();
    choose_frequency->plug(sync, Switch::kSet);
    choose_frequency->plugNext(frequency);
    choose_frequency->plugNext(tempo_frequency);
    choose_frequency->plugNext(tempo_frequency);
    choose_frequency->plugNext(tempo_frequency);

    getMonoRouter()->addProcessor(choose_frequency);
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

  Output* HelmModule::getModulationSource(std::string name) {
    if (mod_sources_.count(name))
      return mod_sources_[name];

    for (HelmModule* sub_module : sub_modules_) {
      Output* source = sub_module->getModulationSource(name);
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
