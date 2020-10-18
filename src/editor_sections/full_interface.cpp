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

#include "full_interface.h"

#include "colors.h"
#include "fonts.h"
#include "helm_engine.h"
#include "helm_common.h"
#include "load_save.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

#define TOP_HEIGHT 64

#ifndef PAY_NAG
  #define PAY_NAG 1
#endif

FullInterface::FullInterface(mopo::control_map controls, mopo::output_map modulation_sources,
                             mopo::output_map mono_modulations,
                             mopo::output_map poly_modulations,
                             MidiKeyboardState* keyboard_state) : SynthSection("full_interface") {
  animate_ = true;
  open_gl_context.setContinuousRepainting(true);
  open_gl_context.setRenderer(this);
  open_gl_context.attachTo(*getTopLevelComponent());
  open_gl_context.setOpenGLVersionRequired(OpenGLContext::openGL3_2);

  addSubSection(synthesis_interface_ = new SynthesisInterface(controls, keyboard_state));
  addSubSection(arp_section_ = new ArpSection(TRANS("ARP")));
  addSubSection(bpm_section_ = new BpmSection(TRANS("BPM")));

  addSubSection(patch_selector_ = new PatchSelector());
  addAndMakeVisible(global_tool_tip_ = new GlobalToolTip());
  addSubSection(volume_section_ = new VolumeSection("VOLUME"));
  addOpenGLComponent(oscilloscope_ = new OpenGLOscilloscope());

  setAllValues(controls);
  createModulationSliders(modulation_sources, mono_modulations, poly_modulations);

  logo_button_ = new ImageButton("logo_button");
  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  if (display.scale > 1.5) {
    Image helm = ImageCache::getFromMemory(BinaryData::helm_icon_128_2x_png,
                                           BinaryData::helm_icon_128_2x_pngSize);
    logo_button_->setImages(true, true, false,
                            helm, 1.0, Colours::transparentBlack,
                            helm, 1.0, Colour(0x11ffffff),
                            helm, 1.0, Colour(0x11000000));
  }
  else {
    Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_128_1x_png,
                                                 BinaryData::helm_icon_128_1x_pngSize);
    logo_button_->setImages(true, true, false,
                            helm_small, 1.0, Colours::transparentBlack,
                            helm_small, 1.0, Colour(0x11ffffff),
                            helm_small, 1.0, Colour(0x11000000));
  }
  addAndMakeVisible(logo_button_);
  logo_button_->addListener(this);

  addChildComponent(patch_browser_ = new PatchBrowser());
  patch_selector_->setBrowser(patch_browser_);

  addChildComponent(save_section_ = new SaveSection("save_section"));
  patch_browser_->setSaveSection(save_section_);
  patch_selector_->setSaveSection(save_section_);

  addChildComponent(delete_section_ = new DeleteSection("delete_section"));
  patch_browser_->setDeleteSection(delete_section_);

  about_section_ = new AboutSection("about");
  addChildComponent(about_section_);

#if PAY_NAG
  if (LoadSave::shouldAskForPayment()) {
    contribute_section_ = new ContributeSection("contribute");
    addAndMakeVisible(contribute_section_);
  }
#endif

  update_check_section_ = new UpdateCheckSection("update_check");
  addChildComponent(update_check_section_);

  synthesis_interface_->toFront(true);
  modulation_manager_->toFront(false);
  patch_browser_->toFront(false);
  about_section_->toFront(false);
  if (contribute_section_)
    contribute_section_->toFront(false);
  save_section_->toFront(false);
  delete_section_->toFront(false);

  setOpaque(true);
}

FullInterface::~FullInterface() {
  open_gl_context.detach();
  open_gl_context.setRenderer(nullptr);
  about_section_ = nullptr;
  contribute_section_ = nullptr;
  update_check_section_ = nullptr;
  arp_section_ = nullptr;
  oscilloscope_ = nullptr;
  synthesis_interface_ = nullptr;
  bpm_section_ = nullptr;
  global_tool_tip_ = nullptr;
  patch_selector_ = nullptr;
  save_section_ = nullptr;
  delete_section_ = nullptr;
  volume_section_ = nullptr;
}

void FullInterface::paint(Graphics& g) { }

void FullInterface::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
  static const DropShadow logo_shadow(Colour(0xff000000), 8, Point<int>(0, 0));
  static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
  Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_32_2x_png,
                                               BinaryData::helm_icon_32_2x_pngSize);
  g.setColour(Colors::background);
  g.fillRect(getLocalBounds());

  shadow.drawForRectangle(g, arp_section_->getBounds());
  shadow.drawForRectangle(g, oscilloscope_->getBounds());
  shadow.drawForRectangle(g, patch_selector_->getBounds());

  int logo_padding = 2 * size_ratio_;
  int x = logo_button_->getX() - logo_padding;
  int width = logo_button_->getWidth() + 2 * logo_padding;

  shadow.drawForRectangle(g, Rectangle<int>(x, logo_button_->getY(),
                                            width, logo_button_->getHeight()));
  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(x, logo_button_->getY(), width, logo_button_->getHeight(), 3.0f);

  g.saveState();
  g.setOrigin(logo_button_->getX(), logo_button_->getY());
  g.addTransform(AffineTransform::scale(size_ratio_, size_ratio_));

  logo_shadow.drawForImage(g, helm_small);
  g.restoreState();

  component_shadow.drawForRectangle(g, patch_selector_->getBounds());
  component_shadow.drawForRectangle(g, volume_section_->getBounds());

  paintKnobShadows(g);
  paintChildrenBackgrounds(g);
}

void FullInterface::resized() {
  int left = 0;
  int width = getWidth();
  int height = getHeight();
  float ratio = 1.0f;
  float width_ratio = getWidth() / (1.0f * mopo::DEFAULT_WINDOW_WIDTH);
  float height_ratio = getHeight() / (1.0f * mopo::DEFAULT_WINDOW_HEIGHT);
  if (width_ratio > height_ratio) {
    ratio = height_ratio;
    width = height_ratio * mopo::DEFAULT_WINDOW_WIDTH;
    left = (getWidth() - width) / 2;
  }
  else {
    ratio = width_ratio;
    height = width_ratio * mopo::DEFAULT_WINDOW_HEIGHT;
  }

  setSizeRatio(ratio);
  save_section_->setSizeRatio(ratio);
  delete_section_->setSizeRatio(ratio);
  patch_browser_->setSizeRatio(ratio);
  about_section_->setSizeRatio(ratio);
  if (contribute_section_)
    contribute_section_->setSizeRatio(ratio);
  int padding = 8 * ratio;
  int top_height = TOP_HEIGHT * ratio;

  int section_one_width = 320 * ratio;
  int section_two_width = section_one_width;
  int section_three_width = width - section_one_width - section_two_width - 4 * padding;

  synthesis_interface_->setPadding(padding);
  synthesis_interface_->setSectionOneWidth(section_one_width);
  synthesis_interface_->setSectionTwoWidth(section_two_width);
  synthesis_interface_->setSectionThreeWidth(section_three_width);

  int logo_padding = 2 * ratio;
  int logo_width = top_height + 2 * logo_padding;

  int patch_selector_width = section_one_width - logo_width - padding;

  logo_button_->setBounds(left + padding + logo_padding, padding, top_height, top_height);
  patch_selector_->setBounds(logo_button_->getRight() + padding + logo_padding, padding,
                             patch_selector_width, top_height);
  global_tool_tip_->setBounds(patch_selector_->getX() + 0.11 * patch_selector_->getWidth(),
                              patch_selector_->getY(),
                              0.78 * patch_selector_->getWidth(),
                              patch_selector_->getBrowseHeight());

  int volume_width = (section_two_width - padding) / 2;
  int oscilloscope_width = section_two_width - padding - volume_width;
  volume_section_->setBounds(patch_selector_->getRight() + padding, padding,
                             volume_width, top_height);

  oscilloscope_->setBounds(volume_section_->getRight() + padding, padding,
                           oscilloscope_width, top_height);

  int bpm_width = 40 * ratio;
  int arp_width = section_three_width - bpm_width - padding;
  bpm_section_->setBounds(oscilloscope_->getRight() + padding, padding,
                          bpm_width, top_height);

  arp_section_->setBounds(bpm_section_->getRight() + padding, padding,
                          arp_width, top_height);

  synthesis_interface_->setBounds(left, top_height + padding,
                                  width, height - top_height - padding);

  about_section_->setBounds(getBounds());
  if (contribute_section_)
    contribute_section_->setBounds(getBounds());
  update_check_section_->setBounds(getBounds());
  save_section_->setBounds(getBounds());
  delete_section_->setBounds(getBounds());

  patch_browser_->setBounds(synthesis_interface_->getX() + padding, synthesis_interface_->getY(),
                            arp_section_->getRight() - synthesis_interface_->getX() - padding,
                            synthesis_interface_->getHeight() - padding);

  SynthSection::resized();
  modulation_manager_->setBounds(getBounds());

  checkBackground();
}

void FullInterface::setOutputMemory(const float* output_memory) {
  oscilloscope_->setOutputMemory(output_memory);
}

void FullInterface::createModulationSliders(mopo::output_map modulation_sources,
                                            mopo::output_map mono_modulations,
                                            mopo::output_map poly_modulations) {
  std::map<std::string, SynthSlider*> all_sliders = getAllSliders();
  std::map<std::string, SynthSlider*> modulatable_sliders;

  for (auto& destination : mono_modulations) {
    if (all_sliders.count(destination.first))
      modulatable_sliders[destination.first] = all_sliders[destination.first];
  }

  modulation_manager_ = new OpenGLModulationManager(modulation_sources,
                                                    getAllModulationButtons(),
                                                    modulatable_sliders,
                                                    mono_modulations, poly_modulations);
  modulation_manager_->setOpaque(false);
  addOpenGLComponent(modulation_manager_);
}

void FullInterface::setToolTipText(String parameter, String value) {
  if (global_tool_tip_)
    global_tool_tip_->setText(parameter, value);
}

void FullInterface::buttonClicked(Button* clicked_button) {
  if (clicked_button == logo_button_) {
    about_section_->setVisible(true);
  }
  else
    SynthSection::buttonClicked(clicked_button);
}

void FullInterface::animate(bool animate) {
  animate_ = animate;
  SynthSection::animate(animate);
  open_gl_context.setContinuousRepainting(animate);
  repaint();
}

void FullInterface::checkBackground() {
  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  int width = scale * getWidth();
  int height = scale * getHeight();

  if (background_image_.getWidth() != width || background_image_.getHeight() != height) {
    background_image_ = Image(Image::ARGB, width, height, true);
    Graphics g(background_image_);
    g.addTransform(AffineTransform::scale(scale, scale));
    paintBackground(g);
    background_.updateBackgroundImage(background_image_);
  }
}

void FullInterface::newOpenGLContextCreated() {
  background_.init(open_gl_context);
  initOpenGLComponents(open_gl_context);
}

void FullInterface::renderOpenGL() {
  background_.render(open_gl_context);
  renderOpenGLComponents(open_gl_context, animate_);
}

void FullInterface::openGLContextClosing() {
  background_.destroy(open_gl_context);
  destroyOpenGLComponents(open_gl_context);
}

void FullInterface::notifyFresh() {
  global_tool_tip_->setVisible(false);
  patch_selector_->setModified(false);
}
