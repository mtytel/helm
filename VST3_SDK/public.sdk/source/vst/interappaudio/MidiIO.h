//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/MidiIO.h
// Created by  : Steinberg, 09/2013.
// Description : VST 3 InterAppAudio
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2017, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

#include <CoreMIDI/CoreMIDI.h>
#include "AudioIO.h"
#include <vector>

namespace Steinberg {
namespace Vst {
namespace InterAppAudio {

//-----------------------------------------------------------------------------
class MidiIO
{
public:
	static MidiIO& instance ();
	
	bool setEnabled (bool state);
	bool isEnabled () const;

	// MIDI Network is experimental, don't use yet
	void setMidiNetworkEnabled (bool state);
	bool isMidiNetworkEnabled () const;
	void setMidiNetworkPolicy (MIDINetworkConnectionPolicy policy);
	MIDINetworkConnectionPolicy getMidiNetworkPolicy () const;

	void addProcessor (IMidiProcessor* processor);
	void removeProcessor (IMidiProcessor* processor);

//-----------------------------------------------------------------------------
private:
	MidiIO ();
	~MidiIO ();

	void onInput (const MIDIPacketList *pktlist);
	void onSourceAdded (MIDIObjectRef source);
	void onSetupChanged ();
	void disconnectSources ();
	
	MIDIClientRef client;
	MIDIPortRef inputPort;
	MIDIEndpointRef destPort;

	typedef std::vector<IMidiProcessor*> MidiProcessors;
	MidiProcessors midiProcessors;

	typedef std::vector<MIDIEndpointRef> ConnectionList;
	ConnectionList connectedSources;

	static void readProc (const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon);
	static void notifyProc (const MIDINotification *message, void *refCon);
};

//------------------------------------------------------------------------
} // namespace InterAppAudio
} // namespace Vst
} // namespace Steinberg
