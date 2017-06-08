//-----------------------------------------------------------------------------
// Project     : SDK Core
//
// Category    : SDK GUI Interfaces
// Filename    : pluginterfaces/gui/iplugview.h
// Created by  : Steinberg, 12/2007
// Description : Plug-in User Interface
//
//-----------------------------------------------------------------------------
// This file is part of a Steinberg SDK. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this distribution
// and at www.steinberg.net/sdklicenses. 
// No part of the SDK, including this file, may be copied, modified, propagated,
// or distributed except according to the terms contained in the LICENSE file.
//-----------------------------------------------------------------------------

#pragma once

#include "../base/funknown.h"

namespace Steinberg {

class IPlugFrame;

//------------------------------------------------------------------------
/*! \defgroup pluginGUI Graphical User Interface
*/

//------------------------------------------------------------------------
/**  Graphical rectangle structure. Used with IPlugView.
\ingroup pluginGUI
*/
//------------------------------------------------------------------------
struct ViewRect
{
	ViewRect (int32 l = 0, int32 t = 0, int32 r = 0, int32 b = 0)
	: left (l), top (t), right (r), bottom (b)
	{
	}

	int32 left;
	int32 top;
	int32 right;
	int32 bottom;

	//--- ---------------------------------------------------------------------
	int32 getWidth () const { return right - left; }
	int32 getHeight () const { return bottom - top; }
};

//------------------------------------------------------------------------
/**  \defgroup platformUIType Platform UI Types
\ingroup pluginGUI
List of Platform UI types for IPlugView. This list is used to match the GUI-System between
the host and a Plug-in in case that an OS provides multiple GUI-APIs.
*/
/*@{*/
/** The parent parameter in IPlugView::attached() is a HWND handle. You should attach a child window
 * to it. */
const FIDString kPlatformTypeHWND = "HWND"; ///< HWND handle. (Microsoft Windows)

/** The parent parameter in IPlugView::attached() is a WindowRef. You should attach a HIViewRef to
 * the content view of the window. */
const FIDString kPlatformTypeHIView = "HIView"; ///< HIViewRef. (Mac OS X)

/** The parent parameter in IPlugView::attached() is a NSView pointer. You should attach a NSView to it. */
const FIDString kPlatformTypeNSView = "NSView"; ///< NSView pointer. (Mac OS X)

/** The parent parameter in IPlugView::attached() is a UIView pointer. You should attach an UIView to it. */
const FIDString kPlatformTypeUIView = "UIView";		///< UIView pointer. (iOS)

/** The parent parameter in IPlugView::attached() is a X11 Window supporting XEmbed. You should attach
 * a Window to it. */
const FIDString kPlatformTypeX11EmbedWindowID = "X11EmbedWindowID"; ///< X11 Window ID. (X11)

/*@}*/
//------------------------------------------------------------------------

//------------------------------------------------------------------------
/**  Plug-in definition of a view.
\ingroup pluginGUI
- [plug imp]

\par Sizing of a view
Usually the size of a Plug-in view is fixed. But both the host and the Plug-in can cause
a view to be resized:
\n
- <b> Host </b> : If IPlugView::canResize () returns kResultTrue the host will setup the window
  so that the user can resize it. While the user resizes the window IPlugView::checkSizeConstraint
()
  is called, allowing the Plug-in to change the size to a valid rect. The host then resizes the
window to this rect and has to call IPlugView::onSize ().
\n
\n
- <b> Plug-in </b> : The Plug-in can call IPlugFrame::resizeView () and cause the host to resize the
window.
  Afterwards the host has to call IPlugView::onSize () if a resize is needed (size was changed).
  Note that if the host calls IPlugView::getSize () before calling IPlugView::onSize () (if needed),
  it will get the current (old) size not the wanted one!!
  Here the calling sequence:
    * plug-in->host: IPlugFrame::resizeView (newSize)
    * host->plug-in (optional): IPlugView::getSize () returns the currentSize (not the wanted
newSize)!
    * host->plug-in: if newSize is different than its actual current size: IPlugView::onSize
(newSize)
    * host->plug-in (optional): IPlugView::getSize () returns the newSize
\n
Please only resize the platform representation of the view when IPlugView::onSize () is called.

\par Keyboard handling
The Plug-in view receives keyboard events from the host. A view implementation must not handle
keyboard events by the means of platform callbacks, but let the host pass them to the view. The host
depends on a proper return value when IPlugView::onKeyDown is called, otherwise the Plug-in view may
cause a malfunction of the host's key command handling!

\see IPlugFrame, \ref platformUIType
*/
//------------------------------------------------------------------------
class IPlugView: public FUnknown
{
public:
//------------------------------------------------------------------------
	/** Is Platform UI Type supported
	    \param type : IDString of \ref platformUIType */
	virtual tresult PLUGIN_API isPlatformTypeSupported (FIDString type) = 0;

	/** The parent window of the view has been created, the (platform) representation of the view
	   should now be created as well.
	    Note that the parent is owned by the caller and you are not allowed to alter it in any way
	   other than adding your own views.
	    Note that in this call the Plug-in could call a IPlugFrame::resizeView ()!
	    \param parent : platform handle of the parent window or view
	    \param type : \ref platformUIType which should be created */
	virtual tresult PLUGIN_API attached (void* parent, FIDString type) = 0;

	/** The parent window of the view is about to be destroyed.
	    You have to remove all your own views from the parent window or view. */
	virtual tresult PLUGIN_API removed () = 0;

	/** Handling of mouse wheel. */
	virtual tresult PLUGIN_API onWheel (float distance) = 0;

	/** Handling of keyboard events : Key Down.
	    \param key : unicode code of key
	    \param keyCode : virtual keycode for non ascii keys - see \ref VirtualKeyCodes in keycodes.h
	    \param modifiers : any combination of modifiers - see \ref KeyModifier in keycodes.h
	    \return kResultTrue if the key is handled, otherwise kResultFalse. \n
	            <b> Please note that kResultTrue must only be returned if the key has really been
	   handled. </b> Otherwise key command handling of the host might be blocked! */
	virtual tresult PLUGIN_API onKeyDown (char16 key, int16 keyCode, int16 modifiers) = 0;

	/** Handling of keyboard events : Key Up.
	    \param key : unicode code of key
	    \param keyCode : virtual keycode for non ascii keys - see \ref VirtualKeyCodes in keycodes.h
	    \param modifiers : any combination of KeyModifier - see \ref KeyModifier in keycodes.h
	    \return kResultTrue if the key is handled, otherwise return kResultFalse. */
	virtual tresult PLUGIN_API onKeyUp (char16 key, int16 keyCode, int16 modifiers) = 0;

	/** Returns the size of the platform representation of the view. */
	virtual tresult PLUGIN_API getSize (ViewRect* size) = 0;

	/** Resizes the platform representation of the view to the given rect. Note that if the Plug-in
	 * requests a resize (IPlugFrame::resizeView ()) onSize has to be called afterward. */
	virtual tresult PLUGIN_API onSize (ViewRect* newSize) = 0;

	/** Focus changed message. */
	virtual tresult PLUGIN_API onFocus (TBool state) = 0;

	/** Sets IPlugFrame object to allow the Plug-in to inform the host about resizing. */
	virtual tresult PLUGIN_API setFrame (IPlugFrame* frame) = 0;

	/** Is view sizable by user. */
	virtual tresult PLUGIN_API canResize () = 0;

	/** On live resize this is called to check if the view can be resized to the given rect, if not
	 * adjust the rect to the allowed size. */
	virtual tresult PLUGIN_API checkSizeConstraint (ViewRect* rect) = 0;
//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IPlugView, 0x5BC32507, 0xD06049EA, 0xA6151B52, 0x2B755B29)

//------------------------------------------------------------------------
/** Callback interface passed to IPlugView.
\ingroup pluginGUI
- [host imp]

Enables a Plug-in to resize the view and cause the host to resize the window.
*/
//------------------------------------------------------------------------
class IPlugFrame : public FUnknown
{
public:
//------------------------------------------------------------------------
	/** Called to inform the host about the resize of a given view.
	    Afterwards the host has to call IPlugView::onSize (). */
	virtual tresult PLUGIN_API resizeView (IPlugView* view, ViewRect* newSize) = 0;
//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IPlugFrame, 0x367FAF01, 0xAFA94693, 0x8D4DA2A0, 0xED0882A3)

//------------------------------------------------------------------------
class IPlugViewIdleHandler : public FUnknown
{
public:
	virtual void PLUGIN_API onPlugViewIdle () = 0;

//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IPlugViewIdleHandler, 0x35BD6C3A, 0x9C1F4073, 0x89C7DA88, 0xBF6595D3)

//------------------------------------------------------------------------
/** Extension to IPlugFrame
\ingroup pluginGUI
- [host impl]

Enables a Plug-in to add an idle handler to the main event loop (Linux only)
*/
//------------------------------------------------------------------------
class IPlugFrameIdle : public FUnknown
{
public:
	virtual tresult PLUGIN_API addIdleHandler (IPlugViewIdleHandler* handler) = 0;
	virtual tresult PLUGIN_API removeIdleHandler (IPlugViewIdleHandler* handler) = 0;

//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IPlugFrameIdle, 0x31DE26C9, 0x36654639, 0x9F858E64, 0xE211817A)

//------------------------------------------------------------------------
} // namespace Steinberg
