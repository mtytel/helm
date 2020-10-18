//-----------------------------------------------------------------------------
// Project     : SDK Core
//
// Category    : SDK Core Interfaces
// Filename    : pluginterfaces/base/fstrdefs.h
// Created by  : Steinberg, 01/2004
// Description : Definitions for handling strings (Unicode / ASCII / Platforms)
//
//-----------------------------------------------------------------------------
// This file is part of a Steinberg SDK. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this distribution
// and at www.steinberg.net/sdklicenses. 
// No part of the SDK, including this file, may be copied, modified, propagated,
// or distributed except according to the terms contained in the LICENSE file.
//-----------------------------------------------------------------------------

#pragma once

#include "ftypes.h"

//----------------------------------------------------------------------------
// string methods defines unicode / ASCII
//----------------------------------------------------------------------------

// 16 bit string operations
#if SMTG_CPP11	// if c++11 unicode string literals
	#define SMTG_CPP11_CAT_PRIVATE_DONT_USE(a,b)			a ## b
	#if SMTG_OS_WINDOWS
		#define STR16(x) SMTG_CPP11_CAT_PRIVATE_DONT_USE(L,x)
	#else
		#define STR16(x) SMTG_CPP11_CAT_PRIVATE_DONT_USE(u,x)
	#endif
#else
	#include "conststringtable.h"
	#define STR16(x) Steinberg::ConstStringTable::instance ()->getString (x)
#endif

#ifdef UNICODE
	#define STR(x) STR16(x)
	#define tStrBufferSize(buffer) (sizeof(buffer)/sizeof(Steinberg::tchar))

#else
	#define STR(x) x
	#define tStrBufferSize(buffer) (sizeof(buffer))
#endif

#define str8BufferSize(buffer) (sizeof(buffer)/sizeof(Steinberg::char8))
#define str16BufferSize(buffer) (sizeof(buffer)/sizeof(Steinberg::char16))

#if SMTG_OS_WINDOWS
#define FORMAT_INT64A "I64d"
#define FORMAT_UINT64A "I64u"

#elif SMTG_OS_MACOS || SMTG_OS_LINUX
#define FORMAT_INT64A  "lld"
#define FORMAT_UINT64A "llu"
#define stricmp		strcasecmp
#define strnicmp	strncasecmp
#endif

#ifdef UNICODE
#define FORMAT_INT64W STR(FORMAT_INT64A)
#define FORMAT_UINT64W STR(FORMAT_UINT64A)

#define FORMAT_INT64 FORMAT_INT64W
#define FORMAT_UINT64 FORMAT_UINT64W
#else
#define FORMAT_INT64 FORMAT_INT64A
#define FORMAT_UINT64 FORMAT_UINT64A
#endif


//----------------------------------------------------------------------------
// newline
//----------------------------------------------------------------------------
#if SMTG_OS_WINDOWS
#define ENDLINE_A   "\r\n"
#define ENDLINE_W   STR ("\r\n")
#elif SMTG_OS_MACOS
#define ENDLINE_A   "\r"
#define ENDLINE_W   STR ("\r")
#elif SMTG_OS_LINUX
#define ENDLINE_A   "\n"
#define ENDLINE_W   STR ("\n")
#endif

#ifdef UNICODE
#define ENDLINE ENDLINE_W
#else
#define ENDLINE ENDLINE_A
#endif

#if SMTG_OS_WINDOWS && !defined(__GNUC__) && defined(_MSC_VER) && (_MSC_VER < 1900)
#define stricmp _stricmp
#define strnicmp _strnicmp
#define snprintf _snprintf
#endif

namespace Steinberg {

//----------------------------------------------------------------------------
static const tchar kEmptyString[] = { 0 };
static const char8 kEmptyString8[] = { 0 };
static const char16 kEmptyString16[] = { 0 };

#ifdef UNICODE
static const tchar kInfiniteSymbol[] = { 0x221E, 0 };
#else
static const tchar* const kInfiniteSymbol = STR ("oo");
#endif

//----------------------------------------------------------------------------
template <class T>
inline int32 _tstrlen (const T* wcs)
{
	const T* eos = wcs;

	while (*eos++)
		;

	return (int32) (eos - wcs - 1);
}

inline int32 tstrlen (const tchar* str) {return _tstrlen (str);}
inline int32 strlen8 (const char8* str) {return _tstrlen (str);}
inline int32 strlen16 (const char16* str) {return _tstrlen (str);}

//----------------------------------------------------------------------------
template <class T>
inline int32 _tstrcmp (const T* src, const T* dst)
{
	while (*src == *dst && *dst)
	{
		src++;
		dst++;
	}

	if (*src == 0 && *dst == 0)
		return 0;
	else if (*src == 0)
		return -1;
	else if (*dst == 0)
		return 1;
	else
		return (int32) (*src - *dst);
}

inline int32 tstrcmp (const tchar* src, const tchar* dst) {return _tstrcmp (src, dst);}
inline int32 strcmp8 (const char8* src, const char8* dst) {return _tstrcmp (src, dst);}
inline int32 strcmp16 (const char16* src, const char16* dst) {return _tstrcmp (src, dst);}

template <typename T>
inline int32 strcmpT (const T* first, const T* last);

template <>
inline int32 strcmpT<char8> (const char8* first, const char8* last) { return _tstrcmp (first, last); }

template <>
inline int32 strcmpT<char16> (const char16* first, const char16* last) { return _tstrcmp (first, last); }

//----------------------------------------------------------------------------
template <class T>
inline int32 _tstrncmp (const T* first, const T* last, uint32 count)
{
	if (count == 0)
		return 0;

	while (--count && *first && *first == *last)
	{
		first++;
		last++;
	}

	if (*first == 0 && *last == 0)
		return 0;
	else if (*first == 0)
		return -1;
	else if (*last == 0)
		return 1;
	else
		return (int32) (*first - *last);
}

inline int32 tstrncmp (const tchar* first, const tchar* last, uint32 count) {return _tstrncmp (first, last, count);}
inline int32 strncmp8 (const char8* first, const char8* last, uint32 count) {return _tstrncmp (first, last, count);}
inline int32 strncmp16 (const char16* first, const char16* last, uint32 count) {return _tstrncmp (first, last, count);}

template <typename T>
inline int32 strncmpT (const T* first, const T* last, uint32 count);

template <>
inline int32 strncmpT<char8> (const char8* first, const char8* last, uint32 count) { return _tstrncmp (first, last, count); }

template <>
inline int32 strncmpT<char16> (const char16* first, const char16* last, uint32 count) {return _tstrncmp (first, last, count); }

//----------------------------------------------------------------------------
template <class T>
inline T* _tstrcpy (T* dst, const T* src)
{
	T* cp = dst;
	while ((*cp++ = *src++) != 0) // copy string
		;
	return dst;
}
inline tchar* tstrcpy (tchar* dst, const tchar* src) {return _tstrcpy (dst, src);}
inline char8* strcpy8 (char8* dst, const char8* src) {return _tstrcpy (dst, src);}
inline char16* strcpy16 (char16* dst, const char16* src) {return _tstrcpy (dst, src);}

//----------------------------------------------------------------------------
template <class T>
inline T* _tstrncpy (T* dest, const T* source, uint32 count)
{
	T* start = dest;
	while (count && (*dest++ = *source++) != 0) // copy string
		count--;

	if (count) // pad out with zeros
	{
		while (--count)
			*dest++ = 0;
	}
	return start;
}

inline tchar* tstrncpy (tchar* dest, const tchar* source, uint32 count) {return _tstrncpy (dest, source, count);}
inline char8* strncpy8 (char8* dest, const char8* source, uint32 count) {return _tstrncpy (dest, source, count);}
inline char16* strncpy16 (char16* dest, const char16* source, uint32 count) {return _tstrncpy (dest, source, count);}

//----------------------------------------------------------------------------
template <class T>
inline T* _tstrcat (T* dst, const T* src)
{
	T* cp = dst;

	while (*cp)
		cp++; // find end of dst

	while ((*cp++ = *src++) != 0) // Copy src to end of dst
		;

	return dst;
}

inline tchar* tstrcat (tchar* dst, const tchar* src) {return _tstrcat (dst, src); }
inline char8* strcat8 (char8* dst, const char8* src) {return _tstrcat (dst, src); }
inline char16* strcat16 (char16* dst, const char16* src) {return _tstrcat (dst, src); }

//----------------------------------------------------------------------------
inline void str8ToStr16 (char16* dst, const char8* src, int32 n = -1)
{
	int32 i = 0;
	for (;;)
	{
		if (i == n)
		{
			dst[i] = 0;
			return;
		}

#if BYTEORDER == kBigEndian
		char8* pChr = (char8*)&dst[i];
		pChr[0] = 0;
		pChr[1] = src[i];
#else
		dst[i] = static_cast<char16> (src[i]);
#endif

		if (src[i] == 0)
			break;

		i++;
	}

	while (n > i)
	{
		dst[i] = 0;
		i++;
	}
}

//------------------------------------------------------------------------
inline bool FIDStringsEqual (FIDString id1, FIDString id2)
{
	return (id1 && id2) ? (strcmp8 (id1, id2) == 0) : false;
}

static const uint32 kPrintfBufferSize = 4096;

//------------------------------------------------------------------------
} // namespace Steinberg
