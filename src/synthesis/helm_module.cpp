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

#include "value_switch.h"
#include "gate.h"
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
      val = new cr::Value(default_value);
      getMonoRouter()->addIdleProcessor(val);
    }

    controls_[name] = val;
    return val;
  }

  ValueSwitch* HelmModule::createBaseSwitchControl(std::string name) {
    mopo_float default_value = Parameters::getDetails(name).default_value;
    
    ValueSwitch* val = new ValueSwitch(default_value);
    getMonoRouter()->addIdleProcessor(val);
    controls_[name] = val;
    return val;
  }

  Output* HelmModule::createBaseModControl(std::string name, bool smooth_value) {
    Processor* base_val = createBaseControl(name, smooth_value);

    cr::VariableAdd* mono_total = new cr::VariableAdd();
    mono_total->plugNext(base_val);
    getMonoRouter()->addProcessor(mono_total);
    mono_mod_destinations_[name] = mono_total;
    mono_modulation_readout_[name] = mono_total->output();

    ValueSwitch* control_switch = new ValueSwitch(0.0);
    control_switch->plugNext(base_val);
    control_switch->plugNext(mono_total);
    control_switch->addProcessor(mono_total);
    getMonoRouter()->addProcessor(control_switch);
    control_switch->set(0);
    mono_modulation_switches_[name] = control_switch;

    return control_switch->output(ValueSwitch::kSwitch);
  }

  Output* HelmModule::createMonoModControl(std::string name, bool control_rate,
                                           bool smooth_value) {
    ProcessorRouter* mono_owner = getMonoRouter();
    ValueDetails details = Parameters::getDetails(name);
    Output* control_rate_total = createBaseModControl(name, smooth_value);

    if (details.display_skew == ValueDetails::kQuadratic) {
      Processor* scale = nullptr;
      if (details.post_offset)
        scale = new cr::Quadratic(details.post_offset);
      else
        scale = new cr::Square();
      
      scale->plug(control_rate_total);
      mono_owner->addProcessor(scale);
      control_rate_total = scale->output();
    }
    else if (details.display_skew == ValueDetails::kExponential) {
      cr::ExponentialScale* exponential = new cr::ExponentialScale(2.0);
      exponential->plug(control_rate_total);
      mono_owner->addProcessor(exponential);
      control_rate_total = exponential->output();
    }
    else if (details.display_skew == ValueDetails::kSquareRoot) {
      cr::Root* root = new cr::Root(details.post_offset);
      root->plug(control_rate_total);
      mono_owner->addProcessor(root);
      control_rate_total = root->output();
    }

    if (control_rate)
      return control_rate_total;

    SampleAndHoldBuffer* audio_rate = new SampleAndHoldBuffer();
    audio_rate->plug(control_rate_total);
    mono_owner->addProcessor(audio_rate);

    return audio_rate->output();
  }

  Output* HelmModule::createPolyModControl(std::string name, bool control_rate,
                                           bool smooth_value) {
    ValueDetails details = Parameters::getDetails(name);
    Output* base_control = createBaseModControl(name, smooth_value);
    ProcessorRouter* poly_owner = getPolyRouter();

    cr::VariableAdd* poly_total = new cr::VariableAdd();
    poly_owner->addProcessor(poly_total);
    poly_mod_destinations_[name] = poly_total;

    cr::Add* modulation_total = new cr::Add();
    modulation_total->plug(base_control, 0);
    modulation_total->plug(poly_total, 1);
    poly_owner->addProcessor(modulation_total);

    poly_modulation_readout_[name] = poly_total->output();

    ValueSwitch* control_switch = new ValueSwitch(0.0);
    control_switch->plugNext(base_control);
    control_switch->plugNext(modulation_total);
    control_switch->addProcessor(poly_total);
    control_switch->addProcessor(modulation_total);
    control_switch->set(0);
    poly_owner->addProcessor(control_switch);
    poly_modulation_switches_[name] = control_switch;

    Output* control_rate_total = control_switch->output(ValueSwitch::kSwitch);
    if (details.display_skew == ValueDetails::kQuadratic) {
      Processor* scale = nullptr;
      if (details.post_offset)
        scale = new cr::Quadratic(details.post_offset);
      else
        scale = new cr::Square();
      
      scale->plug(control_rate_total);
      poly_owner->addProcessor(scale);
      control_rate_total = scale->output();
    }
    else if (details.display_skew == ValueDetails::kExponential) {
      cr::ExponentialScale* exponential = new cr::ExponentialScale(2.0, details.post_offset);
      exponential->plug(control_rate_total);
      poly_owner->addProcessor(exponential);
      control_rate_total = exponential->output();
    }
    else if (details.display_skew == ValueDetails::kSquareRoot) {
      cr::Root* root = new cr::Root(details.post_offset);
      root->plug(control_rate_total);
      poly_owner->addProcessor(root);
      control_rate_total = root->output();
    }

    if (control_rate)
      return control_rate_total;

    SampleAndHoldBuffer* audio_rate = new SampleAndHoldBuffer();
    audio_rate->plug(control_rate_total);
    poly_owner->addProcessor(audio_rate);
    return audio_rate->output();
  }

  Output* HelmModule::createTempoSyncSwitch(std::string name, Processor* frequency,
                                            Output* bps, bool poly, ValueSwitch* owner) {
    static const Value dotted_ratio(2.0 / 3.0);
    static const Value triplet_ratio(3.0 / 2.0);

    ProcessorRouter* router = poly ? getPolyRouter() : getMonoRouter();
    Output* tempo = nullptr;
    if (poly)
      tempo = createPolyModControl(name + "_tempo", frequency->isControlRate());
    else
      tempo = createMonoModControl(name + "_tempo", frequency->isControlRate());

    Gate* choose_tempo = new Gate();
    choose_tempo->plug(tempo, Gate::kChoice);

    for (int i = 0; i < sizeof(synced_freq_ratios) / sizeof(Value); ++i)
      choose_tempo->plugNext(&synced_freq_ratios[i]);

    Gate* choose_modifier = new Gate();
    Value* sync = new cr::Value(1);
    router->addIdleProcessor(sync);
    choose_modifier->plug(sync, Gate::kChoice);
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
    router->addProcessor(modified_tempo);
    router->addProcessor(tempo_frequency);

    Gate* choose_frequency = new Gate();
    choose_frequency->plug(sync, Gate::kChoice);
    choose_frequency->plugNext(frequency);
    choose_frequency->plugNext(tempo_frequency);
    choose_frequency->plugNext(tempo_frequency);
    choose_frequency->plugNext(tempo_frequency);

    if (owner) {
      owner->addProcessor(choose_tempo);
      owner->addProcessor(choose_modifier);
      owner->addProcessor(modified_tempo);
      owner->addProcessor(tempo_frequency);
      owner->addProcessor(choose_frequency);
      owner->set(owner->value());
    }

    getMonoRouter()->addProcessor(choose_frequency);
    controls_[name + "_sync"] = sync;
    return choose_frequency->output();
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
    Processor* poly_destination = getPolyModulationDestination(name);

    if (poly && poly_destination)
      return poly_destination;

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

  ValueSwitch* HelmModule::getModulationSwitch(std::string name, bool poly) {
    if (poly)
      return getPolyModulationSwitch(name);
    return getMonoModulationSwitch(name);
  }

  ValueSwitch* HelmModule::getMonoModulationSwitch(std::string name) {
    if (mono_modulation_switches_.count(name))
      return mono_modulation_switches_[name];

    for (HelmModule* sub_module : sub_modules_) {
      ValueSwitch* value_switch = sub_module->getMonoModulationSwitch(name);
      if (value_switch)
        return value_switch;
    }

    return 0;
  }

  ValueSwitch* HelmModule::getPolyModulationSwitch(std::string name) {
    if (poly_modulation_switches_.count(name))
      return poly_modulation_switches_[name];

    for (HelmModule* sub_module : sub_modules_) {
      ValueSwitch* value_switch = sub_module->getPolyModulationSwitch(name);
      if (value_switch)
        return value_switch;
    }

    return 0;
  }

  void HelmModule::updateAllModulationSwitches() {
    for (auto& mod_switch : mono_modulation_switches_) {
      bool enable = mono_mod_destinations_[mod_switch.first]->connectedInputs() > 1;
      if (poly_mod_destinations_.count(mod_switch.first))
        enable = enable || poly_mod_destinations_[mod_switch.first]->connectedInputs() > 0;
      mod_switch.second->set(enable);
    }

    for (auto& mod_switch : poly_modulation_switches_)
      mod_switch.second->set(poly_mod_destinations_[mod_switch.first]->connectedInputs() > 0);

    for (HelmModule* sub_module : sub_modules_)
      sub_module->updateAllModulationSwitches();
  }

  output_map& HelmModule::getModulationSources() {
    output_map& all_sources = mod_sources_;
    for (HelmModule* sub_module : sub_modules_) {
      output_map& sub_sources = sub_module->getModulationSources();
      all_sources.insert(sub_sources.begin(), sub_sources.end());
    }

    return all_sources;
  }

  output_map& HelmModule::getMonoModulations() {
    output_map& all_readouts = mono_modulation_readout_;
    for (HelmModule* sub_module : sub_modules_) {
      output_map& sub_readouts = sub_module->getMonoModulations();
      all_readouts.insert(sub_readouts.begin(), sub_readouts.end());
    }

    return all_readouts;
  }

  output_map& HelmModule::getPolyModulations() {
    output_map& all_readouts = poly_modulation_readout_;
    for (HelmModule* sub_module : sub_modules_) {
      output_map& sub_readouts = sub_module->getPolyModulations();
      all_readouts.insert(sub_readouts.begin(), sub_readouts.end());
    }

    return all_readouts;
  }

  void HelmModule::correctToTime(mopo_float samples) {
    for (HelmModule* sub_module : sub_modules_)
      sub_module->correctToTime(samples);
  }
} // namespace mopo
