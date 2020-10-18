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

struct DeviceInfo
{
    // VS2015 requires a constructor to avoid aggregate initialization
    DeviceInfo (Block::UID buid, BlocksProtocol::TopologyIndex tidx,
                BlocksProtocol::BlockSerialNumber s, BlocksProtocol::VersionNumber v,
                BlocksProtocol::BlockName n, BlocksProtocol::BatteryLevel level,
                BlocksProtocol::BatteryCharging charging, Block::UID master)
        : uid (buid), index (tidx), serial (s), version (v), name (n),
          batteryLevel (level), batteryCharging (charging), masterUid (master),
          isMaster (uid == master)
    {
    }

    Block::UID uid {};
    BlocksProtocol::TopologyIndex index;
    BlocksProtocol::BlockSerialNumber serial;
    BlocksProtocol::VersionNumber version;
    BlocksProtocol::BlockName name;
    BlocksProtocol::BatteryLevel batteryLevel;
    BlocksProtocol::BatteryCharging batteryCharging;
    Block::UID masterUid;
    bool isMaster {};
};

} // namespace juce
