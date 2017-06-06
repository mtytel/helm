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

#include "open_gl_modulation_manager.h"

#include "full_interface.h"
#include "helm_common.h"
#include "modulation_highlight.h"
#include "modulation_look_and_feel.h"
#include "open_gl_modulation_meter.h"
#include "modulation_slider.h"
#include "shaders.h"
#include "synth_gui_interface.h"

#define FLOATS_PER_METER 24
#define INDICES_PER_METER 6
#define POINTS_PER_METER 4

OpenGLModulationManager::OpenGLModulationManager(
    mopo::output_map modulation_sources,
    std::map<std::string, ModulationButton*> modulation_buttons,
    std::map<std::string, SynthSlider*> sliders,
    mopo::output_map mono_modulations,
    mopo::output_map poly_modulations) {
  static const int quad_triangles[6] {
    0, 1, 2,
    2, 3, 0
  };

  modulation_buttons_ = modulation_buttons;
  modulation_sources_ = modulation_sources;
  setInterceptsMouseClicks(false, true);

  current_modulator_ = "";

  polyphonic_destinations_ = new Component();
  polyphonic_destinations_->setInterceptsMouseClicks(false, true);

  monophonic_destinations_ = new Component();
  monophonic_destinations_->setInterceptsMouseClicks(false, true);

  for (auto& mod_button : modulation_buttons_) {
    mod_button.second->addListener(this);
    mod_button.second->addDisconnectListener(this);

    // Create modulation highlight overlays.
    std::string name = mod_button.second->getName().toStdString();
    ModulationHighlight* overlay = new ModulationHighlight();
    addChildComponent(overlay);
    overlay_lookup_[name] = overlay;
    overlay->setName(name);
    Rectangle<int> local_bounds = mod_button.second->getBoundsInParent();
    overlay->setBounds(mod_button.second->getParentComponent()->localAreaToGlobal(local_bounds));
  }

  slider_model_lookup_ = sliders;
  vertices_ = new float[FLOATS_PER_METER * slider_model_lookup_.size()];
  triangles_ = new int[INDICES_PER_METER * slider_model_lookup_.size()];

  int i = 0;
  for (auto& slider : slider_model_lookup_) {
    std::string name = slider.first;
    const mopo::Output* mono_total = mono_modulations[name];
    const mopo::Output* poly_total = poly_modulations[name];

    float* meter_vertices = vertices_ + (i * FLOATS_PER_METER);
    memset(meter_vertices, 0, FLOATS_PER_METER * sizeof(float));

    int* meter_triangles = triangles_ + (i * INDICES_PER_METER);
    for (int t = 0; t < INDICES_PER_METER; ++t)
      meter_triangles[t] = i * POINTS_PER_METER + quad_triangles[t];

    slider.second->addSliderListener(this);

    // Create modulation meter.
    if (mono_total) {
      std::string name = slider.second->getName().toStdString();
      OpenGLModulationMeter* meter = new OpenGLModulationMeter(mono_total, poly_total,
                                                               slider.second, meter_vertices);
      addChildComponent(meter);
      meter_lookup_[name] = meter;
      meter->setName(name);
      Rectangle<int> local_bounds = slider.second->getBoundsInParent();
      meter->setBounds(slider.second->getParentComponent()->localAreaToGlobal(local_bounds));
    }

    // Create modulation slider.
    ModulationSlider* mod_slider = new ModulationSlider(slider.second);
    mod_slider->setLookAndFeel(ModulationLookAndFeel::instance());
    mod_slider->addListener(this);
    if (poly_total)
      polyphonic_destinations_->addAndMakeVisible(mod_slider);
    else
      monophonic_destinations_->addAndMakeVisible(mod_slider);

    slider_lookup_[name] = mod_slider;
    owned_sliders_.push_back(mod_slider);

    ++i;
  }

  addAndMakeVisible(polyphonic_destinations_);
  addAndMakeVisible(monophonic_destinations_);

  forgetModulator();
}

OpenGLModulationManager::~OpenGLModulationManager() {
  for (auto& meter : meter_lookup_)
    delete meter.second;
  for (auto& overlay : overlay_lookup_)
    delete overlay.second;
  for (Slider* slider : owned_sliders_)
    delete slider;
}

void OpenGLModulationManager::paint(Graphics& g) {
}

void OpenGLModulationManager::resized() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  polyphonic_destinations_->setBounds(getBounds());
  monophonic_destinations_->setBounds(getBounds());

  // Update modulation slider locations.
  for (auto& slider : slider_lookup_) {
    SynthSlider* model = slider_model_lookup_[slider.first];
    Point<float> local_top_left = getLocalPoint(model, Point<float>(0.0f, 0.0f));
    slider.second->setVisible(model->isVisible());
    slider.second->setBounds(local_top_left.x, local_top_left.y,
                             model->getWidth(), model->getHeight());
  }

  // Update modulation meter locations.
  for (auto& meter : meter_lookup_) {
    Slider* model = slider_model_lookup_[meter.first];
    Point<float> local_top_left = getLocalPoint(model, Point<float>(0.0f, 0.0f));
    meter.second->setBounds(local_top_left.x, local_top_left.y,
                            model->getWidth(), model->getHeight());
    if (parent) {
      int num_modulations = parent->getSynth()->getNumModulations(meter.first);
      meter.second->setModulated(num_modulations);
      meter.second->setVisible(num_modulations);
    }
  }

  // Update modulation highlight overlay locations.
  for (auto& overlay : overlay_lookup_) {
    ModulationButton* model = modulation_buttons_[overlay.first];
    Point<float> local_top_left = getLocalPoint(model, Point<float>(0.0f, 0.0f));
    overlay.second->setBounds(local_top_left.x, local_top_left.y,
                              model->getWidth(), model->getHeight());
  }

  OpenGLComponent::resized();
}

void OpenGLModulationManager::buttonClicked(juce::Button *clicked_button) {
  std::string name = clicked_button->getName().toStdString();
  if (clicked_button->getToggleState()) {
    if (current_modulator_ != "") {
      Button* modulator = modulation_buttons_[current_modulator_];
      modulator->setToggleState(false, NotificationType::dontSendNotification);
    }
    changeModulator(name);
  }
  else
    forgetModulator();
}

void OpenGLModulationManager::sliderValueChanged(juce::Slider *moved_slider) {
  std::string destination_name = moved_slider->getName().toStdString();
  setModulationAmount(current_modulator_, destination_name, moved_slider->getValue());

  modulation_buttons_[current_modulator_]->repaint();
  last_value_ = moved_slider->getValue();
}

void OpenGLModulationManager::modulationDisconnected(mopo::ModulationConnection* connection, bool last) {
  if (connection->source == current_modulator_) {
    Slider* slider = slider_lookup_[connection->destination];
    slider->setValue(slider->getDoubleClickReturnValue());
  }

  meter_lookup_[connection->destination]->setModulated(!last);
  meter_lookup_[connection->destination]->setVisible(!last);
}

void OpenGLModulationManager::init(OpenGLContext& open_gl_context) {
  open_gl_context.extensions.glGenBuffers(1, &vertex_buffer_);
  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

  int num_meters = slider_model_lookup_.size();
  GLsizeiptr vert_size = static_cast<GLsizeiptr>(num_meters * FLOATS_PER_METER * sizeof(float));
  open_gl_context.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                          vertices_, GL_STATIC_DRAW);

  open_gl_context.extensions.glGenBuffers(1, &triangle_buffer_);
  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);

  GLsizeiptr tri_size = static_cast<GLsizeiptr>(num_meters * INDICES_PER_METER * sizeof(int));
  open_gl_context.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_size,
                                          triangles_, GL_STATIC_DRAW);

  const char* vertex_shader = Shaders::getShader(Shaders::kModulationVertex);
  const char* fragment_shader = Shaders::getShader(Shaders::kModulationFragment);

  shader_ = new OpenGLShaderProgram(open_gl_context);

  if (shader_->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertex_shader)) &&
      shader_->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragment_shader)) &&
      shader_->link()) {
    shader_->use();
    position_ = new OpenGLShaderProgram::Attribute(*shader_, "position");
    coordinates_ = new OpenGLShaderProgram::Attribute(*shader_, "coordinates");
    range_ = new OpenGLShaderProgram::Attribute(*shader_, "range");
    radius_uniform_ = new OpenGLShaderProgram::Uniform(*shader_, "radius");
  }
}

void OpenGLModulationManager::render(OpenGLContext& open_gl_context, bool animate) {
  if (!animate)
    return;

  for (auto& meter : meter_lookup_) {
    bool show = meter.second->isModulated() && slider_model_lookup_[meter.first]->isVisible();
    if (show)
      meter.second->updateDrawing();
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  setViewPort(open_gl_context);

  shader_->use();
  radius_uniform_->set(0.9f);

  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

  int num_meters = slider_model_lookup_.size();
  GLsizeiptr vert_size = static_cast<GLsizeiptr>(num_meters * FLOATS_PER_METER * sizeof(float));
  open_gl_context.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                          vertices_, GL_STATIC_DRAW);
  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);

  if (position_ != nullptr) {
    open_gl_context.extensions.glVertexAttribPointer(position_->attributeID, 2, GL_FLOAT,
                                                     GL_FALSE, 6 * sizeof(float), 0);
    open_gl_context.extensions.glEnableVertexAttribArray(position_->attributeID);
  }

  if (coordinates_ != nullptr) {
    open_gl_context.extensions.glVertexAttribPointer(coordinates_->attributeID, 2, GL_FLOAT,
                                                     GL_FALSE, 6 * sizeof(float),
                                                     (GLvoid*)(2 * sizeof(float)));
    open_gl_context.extensions.glEnableVertexAttribArray(coordinates_->attributeID);
  }

  if (range_ != nullptr) {
    open_gl_context.extensions.glVertexAttribPointer(range_->attributeID, 2, GL_FLOAT,
                                                     GL_FALSE, 6 * sizeof(float),
                                                     (GLvoid*)(4 * sizeof(float)));
    open_gl_context.extensions.glEnableVertexAttribArray(range_->attributeID);
  }

  glDrawElements(GL_TRIANGLES, num_meters * INDICES_PER_METER, GL_UNSIGNED_INT, 0);

  if (position_ != nullptr)
    open_gl_context.extensions.glDisableVertexAttribArray(position_->attributeID);

  if (coordinates_ != nullptr)
    open_gl_context.extensions.glDisableVertexAttribArray(coordinates_->attributeID);

  if (range_ != nullptr)
    open_gl_context.extensions.glDisableVertexAttribArray(range_->attributeID);

  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLModulationManager::destroy(OpenGLContext& open_gl_context) {
  shader_ = nullptr;
  position_ = nullptr;
  coordinates_ = nullptr;
  range_ = nullptr;
  radius_uniform_ = nullptr;

  open_gl_context.extensions.glDeleteBuffers(1, &vertex_buffer_);
  open_gl_context.extensions.glDeleteBuffers(1, &triangle_buffer_);
}

void OpenGLModulationManager::hoverStarted(const std::string& name) {
  makeModulationsVisible(name, true);
}

void OpenGLModulationManager::hoverEnded(const std::string& name) {
  makeModulationsVisible(name, false);
}

void OpenGLModulationManager::modulationsChanged(const std::string& destination) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;
  
  int num_modulations = parent->getSynth()->getNumModulations(destination);
  meter_lookup_[destination]->setModulated(num_modulations);
  meter_lookup_[destination]->setVisible(num_modulations);
}

void OpenGLModulationManager::setModulationAmount(std::string source, std::string destination,
                                                  mopo::mopo_float amount) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  parent->getSynth()->changeModulationAmount(source, destination, amount);
  modulationsChanged(destination);
}

void OpenGLModulationManager::forgetModulator() {
  polyphonic_destinations_->setVisible(false);
  monophonic_destinations_->setVisible(false);
  current_modulator_ = "";
}

void OpenGLModulationManager::reset() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  for (auto& meter : meter_lookup_) {
    int num_modulations = parent->getSynth()->getNumModulations(meter.first);
    meter.second->setModulated(num_modulations);
    meter.second->setVisible(num_modulations);
  }

  setSliderValues();
}

void OpenGLModulationManager::makeModulationsVisible(std::string destination, bool visible) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  std::vector<mopo::ModulationConnection*> connections =
      parent->getSynth()->getDestinationConnections(destination);

  for (mopo::ModulationConnection* connection : connections)
    overlay_lookup_[connection->source]->setVisible(visible);
}

void OpenGLModulationManager::setSliderValues() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  std::vector<mopo::ModulationConnection*> connections =
      parent->getSynth()->getSourceConnections(current_modulator_);
  for (auto& slider : slider_lookup_) {
    std::string destination_name = slider.second->getName().toStdString();
    float value = 0.0f;

    for (mopo::ModulationConnection* connection : connections) {
      if (connection->destination == destination_name) {
        value = connection->amount.value();
        break;
      }
    }
    slider.second->setValue(value, NotificationType::dontSendNotification);
    slider.second->repaint();
  }
}

void OpenGLModulationManager::changeModulator(std::string new_modulator) {
  current_modulator_ = new_modulator;
  setSliderValues();

  for (auto& slider : slider_lookup_) {
    SynthSlider* model = slider_model_lookup_[slider.first];
    slider.second->setVisible(model->isVisible());
  }

  polyphonic_destinations_->setVisible(true);
  polyphonic_destinations_->repaint();
  monophonic_destinations_->setVisible(true);
  monophonic_destinations_->repaint();
}
