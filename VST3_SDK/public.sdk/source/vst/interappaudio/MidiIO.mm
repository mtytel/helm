//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/MidiIO.mm
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

#import "MidiIO.h"
#import <CoreMIDI/MIDINetworkSession.h>

namespace Steinberg {
namespace Vst {
namespace InterAppAudio {

//-----------------------------------------------------------------------------
MidiIO& MidiIO::instance ()
{
	static MidiIO gInstance;
	return gInstance;
}

//-----------------------------------------------------------------------------
MidiIO::MidiIO ()
: client (0)
, inputPort (0)
, destPort (0)
{
}

//-----------------------------------------------------------------------------
MidiIO::~MidiIO ()
{
	setEnabled (false);
}

//-----------------------------------------------------------------------------
void MidiIO::addProcessor (IMidiProcessor* processor)
{
	midiProcessors.push_back (processor);
}

//-----------------------------------------------------------------------------
void MidiIO::removeProcessor (IMidiProcessor* processor)
{
	auto it = std::find (midiProcessors.begin (), midiProcessors.end (), processor);
	if (it != midiProcessors.end ())
	{
		midiProcessors.erase (it);
	}
}

//-----------------------------------------------------------------------------
bool MidiIO::isEnabled () const
{
	return client != 0;
}

//-----------------------------------------------------------------------------
bool MidiIO::setEnabled (bool state)
{
	if (state)
	{
		if (client)
			return true;
		
		OSStatus err;
		NSString* name = [[NSBundle mainBundle] bundleIdentifier];
		if (err = MIDIClientCreate ((__bridge CFStringRef)name, notifyProc, this, &client) != noErr)
			return false;
		
		if (err = MIDIInputPortCreate (client, CFSTR("Input"), readProc, this, &inputPort) != noErr)
		{
			MIDIClientDispose (client);
			client = 0;
			return false;
		}
		name = [[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleDisplayName"];
		if (err = MIDIDestinationCreate (client, (__bridge CFStringRef)name, readProc, this, &destPort) != noErr)
		{
			MIDIPortDispose (inputPort);
			inputPort = 0;
			MIDIClientDispose (client);
			client = 0;
			return false;
		}
	}
	else
	{
		if (client == 0)
			return true;
		disconnectSources ();
		MIDIEndpointDispose (destPort);
		destPort = 0;
		MIDIPortDispose (inputPort);
		inputPort = 0;
		MIDIClientDispose (client);
		client = 0;
	}
	return true;
}

//-----------------------------------------------------------------------------
void MidiIO::setMidiNetworkEnabled (bool state)
{
	if (inputPort && isMidiNetworkEnabled() != state)
	{
		if (!state)
		{
			MIDIPortDisconnectSource (inputPort, [MIDINetworkSession defaultSession].sourceEndpoint);
		}
		[MIDINetworkSession defaultSession].enabled = state;
		if (state)
		{
			MIDIPortConnectSource (inputPort, [MIDINetworkSession defaultSession].sourceEndpoint, 0);
		}
	}
}

//-----------------------------------------------------------------------------
bool MidiIO::isMidiNetworkEnabled () const
{
	return [MIDINetworkSession defaultSession].isEnabled;
}

//-----------------------------------------------------------------------------
void MidiIO::setMidiNetworkPolicy (MIDINetworkConnectionPolicy policy)
{
	[MIDINetworkSession defaultSession].connectionPolicy = policy;
}

//-----------------------------------------------------------------------------
MIDINetworkConnectionPolicy MidiIO::getMidiNetworkPolicy () const
{
	return [MIDINetworkSession defaultSession].connectionPolicy;
}

//-----------------------------------------------------------------------------
void MidiIO::onInput (const MIDIPacketList *pktlist)
{
	const MIDIPacket* packet = &pktlist->packet[0];
	for (UInt32 i = 0; i < pktlist->numPackets; i++)
	{
		for (auto processor : midiProcessors)
		{
			processor->onMIDIEvent (packet->data[0], packet->data[1], packet->data[2], 0, false);
		}
		packet = MIDIPacketNext (packet);
	}
}

//-----------------------------------------------------------------------------
void MidiIO::onSourceAdded (MIDIObjectRef source)
{
	connectedSources.push_back ((MIDIEndpointRef)source);
	MIDIPortConnectSource (inputPort, (MIDIEndpointRef)source, NULL);
}

//-----------------------------------------------------------------------------
void MidiIO::disconnectSources ()
{
	for (auto source : connectedSources)
		MIDIPortDisconnectSource (inputPort, source);
	connectedSources.clear ();
}

//-----------------------------------------------------------------------------
void MidiIO::onSetupChanged ()
{
	disconnectSources ();
	ItemCount numSources = MIDIGetNumberOfSources ();
	for (ItemCount i = 0; i < numSources; i++)
	{
		onSourceAdded (MIDIGetSource (i));
	}
}

//-----------------------------------------------------------------------------
void MidiIO::readProc (const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon)
{
	MidiIO* io = static_cast<MidiIO*>(readProcRefCon);
	io->onInput (pktlist);
}

//-----------------------------------------------------------------------------
void MidiIO::notifyProc (const MIDINotification *message, void *refCon)
{
	if (message->messageID == kMIDIMsgSetupChanged)
	{
		MidiIO* mio = (MidiIO*)refCon;
		mio->onSetupChanged ();
	}
}

}}} // namespaces
