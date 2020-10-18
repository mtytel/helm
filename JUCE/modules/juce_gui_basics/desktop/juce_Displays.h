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
    Manages details about connected display devices.

    @tags{GUI}
*/
class JUCE_API  Displays
{
private:
    Displays (Desktop&);

public:
    /** Represents a connected display device. */
    struct Display
    {
        /** This will be true if this is the user's main display device. */
        bool isMain;

        /** The total area of this display in logical pixels including any OS-dependent objects
            like the taskbar, menu bar, etc. */
        Rectangle<int> totalArea;

        /** The total area of this display in logical pixels which isn't covered by OS-dependent
            objects like the taskbar, menu bar, etc.
        */
        Rectangle<int> userArea;

        /** The top-left of this display in physical coordinates. */
        Point<int> topLeftPhysical;

        /** The scale factor of this display.

            For higher-resolution displays, or displays with a user-defined scale factor set,
            this may be a value other than 1.0.

            This value is used to convert between physical and logical pixels. For example, a Component
            with size 10x10 will use 20x20 physical pixels on a display with a scale factor of 2.0.
        */
        double scale;

        /** The DPI of the display.

            This is the number of physical pixels per inch. To get the number of logical
            pixels per inch, divide this by the Display::scale value.
        */
        double dpi;
    };

    /** Converts a Rectangle from physical to logical pixels.

        If useScaleFactorOfDisplay is not null then its scale factor will be used for the conversion
        regardless of the display that the Rectangle to be converted is on.
    */
    Rectangle<int> physicalToLogical (Rectangle<int>, const Display* useScaleFactorOfDisplay = nullptr) const noexcept;

    /** Converts a Rectangle from logical to physical pixels.

        If useScaleFactorOfDisplay is not null then its scale factor will be used for the conversion
        regardless of the display that the Rectangle to be converted is on.
    */
    Rectangle<int> logicalToPhysical (Rectangle<int>, const Display* useScaleFactorOfDisplay = nullptr) const noexcept;

    /** Converts a Point from physical to logical pixels. */
    template <typename ValueType>
    Point<ValueType> physicalToLogical (Point<ValueType>, const Display* useScaleFactorOfDisplay = nullptr) const noexcept;

    /** Converts a Point from logical to physical pixels. */
    template <typename ValueType>
    Point<ValueType> logicalToPhysical (Point<ValueType>, const Display* useScaleFactorOfDisplay = nullptr) const noexcept;

    /** Returns the Display object representing the display containing a given Rectangle (either
        in logical or physical pixels).

        If the Rectangle lies outside all the displays then the nearest one will be returned.
    */
    const Display& findDisplayForRect (Rectangle<int>, bool isPhysical = false)  const noexcept;

    /** Returns the Display object representing the display containing a given Point (either
        in logical or physical pixels).

        If the Point lies outside all the displays then the nearest one will be returned.
    */
    const Display& findDisplayForPoint (Point<int>, bool isPhysical = false)  const noexcept;

    /** Returns the Display object representing the display acting as the user's main screen. */
    const Display& getMainDisplay() const noexcept;

    /** Returns a RectangleList made up of all the displays in LOGICAL pixels. */
    RectangleList<int> getRectangleList (bool userAreasOnly) const;

    /** Returns the smallest bounding box which contains all the displays in LOGICAL pixels. */
    Rectangle<int> getTotalBounds (bool userAreasOnly) const;

    /** An Array containing the Display objects for all of the connected displays. */
    Array<Display> displays;

   #ifndef DOXYGEN
    /** @internal */
    void refresh();
    /** @internal */
    ~Displays() = default;
    // This method has been deprecated - use the findDisplayForPoint() or findDisplayForRect() methods instead
    // as they can deal with converting between logical and physical pixels
    JUCE_DEPRECATED (const Display& getDisplayContaining (Point<int> position) const noexcept);
   #endif

private:
    friend class Desktop;

    void init (Desktop&);
    void findDisplays (float masterScale);

    void updateToLogical();
};

} // namespace juce
