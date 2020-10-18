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

#if defined (MAC_OS_X_VERSION_10_11) && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_11

//==============================================================================
class BluetoothMidiPairingWindowClass   : public ObjCClass<NSObject>
{
public:
    struct Callbacks
    {
        std::unique_ptr<ModalComponentManager::Callback> modalExit;
        std::function<void()> windowClosed;
    };

    BluetoothMidiPairingWindowClass()   : ObjCClass<NSObject> ("JUCEBluetoothMidiPairingWindowClass_")
    {
        addIvar<Callbacks*> ("callbacks");
        addIvar<CABTLEMIDIWindowController*> ("controller");

       #pragma clang diagnostic push
       #pragma clang diagnostic ignored "-Wundeclared-selector"
        addMethod (@selector (initWithCallbacks:),       initWithCallbacks,       "@@:^v");
        addMethod (@selector (show:),                    show,                    "v@:^v");
        addMethod (@selector (receivedWindowWillClose:), receivedWindowWillClose, "v@:^v");
       #pragma clang diagnostic pop

        addMethod (@selector (dealloc), dealloc, "v@:");

        registerClass();
    }

private:
    static CABTLEMIDIWindowController* getController (id self)
    {
        return getIvar<CABTLEMIDIWindowController*> (self, "controller");
    }

    static id initWithCallbacks (id self, SEL, Callbacks* cbs)
    {
        self = sendSuperclassMessage (self, @selector (init));

        object_setInstanceVariable (self, "callbacks", cbs);
        object_setInstanceVariable (self, "controller", [CABTLEMIDIWindowController new]);

       #pragma clang diagnostic push
       #pragma clang diagnostic ignored "-Wundeclared-selector"
        [[NSNotificationCenter defaultCenter] addObserver: self
                                                 selector: @selector (receivedWindowWillClose:)
                                                     name: @"NSWindowWillCloseNotification"
                                                   object: [getController (self) window]];
       #pragma clang diagnostic pop

        return self;
    }

    static void dealloc (id self, SEL)
    {
        [getController (self) release];

        sendSuperclassMessage (self, @selector (dealloc));
    }

    static void show (id self, SEL, Rectangle<int>* bounds)
    {
        if (bounds != nullptr)
        {
            auto nsBounds = makeNSRect (*bounds);

            auto mainScreenHeight = []
            {
                if ([[NSScreen screens] count] == 0)
                    return (CGFloat) 0.0f;

                return [[[NSScreen screens] objectAtIndex: 0] frame].size.height;
            }();

            nsBounds.origin.y = mainScreenHeight - (nsBounds.origin.y + nsBounds.size.height);

            [getController (self).window setFrame: nsBounds
                                          display: YES];
        }

        [getController (self) showWindow: nil];
    }

    static void receivedWindowWillClose (id self, SEL, NSNotification*)
    {
        [[NSNotificationCenter defaultCenter] removeObserver: self];

        auto* cbs = getIvar<Callbacks*> (self, "callbacks");

        if (cbs->modalExit != nullptr)
            cbs->modalExit->modalStateFinished (0);

        cbs->windowClosed();
    }
};

class BluetoothMidiSelectorWindowHelper   : public DeletedAtShutdown
{
public:
    BluetoothMidiSelectorWindowHelper (ModalComponentManager::Callback* exitCallback,
                                       Rectangle<int>* bounds)
    {
        std::unique_ptr<ModalComponentManager::Callback> exitCB (exitCallback);

        static BluetoothMidiPairingWindowClass cls;
        window.reset (cls.createInstance());

        WeakReference<BluetoothMidiSelectorWindowHelper> safeThis (this);

        auto deletionCB = [=]
        {
            if (auto* t = safeThis.get())
                delete t;
        };

        callbacks.reset (new BluetoothMidiPairingWindowClass::Callbacks { std::move (exitCB),
                                                                          std::move (deletionCB) });

       #pragma clang diagnostic push
       #pragma clang diagnostic ignored "-Wundeclared-selector"
        [window.get() performSelector: @selector (initWithCallbacks:)
                           withObject: (id) callbacks.get()];
        [window.get() performSelector: @selector (show:)
                           withObject: (id) bounds];
       #pragma clang diagnostic pop
    }

private:
    std::unique_ptr<NSObject, NSObjectDeleter> window;
    std::unique_ptr<BluetoothMidiPairingWindowClass::Callbacks> callbacks;

    JUCE_DECLARE_WEAK_REFERENCEABLE (BluetoothMidiSelectorWindowHelper)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BluetoothMidiSelectorWindowHelper)
};

//==============================================================================
bool BluetoothMidiDevicePairingDialogue::open (ModalComponentManager::Callback* exitCallback,
                                               Rectangle<int>* bounds)
{
    new BluetoothMidiSelectorWindowHelper (exitCallback, bounds);
    return true;
}

bool BluetoothMidiDevicePairingDialogue::isAvailable()
{
    return true;
}

#else

bool BluetoothMidiDevicePairingDialogue::open (ModalComponentManager::Callback* exitCallback,
                                               Rectangle<int>*)
{
    std::unique_ptr<ModalComponentManager::Callback> cb (exitCallback);
    // This functionality is unavailable when targetting OSX < 10.11. Instead,
    // you should pair Bluetooth MIDI devices using the "Audio MIDI Setup" app
    // (located in /Applications/Utilities).
    jassertfalse;
    return false;
}

bool BluetoothMidiDevicePairingDialogue::isAvailable()
{
    return false;
}

#endif

} // namespace juce
