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
    A box with a small arrow that can be used as a temporary pop-up window to show
    extra controls when a button or other component is clicked.

    Using one of these is similar to having a popup menu attached to a button or
    other component - but it looks fancier, and has an arrow that can indicate the
    object that it applies to.

    The class works best when shown modally, but obviously running modal loops is
    evil and must never be done, so the launchAsynchronously method is provided as
    a handy way of launching an instance of a CallOutBox and automatically managing
    its lifetime, e.g.

    @code
    void mouseUp (const MouseEvent&)
    {
        FoobarContentComp* content = new FoobarContentComp();
        content->setSize (300, 300);

        CallOutBox& myBox
            = CallOutBox::launchAsynchronously (content, getScreenBounds(), nullptr);
    }
    @endcode

    The call-out will resize and position itself when the content changes size.

    @tags{GUI}
*/
class JUCE_API  CallOutBox    : public Component,
                                private Timer
{
public:
    //==============================================================================
    /** Creates a CallOutBox.

        @param contentComponent     the component to display inside the call-out. This should
                                    already have a size set (although the call-out will also
                                    update itself when the component's size is changed later).
                                    Obviously this component must not be deleted until the
                                    call-out box has been deleted.
        @param areaToPointTo        the area that the call-out's arrow should point towards. If
                                    a parentComponent is supplied, then this is relative to that
                                    parent; otherwise, it's a global screen coord.
        @param parentComponent      if not a nullptr, this is the component to add the call-out to.
                                    If this is a nullptr, the call-out will be added to the desktop.
    */
    CallOutBox (Component& contentComponent,
                Rectangle<int> areaToPointTo,
                Component* parentComponent);

    /** Destructor. */
    ~CallOutBox() override;

    //==============================================================================
    /** Changes the base width of the arrow. */
    void setArrowSize (float newSize);

    /** Updates the position and size of the box.

        You shouldn't normally need to call this, unless you need more precise control over the
        layout.

        @param newAreaToPointTo     the rectangle to make the box's arrow point to
        @param newAreaToFitIn       the area within which the box's position should be constrained
    */
    void updatePosition (const Rectangle<int>& newAreaToPointTo,
                         const Rectangle<int>& newAreaToFitIn);


    /** This will launch a callout box containing the given content, pointing to the
        specified target component.

        This method will create and display a callout, returning immediately, after which
        the box will continue to run modally until the user clicks on some other component, at
        which point it will be dismissed and deleted automatically.

        It returns a reference to the newly-created box so that you can customise it, but don't
        keep a pointer to it, as it'll be deleted at some point when it gets closed.

        @param contentComponent     the component to display inside the call-out. This should
                                    already have a size set (although the call-out will also
                                    update itself when the component's size is changed later).
                                    This component will be owned by the callout box and deleted
                                    later when the box is dismissed.
        @param areaToPointTo        the area that the call-out's arrow should point towards. If
                                    a parentComponent is supplied, then this is relative to that
                                    parent; otherwise, it's a global screen coord.
        @param parentComponent      if not a nullptr, this is the component to add the call-out to.
                                    If this is a nullptr, the call-out will be added to the desktop.
    */
    static CallOutBox& launchAsynchronously (Component* contentComponent,
                                             Rectangle<int> areaToPointTo,
                                             Component* parentComponent);

    /** Posts a message which will dismiss the callout box asynchronously.
        NB: it's safe to call this method from any thread.
    */
    void dismiss();

    /** Determines whether the mouse events for clicks outside the calloutbox are
        consumed, or allowed to arrive at the other component that they were aimed at.

        By default this is false, so that when you click on something outside the calloutbox,
        that event will also be sent to the component that was clicked on. If you set it to
        true, then the first click will always just dismiss the box and not be sent to
        anything else.
    */
    void setDismissalMouseClicksAreAlwaysConsumed (bool shouldAlwaysBeConsumed) noexcept;

    //==============================================================================
    /** This abstract base class is implemented by LookAndFeel classes. */
    struct JUCE_API  LookAndFeelMethods
    {
        virtual ~LookAndFeelMethods() = default;

        virtual void drawCallOutBoxBackground (CallOutBox&, Graphics&, const Path&, Image&) = 0;
        virtual int getCallOutBoxBorderSize (const CallOutBox&) = 0;
        virtual float getCallOutBoxCornerSize (const CallOutBox&) = 0;
    };

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void resized() override;
    /** @internal */
    void moved() override;
    /** @internal */
    void childBoundsChanged (Component*) override;
    /** @internal */
    bool hitTest (int x, int y) override;
    /** @internal */
    void inputAttemptWhenModal() override;
    /** @internal */
    bool keyPressed (const KeyPress&) override;
    /** @internal */
    void handleCommandMessage (int) override;
    /** @internal */
    int getBorderSize() const noexcept;

private:
    //==============================================================================
    Component& content;
    Path outline;
    Point<float> targetPoint;
    Rectangle<int> availableArea, targetArea;
    Image background;
    float arrowSize = 16.0f;
    bool dismissalMouseClicksAreAlwaysConsumed = false;

    Time creationTime;

    void refreshPath();
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CallOutBox)
};

} // namespace juce
