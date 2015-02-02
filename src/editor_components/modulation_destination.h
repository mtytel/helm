/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_B702F1CB5F8201CF__
#define __JUCE_HEADER_B702F1CB5F8201CF__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "twytch_common.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ModulationDestination  : public Component,
                               public DragAndDropTarget,
                               public SliderListener
{
public:
    //==============================================================================
    ModulationDestination (std::string name);
    ~ModulationDestination();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    class HoverSlider : public Slider {
    public:
        HoverSlider(ModulationDestination* owner) : owner_(owner) { }

        void mouseUp(const MouseEvent& e) {
            owner_->removeModulationSlider(this);
        }

    private:
        ModulationDestination* owner_;
    };

    void removeModulationSlider(HoverSlider* slider);
    bool addConnection(mopo::ModulationConnection* connection, juce::Colour source_color);
    void clearConnections();
    void connectToSource(std::string source_name, Colour source_color);
    void itemDropped(const SourceDetails &drag_source) override;
    bool isInterestedInDragSource(const SourceDetails &drag_source) override;
    int getNextAvailableIndex();
    int getHoverIndex(Point<int> position);
    void sliderValueChanged(Slider *slider);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void mouseMove (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::vector<mopo::ModulationConnection*> connections_;
    std::vector<Colour> connection_colors_;
    std::vector<HoverSlider*> modulation_sliders_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationDestination)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B702F1CB5F8201CF__
