//------------------------------------------------------------------------
// Project     : SDK Base
// Version     : 1.0
//
// Category    : Helpers
// Filename    : base/source/fcleanup.h
// Created by  : Steinberg, 2008
// Description : Template classes for automatic resource cleanup
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

#include <stdlib.h>

namespace Steinberg {

/** Template definition for classes that help guarding against memory leaks.
A stack allocated object of this type automatically deletes an at construction time passed
dynamically allocated single object when it reaches the end of its scope. \n\n
Intended usage:
\code
    {
        int* pointerToInt = new int;
        Steinberg::FDeleter<int> deleter (pointerToInt);

        // Do something with the variable behind pointerToInt.

    } // No memory leak here, destructor of deleter cleans up the integer.
\endcode
*/
//------------------------------------------------------------------------
template <class T>
struct FDeleter
{
	/// Constructor. _toDelete is a pointer to the dynamically allocated object that is to be
	/// deleted when this FDeleter object's destructor is executed.
	FDeleter (T* _toDelete) : toDelete (_toDelete) {}
	/// Destructor. Calls delete on the at construction time passed pointer.
	~FDeleter ()
	{
		if (toDelete)
			delete toDelete;
	}
	T* toDelete; ///< Remembers the object that is to be deleted during destruction.
};

/** Template definition for classes that help guarding against memory leaks.
A stack allocated object of this type automatically deletes an at construction time passed
dynamically allocated array of objects when it reaches the end of its scope. \n\n
Intended usage:
\code
    {
        int* pointerToIntArray = new int[10];
        Steinberg::FArrayDeleter<int> deleter (pointerToIntArray);

        // Do something with the array behind pointerToIntArray.

    } // No memory leak here, destructor of deleter cleans up the integer array.
\endcode
*/
//------------------------------------------------------------------------
template <class T>
struct FArrayDeleter
{
	/// Constructor. _arrayToDelete is a pointer to the dynamically allocated array of objects that
	/// is to be deleted when this FArrayDeleter object's destructor is executed.
	FArrayDeleter (T* _arrayToDelete) : arrayToDelete (_arrayToDelete) {}

	/// Destructor. Calls delete[] on the at construction time passed pointer.
	~FArrayDeleter ()
	{
		if (arrayToDelete)
			delete[] arrayToDelete;
	}

	T* arrayToDelete; ///< Remembers the array of objects that is to be deleted during destruction.
};

/** Template definition for classes that help guarding against dangling pointers.
A stack allocated object of this type automatically resets an at construction time passed
pointer to null when it reaches the end of its scope. \n\n
Intended usage:
\code
    int* pointerToInt = 0;
    {
        int i = 1;
        pointerToInt = &i;
        Steinberg::FPtrNuller<int> ptrNuller (pointerToInt);

        // Do something with pointerToInt.

    } // No dangling pointer here, pointerToInt is reset to 0 by destructor of ptrNuller.
\endcode
*/
//------------------------------------------------------------------------
template <class T>
struct FPtrNuller
{
	/// Constructor. _toNull is a reference to the pointer that is to be reset to NULL when this
	/// FPtrNuller object's destructor is executed.
	FPtrNuller (T*& _toNull) : toNull (_toNull) {}
	/// Destructor. Calls delete[] on the at construction time passed pointer.
	~FPtrNuller () { toNull = 0; }

	T*& toNull; ///< Remembers the pointer that is to be set to NULL during destruction.
};

/** Template definition for classes that help resetting an object's value.
A stack allocated object of this type automatically resets the value of an at construction time
passed object to null when it reaches the end of its scope. \n\n Intended usage: \code int theObject
= 0;
    {
        Steinberg::FNuller<int> theNuller (theObject);

        theObject = 1;

    } // Here the destructor of theNuller resets the value of theObject to 0.
\endcode
*/
//------------------------------------------------------------------------
template <class T>
struct FNuller
{
	/// Constructor. _toNull is a reference to the object that is to be assigned 0 when this FNuller
	/// object's destructor is executed.
	FNuller (T& _toNull) : toNull (_toNull) {}
	/// Destructor. Assigns 0 to the at construction time passed object reference.
	~FNuller () { toNull = 0; }

	T& toNull; ///< Remembers the object that is to be assigned 0 during destruction.
};

/** Class definition for objects that help resetting boolean variables.
A stack allocated object of this type automatically sets an at construction time passed
boolean variable immediately to TRUE and resets the same variable to FALSE when it
reaches the end of its own scope. \n\n
Intended usage:
\code
    bool theBoolean = false;
    {
        Steinberg::FBoolSetter theBoolSetter (theBoolean);
        // Here the constructor of theBoolSetter sets theBoolean to TRUE.

        // Do something.

    } // Here the destructor of theBoolSetter resets theBoolean to FALSE.
\endcode
*/
//------------------------------------------------------------------------
template <class T>
struct FBooleanSetter
{
	/// Constructor. _toSet is a reference to the boolean that is set to TRUE immediately in this
	/// constructor call and gets reset to FALSE when this FBoolSetter object's destructor is
	/// executed.
	FBooleanSetter (T& _toSet) : toSet (_toSet) { toSet = true; }
	/// Destructor. Resets the at construction time passed boolean to FALSE.
	~FBooleanSetter () { toSet = false; }

	T& toSet; ///< Remembers the boolean that is to be reset during destruction.
};

typedef FBooleanSetter<bool> FBoolSetter;

/** Class definition for objects that help setting boolean variables.
A stack allocated object of this type automatically sets an at construction time passed
boolean variable to TRUE if the given condition is met. At the end of its own scope the
stack object will reset the same boolean variable to FALSE, if it wasn't set so already. \n\n
Intended usage:
\code
    bool theBoolean = false;
    {
        bool creativityFirst = true;
        Steinberg::FConditionalBoolSetter theCBSetter (theBoolean, creativityFirst);
        // Here the constructor of theCBSetter sets theBoolean to the value of creativityFirst.

        // Do something.

    } // Here the destructor of theCBSetter resets theBoolean to FALSE.
\endcode
*/
//------------------------------------------------------------------------
struct FConditionalBoolSetter
{
	/// Constructor. _toSet is a reference to the boolean that is to be set. If the in the second
	/// parameter given condition is TRUE then also _toSet is set to TRUE immediately.
	FConditionalBoolSetter (bool& _toSet, bool condition) : toSet (_toSet)
	{
		if (condition)
			toSet = true;
	}
	/// Destructor. Resets the at construction time passed boolean to FALSE.
	~FConditionalBoolSetter () { toSet = false; }

	bool& toSet; ///< Remembers the boolean that is to be reset during destruction.
};

/** Template definition for classes that help closing resources.
A stack allocated object of this type automatically calls the close method of an at
construction time passed object when it reaches the end of its scope.
It goes without saying that the given type needs to have a close method. \n\n
Intended usage:
\code
    struct CloseableObject
    {
        void close() {};
    };

    {
        CloseableObject theObject;
        Steinberg::FCloser<CloseableObject> theCloser (&theObject);

        // Do something.

    } // Here the destructor of theCloser calls the close method of theObject.
\endcode
*/
template <class T>
struct FCloser
{
	/// Constructor. _obj is the pointer on which close is to be called when this FCloser object's
	/// destructor is executed.
	FCloser (T* _obj) : obj (_obj) {}
	/// Destructor. Calls the close function on the at construction time passed pointer.
	~FCloser ()
	{
		if (obj)
			obj->close ();
	}

	T* obj; ///< Remembers the pointer on which close is to be called during destruction.
};

/** Class definition for objects that help guarding against memory leaks.
A stack allocated object of this type automatically frees the "malloced" memory behind an at
construction time passed pointer when it reaches the end of its scope.
*/
//------------------------------------------------------------------------
/*! \class FMallocReleaser
 */
//------------------------------------------------------------------------
class FMallocReleaser
{
public:
	/// Constructor. _data is the pointer to the memory on which free is to be called when this
	/// FMallocReleaser object's destructor is executed.
	FMallocReleaser (void* _data) : data (_data) {}
	/// Destructor. Calls the free function on the at construction time passed pointer.
	~FMallocReleaser ()
	{
		if (data)
			free (data);
	}
//------------------------------------------------------------------------
protected:
	void* data; ///< Remembers the pointer on which free is to be called during destruction.
};

//------------------------------------------------------------------------
} // namespace Steinberg


#if MAC
typedef const void* CFTypeRef;
extern "C" {
	extern void CFRelease (CFTypeRef cf);
}

namespace Steinberg {

/** Class definition for objects that helps releasing CoreFoundation objects.
A stack allocated object of this type automatically releases an at construction time
passed CoreFoundation object when it reaches the end of its scope.

Only available on Macintosh platform.
*/
//------------------------------------------------------------------------
/*! \class CFReleaser
*/
//------------------------------------------------------------------------
class CFReleaser
{
public:
	/// Constructor. _obj is the reference to an CoreFoundation object which is to be released when this CFReleaser object's destructor is executed. 
	CFReleaser (CFTypeRef _obj) : obj (_obj) {}
	/// Destructor. Releases the at construction time passed object.
	~CFReleaser () { if (obj) CFRelease (obj); }
protected:
	CFTypeRef obj; ///< Remembers the object which is to be released during destruction.
};

//------------------------------------------------------------------------
} // namespace Steinberg

#endif // MAC
