/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

Message::Message() noexcept {}
Message::~Message() {}

void Message::messageCallback()
{
    if (auto* r = recipient.get())
        r->handleMessage (*this);
}

MessageListener::MessageListener() noexcept
{
    // Are you trying to create a messagelistener before or after juce has been initialised??
    JUCE_ASSERT_MESSAGE_MANAGER_EXISTS
}

MessageListener::~MessageListener()
{
    masterReference.clear();
}

void MessageListener::postMessage (Message* const message) const
{
    message->recipient = const_cast<MessageListener*> (this);
    message->post();
}

} // namespace juce
