//-----------------------------------------------------------------------------
// Project     : SDK Core
//
// Category    : SDK Core Interfaces
// Filename    : pluginterfaces/base/fplatform.h
// Created by  : Steinberg, 01/2004
// Description : Detect platform and set define
//
//-----------------------------------------------------------------------------
// This file is part of a Steinberg SDK. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this distribution
// and at www.steinberg.net/sdklicenses. 
// No part of the SDK, including this file, may be copied, modified, propagated,
// or distributed except according to the terms contained in the LICENSE file.
//-----------------------------------------------------------------------------

#pragma once

#define kLittleEndian 0
#define kBigEndian 1

#undef PLUGIN_API

#undef WINDOWS
#undef MAC

//-----------------------------------------------------------------------------
// WIN32 AND WIN64
#if defined (_WIN32)
	#define WINDOWS 1
	#define BYTEORDER kLittleEndian
	#define COM_COMPATIBLE 1
	#define PLUGIN_API __stdcall

	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif

	#pragma warning (disable : 4244) // Conversion from 'type1' to 'type2', possible loss of data.
	#pragma warning (disable : 4250) // Inheritance via dominance is allowed
	#pragma warning (disable : 4996) // deprecated functions

	#pragma warning (3 : 4189) // local variable is initialized but not referenced
	#pragma warning (3 : 4238) // nonstandard extension used : class rvalue used as lvalue

	#if defined (_WIN64)       // WIN64 only
		#define PLATFORM_64 1
	#endif
	#ifndef WIN32
		#define WIN32	1
	#endif

	#ifdef __cplusplus
		#define SMTG_CPP11	__cplusplus >= 201103L || _MSC_VER > 1600 || __INTEL_CXX11_MODE__
		#define SMTG_CPP11_STDLIBSUPPORT SMTG_CPP11
	#endif		
//-----------------------------------------------------------------------------
// LINUX
#elif __gnu_linux__
	#define LINUX 1
	#include <endian.h>
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define BYTEORDER kLittleEndian
	#else
		#define BYTEORDER kBigEndian
	#endif
	#define COM_COMPATIBLE 0
	#define PLUGIN_API
	#define PTHREADS 1
	#if __LP64__
		#define PLATFORM_64 1
	#endif
	#ifdef __cplusplus
		#include <cstddef>
		#define SMTG_CPP11 (__cplusplus >= 201103L)
		#ifndef SMTG_CPP11
			#error unsupported compiler
		#endif
		#define SMTG_CPP11_STDLIBSUPPORT 1
	#endif
//-----------------------------------------------------------------------------
// Mac and iOS
#elif __APPLE__
	#include <TargetConditionals.h>
	#define MAC 1
	#define PTHREADS 1
	#if !TARGET_OS_IPHONE
		#ifndef __CF_USE_FRAMEWORK_INCLUDES__
		#define __CF_USE_FRAMEWORK_INCLUDES__
		#endif
		#ifndef TARGET_API_MAC_CARBON
		#define TARGET_API_MAC_CARBON 1
		#endif
	#endif
	#if __LP64__
		#define PLATFORM_64 1
	#endif
	#if defined (__BIG_ENDIAN__)
		#define BYTEORDER kBigEndian
	#else
		#define BYTEORDER kLittleEndian
	#endif
	#define COM_COMPATIBLE 0
	#define PLUGIN_API

	#if !defined(__PLIST__) && !defined(SMTG_DISABLE_DEFAULT_DIAGNOSTICS)
		#ifdef __clang__
			#pragma GCC diagnostic ignored "-Wswitch-enum"
			#pragma GCC diagnostic ignored "-Wparentheses"
			#pragma GCC diagnostic ignored "-Wuninitialized"
			#if __clang_major__ >= 3
				#pragma GCC diagnostic ignored "-Wtautological-compare"
				#pragma GCC diagnostic ignored "-Wunused-value"
				#if __clang_major__ >= 4 || __clang_minor__ >= 1
					#pragma GCC diagnostic ignored "-Wswitch"
					#pragma GCC diagnostic ignored "-Wcomment"
				#endif
				#if __clang_major__ >= 5
					#pragma GCC diagnostic ignored "-Wunsequenced"
					#if __clang_minor__ >= 1
						#pragma GCC diagnostic ignored "-Wunused-const-variable"
					#endif
				#endif
			#endif
		#endif
	#endif
	#ifdef __cplusplus
		#include <cstddef>
		#define SMTG_CPP11 (__cplusplus >= 201103L || __INTEL_CXX11_MODE__)
		#if defined (_LIBCPP_VERSION) && SMTG_CPP11
		#define SMTG_CPP11_STDLIBSUPPORT 1
		#else
		#define SMTG_CPP11_STDLIBSUPPORT 0
		#endif
	#endif
#else
	#pragma error unknown platform

#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#if SMTG_CPP11
#define SMTG_OVERRIDE override
#else
#define SMTG_OVERRIDE
#endif
