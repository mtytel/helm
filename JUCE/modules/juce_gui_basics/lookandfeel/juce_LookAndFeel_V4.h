/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

//==============================================================================
/**
    The latest JUCE look-and-feel style, as introduced in 2017.
    @see LookAndFeel, LookAndFeel_V1, LookAndFeel_V2, LookAndFeel_V3

    @tags{GUI}
*/
class JUCE_API  LookAndFeel_V4   : public LookAndFeel_V3
{
public:
    /**
         A struct containing the set of colours to apply to the GUI
    */
    class ColourScheme
    {
    public:
        /** The standard set of colours to use. */
        enum UIColour
        {
            windowBackground = 0,
            widgetBackground,
            menuBackground,
            outline,
            defaultText,
            defaultFill,
            highlightedText,
            highlightedFill,
            menuText,

            numColours
        };

        template <typename... ItemColours>
        ColourScheme (ItemColours... coloursToUse)
        {
            static_assert (sizeof... (coloursToUse) == numColours, "Must supply one colour for each UIColour item");
            const Colour c[] = { Colour (coloursToUse)... };

            for (int i = 0; i < numColours; ++i)
                palette[i] = c[i];
        }

        ColourScheme (const ColourScheme&) = default;
        ColourScheme& operator= (const ColourScheme&) = default;

        /** Returns a colour from the scheme */
        Colour getUIColour (UIColour colourToGet) const noexcept;

        /** Sets a scheme colour. */
        void setUIColour (UIColour colourToSet, Colour newColour) noexcept;

        /** Returns true if two ColourPalette objects contain the same colours. */
        bool operator== (const ColourScheme&) const noexcept;
        /** Returns false if two ColourPalette objects contain the same colours. */
        bool operator!= (const ColourScheme&) const noexcept;

    private:
        Colour palette[numColours];
    };

    //==============================================================================
    /** Creates a LookAndFeel_V4 object with a default colour scheme. */
    LookAndFeel_V4();

    /** Creates a LookAndFeel_V4 object with a given colour scheme. */
    LookAndFeel_V4 (ColourScheme);

    /** Destructor. */
    ~LookAndFeel_V4() override;

    //==============================================================================
    void setColourScheme (ColourScheme);
    ColourScheme& getCurrentColourScheme() noexcept       { return currentColourScheme; }

    static ColourScheme getDarkColourScheme();
    static ColourScheme getMidnightColourScheme();
    static ColourScheme getGreyColourScheme();
    static ColourScheme getLightColourScheme();

    //==============================================================================
    Button* createDocumentWindowButton (int) override;
    void positionDocumentWindowButtons (DocumentWindow&, int, int, int, int, Button*, Button*, Button*, bool) override;
    void drawDocumentWindowTitleBar (DocumentWindow&, Graphics&, int, int, int, int, const Image*, bool) override;

    //==============================================================================
    Font getTextButtonFont (TextButton&, int buttonHeight) override;

    void drawButtonBackground (Graphics&, Button&, const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawToggleButton (Graphics&, ToggleButton&,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void drawTickBox (Graphics&, Component&,
                      float x, float y, float w, float h,
                      bool ticked, bool isEnabled,
                      bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void changeToggleButtonWidthToFitText (ToggleButton&) override;

    //==============================================================================
    AlertWindow* createAlertWindow (const String& title, const String& message,
                                    const String& button1,
                                    const String& button2,
                                    const String& button3,
                                    AlertWindow::AlertIconType iconType,
                                    int numButtons, Component* associatedComponent) override;
    void drawAlertBox (Graphics&, AlertWindow&, const Rectangle<int>& textArea, TextLayout&) override;

    int getAlertWindowButtonHeight() override;
    Font getAlertWindowTitleFont() override;
    Font getAlertWindowMessageFont() override;
    Font getAlertWindowFont() override;

    //==============================================================================
    void drawProgressBar (Graphics&, ProgressBar&, int width, int height, double progress, const String& textToShow) override;
    bool isProgressBarOpaque (ProgressBar&) override    { return false; }

    //==============================================================================
    int getDefaultScrollbarWidth() override;
    void drawScrollbar (Graphics&, ScrollBar&, int x, int y, int width, int height, bool isScrollbarVertical,
                        int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;

    //==============================================================================
    Path getTickShape (float height) override;
    Path getCrossShape (float height) override;

    //==============================================================================
    void fillTextEditorBackground (Graphics&, int width, int height, TextEditor&) override;
    void drawTextEditorOutline (Graphics&, int width, int height, TextEditor&) override;

    //==============================================================================
    Button* createFileBrowserGoUpButton() override;

    void layoutFileBrowserComponent (FileBrowserComponent&,
                                     DirectoryContentsDisplayComponent*,
                                     FilePreviewComponent*,
                                     ComboBox* currentPathBox,
                                     TextEditor* filenameBox,
                                     Button* goUpButton) override;

    void drawFileBrowserRow (Graphics&, int width, int height,
                             const File& file, const String& filename, Image* icon,
                             const String& fileSizeDescription, const String& fileTimeDescription,
                             bool isDirectory, bool isItemSelected, int itemIndex,
                             DirectoryContentsDisplayComponent&) override;

    //==============================================================================
    void drawPopupMenuItem (Graphics&, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override;

    void getIdealPopupMenuItemSize (const String& text, bool isSeparator, int standardMenuItemHeight,
                                    int& idealWidth, int& idealHeight) override;

    void drawMenuBarBackground (Graphics&, int width, int height, bool isMouseOverBar, MenuBarComponent&) override;

    void drawMenuBarItem (Graphics&, int width, int height,
                          int itemIndex, const String& itemText,
                          bool isMouseOverItem, bool isMenuOpen, bool isMouseOverBar,
                          MenuBarComponent&) override;

    //==============================================================================
    void drawComboBox (Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox&) override;
    Font getComboBoxFont (ComboBox&) override;
    void positionComboBoxText (ComboBox&, Label&) override;

    //==============================================================================
    int getSliderThumbRadius (Slider&) override;

    void drawLinearSlider (Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle, Slider&) override;

    void drawRotarySlider (Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider&) override;

    void drawPointer (Graphics&, float x, float y, float diameter,
                      const Colour&, int direction) noexcept;

    Label* createSliderTextBox (Slider&) override;

    //==============================================================================
    void drawTooltip (Graphics&, const String& text, int width, int height) override;

    //==============================================================================
    void drawConcertinaPanelHeader (Graphics&, const Rectangle<int>& area,
                                    bool isMouseOver, bool isMouseDown,
                                    ConcertinaPanel&, Component& panel) override;

    //==============================================================================
    void drawLevelMeter (Graphics&, int, int, float) override;

    //==============================================================================
    void paintToolbarBackground (Graphics&, int width, int height, Toolbar&) override;

    void paintToolbarButtonLabel (Graphics&, int x, int y, int width, int height,
                                  const String& text, ToolbarItemComponent&) override;

    //==============================================================================
    void drawPropertyPanelSectionHeader (Graphics&, const String& name, bool isOpen, int width, int height) override;
    void drawPropertyComponentBackground (Graphics&, int width, int height, PropertyComponent&) override;
    void drawPropertyComponentLabel (Graphics&, int width, int height, PropertyComponent&) override;
    Rectangle<int> getPropertyComponentContentPosition (PropertyComponent&) override;

    //==============================================================================
    void drawCallOutBoxBackground (CallOutBox&, Graphics&, const Path&, Image&) override;

    //==============================================================================
    void drawStretchableLayoutResizerBar (Graphics&, int, int, bool, bool, bool) override;

private:
    //==============================================================================
    void drawLinearProgressBar (Graphics&, ProgressBar&, int width, int height, double progress, const String&);
    void drawCircularProgressBar (Graphics&, ProgressBar&, const String&);

    int getPropertyComponentIndent (PropertyComponent&);

    //==============================================================================
    void initialiseColours();
    ColourScheme currentColourScheme;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel_V4)
};

} // namespace juce
