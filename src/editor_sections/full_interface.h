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

#pragma once
#ifndef FULL_INTERFACE_H
#define FULL_INTERFACE_H

#include "JuceHeader.h"

#include "about_section.h"
#include "arp_section.h"
#include "bpm_section.h"
#include "contribute_section.h"
#include "global_tool_tip.h"
#include "open_gl_modulation_manager.h"
#include "oscilloscope.h"
#include "open_gl_background.h"
#include "open_gl_oscilloscope.h"
#include "open_gl_modulation_meter.h"
#include "overlay.h"
#include "patch_browser.h"
#include "patch_selector.h"
#include "synthesis_interface.h"
#include "synth_section.h"
#include "update_check_section.h"

class FullInterface : public SynthSection, public OpenGLRenderer {
  public:
    FullInterface(mopo::control_map controls, mopo::output_map modulation_sources,
                  mopo::output_map mono_modulations, mopo::output_map poly_modulations,
                  MidiKeyboardState* keyboard_state);
    ~FullInterface();

    void setOutputMemory(const float* output_memory);

    void createModulationSliders(mopo::output_map modulation_sources,
                                 mopo::output_map mono_modulations,
                                 mopo::output_map poly_modulations);

    void setToolTipText(String parameter, String value);

    void paint(Graphics& g) override;
    void paintBackground(Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* clicked_button) override;
    void animate(bool animate = true) override;
    void checkBackground();

    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

    void resetModulations() { modulation_manager_->reset(); }
    void setFocus() { synthesis_interface_->setFocus(); }
    void notifyChange() { patch_selector_->setModified(true); }
    void notifyFresh();
    void externalPatchLoaded(File patch) { patch_browser_->externalPatchLoaded(patch); }

  private:
    std::map<std::string, SynthSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    ScopedPointer<OpenGLModulationManager> modulation_manager_;
    ScopedPointer<SynthSlider> arp_tempo_;

    ScopedPointer<AboutSection> about_section_;
    ScopedPointer<ContributeSection> contribute_section_;
    ScopedPointer<UpdateCheckSection> update_check_section_;
    ScopedPointer<Component> standalone_settings_section_;
    ScopedPointer<ImageButton> logo_button_;
    ScopedPointer<ArpSection> arp_section_;
    ScopedPointer<SynthesisInterface> synthesis_interface_;
    ScopedPointer<OpenGLOscilloscope> oscilloscope_;
    ScopedPointer<BpmSection> bpm_section_;
    ScopedPointer<GlobalToolTip> global_tool_tip_;
    ScopedPointer<PatchSelector> patch_selector_;
    ScopedPointer<PatchBrowser> patch_browser_;
    ScopedPointer<SaveSection> save_section_;
    ScopedPointer<DeleteSection> delete_section_;
    ScopedPointer<VolumeSection> volume_section_;

    bool animate_;
    OpenGLContext open_gl_context;
    Image background_image_;
    OpenGLBackground background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FullInterface)
};

#endif // FULL_INTERFACE_H
