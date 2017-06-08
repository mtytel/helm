//------------------------------------------------------------------------
// Project     : SDK Base
// Version     : 1.0
// 
// Category    : Helpers
// Filename    : base/source/timer.cpp
// Created by  : Steinberg, 05/2006
// Description : Timer class for receiving triggers at regular intervals
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

#include "base/source/timer.h"

namespace Steinberg {
static bool timersEnabled = true;

//------------------------------------------------------------------------
DisableDispatchingTimers::DisableDispatchingTimers ()
{
	oldState = timersEnabled;
	timersEnabled = false;
}

//------------------------------------------------------------------------
DisableDispatchingTimers::~DisableDispatchingTimers ()
{
	timersEnabled = oldState;
}

//------------------------------------------------------------------------
namespace SystemTime {

//------------------------------------------------------------------------
struct ZeroStartTicks
{
	static const uint64 startTicks;

	static int32 getTicks32 ()
	{
		return static_cast<int32> (SystemTime::getTicks64 () - startTicks);
	}
};
const uint64 ZeroStartTicks::startTicks = SystemTime::getTicks64 ();

//------------------------------------------------------------------------
int32 getTicks ()
{
	return ZeroStartTicks::getTicks32 ();
}

//------------------------------------------------------------------------
} // namespace SystemTime
} // namespace Steinberg


#if MAC
#include <CoreFoundation/CoreFoundation.h>
#include <mach/mach_time.h>

#ifdef verify
#undef verify
#endif

namespace Steinberg {

namespace SystemTime {
	struct MachTimeBase {
	private:
		struct mach_timebase_info timebaseInfo;

		MachTimeBase ()
		{
			mach_timebase_info (&timebaseInfo);
		}

		static const MachTimeBase& instance ()
		{
			static MachTimeBase gInstance;
			return gInstance;
		}
	public:
		static double getTimeNanos ()
		{
			const MachTimeBase& timeBase = instance ();
			double absTime = static_cast<double> (mach_absolute_time ());
			double d = (absTime / timeBase.timebaseInfo.denom) * timeBase.timebaseInfo.numer;	// nano seconds
			return d;
		}
	};

	//------------------------------------------------------------------------
	uint64 getTicks64 ()
	{
		return static_cast<uint64> (MachTimeBase::getTimeNanos () / 1000000.);
	}
} // namespace SystemTime

//------------------------------------------------------------------------
class MacPlatformTimer : public Timer
{
public:
	MacPlatformTimer (ITimerCallback* callback, uint32 milliseconds);
	~MacPlatformTimer ();

	void stop ();
	bool verify () const { return platformTimer != 0; }

	static void timerCallback (CFRunLoopTimerRef timer, void *info);
protected:
	CFRunLoopTimerRef platformTimer;
	ITimerCallback* callback;
};

//------------------------------------------------------------------------
MacPlatformTimer::MacPlatformTimer (ITimerCallback* callback, uint32 milliseconds)
: platformTimer (0)
, callback (callback)
{
	if (callback)
	{
		CFRunLoopTimerContext timerContext = {0};
		timerContext.info = this;
		platformTimer = CFRunLoopTimerCreate (kCFAllocatorDefault, CFAbsoluteTimeGetCurrent () + milliseconds * 0.001, milliseconds * 0.001f, 0, 0, timerCallback, &timerContext);
		if (platformTimer)
			CFRunLoopAddTimer (CFRunLoopGetMain (), platformTimer, kCFRunLoopCommonModes);
	}
}

//------------------------------------------------------------------------
MacPlatformTimer::~MacPlatformTimer ()
{
	stop ();
}

//------------------------------------------------------------------------
void MacPlatformTimer::stop ()
{
	if (platformTimer)
	{
		CFRunLoopRemoveTimer (CFRunLoopGetMain (), platformTimer, kCFRunLoopCommonModes);
		CFRelease (platformTimer);
		platformTimer = 0;
	}
}

//------------------------------------------------------------------------
void MacPlatformTimer::timerCallback (CFRunLoopTimerRef , void *info)
{
	if (timersEnabled)
	{
		MacPlatformTimer* timer = (MacPlatformTimer*)info;
		if (timer)
		{
			timer->callback->onTimer (timer);
		}
	}
}

//------------------------------------------------------------------------
Timer* Timer::create (ITimerCallback* callback, uint32 milliseconds)
{
	MacPlatformTimer* timer = NEW MacPlatformTimer (callback, milliseconds);
	if (timer->verify ())
		return timer;
	timer->release ();
	return 0;
}

}

#elif WINDOWS

#include <windows.h>
#include <list>
#include <algorithm>

namespace Steinberg {
namespace SystemTime {
/*
    @return the current system time in milliseconds
*/
uint64 getTicks64 ()
{
	return GetTickCount64 ();
}
}

class WinPlatformTimer;
typedef std::list<WinPlatformTimer*> WinPlatformTimerList;

//------------------------------------------------------------------------
// WinPlatformTimer
//------------------------------------------------------------------------
class WinPlatformTimer : public Timer
{
public:
//------------------------------------------------------------------------
	WinPlatformTimer (ITimerCallback* callback, uint32 milliseconds);
	~WinPlatformTimer ();

	void stop ();
	bool verify () const {return id != 0;}

//------------------------------------------------------------------------
private:
	UINT_PTR id;
	ITimerCallback* callback;

	static void addTimer (WinPlatformTimer* t);
	static void removeTimer (WinPlatformTimer* t);

	static void CALLBACK TimerProc (HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static WinPlatformTimerList* timers;
};

//------------------------------------------------------------------------
WinPlatformTimerList* WinPlatformTimer::timers = 0;

//------------------------------------------------------------------------
WinPlatformTimer::WinPlatformTimer (ITimerCallback* callback, uint32 milliseconds)
: callback (callback)
{
	id = SetTimer (0, 0, milliseconds, TimerProc);
	if (id)
		addTimer (this);
}

//------------------------------------------------------------------------
WinPlatformTimer::~WinPlatformTimer ()
{
	stop ();
}

//------------------------------------------------------------------------
void WinPlatformTimer::addTimer (WinPlatformTimer* t)
{
	if (timers == 0)
		timers = NEW WinPlatformTimerList;
	timers->push_back (t);
}

//------------------------------------------------------------------------
void WinPlatformTimer::removeTimer (WinPlatformTimer* t)
{
	if (!timers)
		return;

	WinPlatformTimerList::iterator it = std::find (timers->begin (), timers->end (), t);
	if (it != timers->end ())
		timers->erase (it);
	if (timers->empty ())
	{
		delete timers;
		timers = 0;
	}
}

//------------------------------------------------------------------------
void WinPlatformTimer::stop ()
{
	if (!id)
		return;

	KillTimer (0, id);
	removeTimer (this);
	id = 0;
}

//------------------------------------------------------------------------
void CALLBACK WinPlatformTimer::TimerProc (HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (timersEnabled && timers)
	{
		WinPlatformTimerList::const_iterator it = timers->cbegin ();
		while (it != timers->cend ())
		{	
			WinPlatformTimer* timer = *it;
			if (timer->id == idEvent)
			{
				if (timer->callback)
					timer->callback->onTimer (timer);
				return;
			}
		}
	}
}

//------------------------------------------------------------------------
Timer* Timer::create (ITimerCallback* callback, uint32 milliseconds)
{
	WinPlatformTimer* platformTimer = NEW WinPlatformTimer (callback, milliseconds);
	if (platformTimer->verify ())
		return platformTimer;
	platformTimer->release ();
	return 0;
}

//------------------------------------------------------------------------
} // namespace Steinberg

#elif LINUX
#warning DEPRECATED No Linux implementation
#endif
