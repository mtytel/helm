//-----------------------------------------------------------------------------
// Project     : SDK Core
// Version     : 1.0
//
// Category    : SDK Core Interfaces
// Filename    : pluginterfaces/test/itest.h
// Created by  : Steinberg, 01/2005
// Description : Test Interface - Availability Depends on HOST  
//
//-----------------------------------------------------------------------------
// This file is part of a Steinberg SDK. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this distribution
// and at www.steinberg.net/sdklicenses. 
// No part of the SDK, including this file, may be copied, modified, propagated,
// or distributed except according to the terms contained in the LICENSE file.
//-----------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"

#ifndef kTestClass
#define kTestClass "Test Class" ///< A class for automated tests
#endif

namespace Steinberg {

class ITestResult;

//------------------------------------------------------------------------
// ITest interface declaration
//------------------------------------------------------------------------
class ITest : public FUnknown
{
public:
	//--- ---------------------------------------------------------------------
	/** called immediately before the test is actually run. 
		Usually this will be used to setup the test environment.
		\return true upon success	*/
	virtual bool PLUGIN_API setup () = 0;

	/** execute the test. 
		\param testResult : points to a test result where the test can
							(optionally) add an error message. 
		\return true upon success
		\sa ITestResult */
	virtual bool PLUGIN_API run (ITestResult* testResult) = 0;

	/** called after the test has run. This method shall be used to
		deconstruct a test environment that has been setup with ITest::setup (). 
	\return true upon success		*/
	virtual bool PLUGIN_API teardown () = 0;

	/** This function is used to provide information about the performed
		testcase. What is done, what is validated and what has to be prepared
		before executing the test (in case of half-automated tests).
	\return null terminated string upon success, zero otherwise		*/
	virtual const tchar* PLUGIN_API getDescription () {return 0;}
	//--- ---------------------------------------------------------------------
	static const FUID iid;
};

#ifdef UNICODE
DECLARE_CLASS_IID (ITest, 0xFE64FC19, 0x95684F53, 0xAAA78DC8, 0x7228338E)
#else
DECLARE_CLASS_IID (ITest, 0x9E2E608B, 0x64C64CF8, 0x839059BD, 0xA194032D)
#endif

//------------------------------------------------------------------------
// ITestResult interface declaration
//------------------------------------------------------------------------
/** Test Result message logger
[host imp]
when a test is called, a pointer to an ITestResult is passed in, so the 
test class can output error messages
*/
//------------------------------------------------------------------------
class ITestResult : public FUnknown
{
public:
	//--- ---------------------------------------------------------------------
	/** add an error message */
	virtual void PLUGIN_API addErrorMessage (const tchar* msg) = 0;
	virtual void PLUGIN_API addMessage (const tchar* msg) = 0;
	//--- ---------------------------------------------------------------------
	static const FUID iid;
};

#ifdef UNICODE
DECLARE_CLASS_IID (ITestResult, 0x69796279, 0xF651418B, 0xB24D79B7, 0xD7C527F4)
#else
DECLARE_CLASS_IID (ITestResult, 0xCE13B461, 0x5334451D, 0xB3943E99, 0x7446885B)
#endif

//------------------------------------------------------------------------
// ITestSuite interface declaration
//------------------------------------------------------------------------
/** A collection of tests supporting a hierarchical ordering 
[host imp]
[create via hostclasses]*/
//------------------------------------------------------------------------
class ITestSuite : public FUnknown
{
public:
	//--- ---------------------------------------------------------------------
	/** append a new test */
	virtual tresult PLUGIN_API addTest (FIDString name, ITest* test) = 0;

	/** append an entire test suite as a child suite */
	virtual tresult PLUGIN_API addTestSuite (FIDString name, ITestSuite* testSuite) = 0;

	virtual tresult PLUGIN_API setEnvironment (ITest* environment) = 0;

	//--- ---------------------------------------------------------------------
	static const FUID iid;
};

#ifdef UNICODE
DECLARE_CLASS_IID (ITestSuite, 0x5CA7106F, 0x98784AA5, 0xB4D30D71, 0x2F5F1498)
#else
DECLARE_CLASS_IID (ITestSuite, 0x81724C94, 0xE9F64F65, 0xACB104E9, 0xCC702253)
#endif

//------------------------------------------------------------------------
//  ITestFactory interface declaration
//------------------------------------------------------------------------
/**	Class factory that any testable module defines for creating tests
that will be executed from the host
[plug imp] \n
*/
//------------------------------------------------------------------------
class ITestFactory : public FUnknown
{
public:
	//--- ---------------------------------------------------------------------
	/** create the tests that this module provides.
		\param parentSuite : the test suite that the newly created tests
							shall register with. */
	virtual tresult PLUGIN_API createTests (FUnknown* context, ITestSuite* parentSuite) = 0;
	//--- ---------------------------------------------------------------------
	static const FUID iid;
};

#ifdef UNICODE
DECLARE_CLASS_IID (ITestFactory, 0xAB483D3A, 0x15264650, 0xBF86EEF6, 0x9A327A93)
#else
DECLARE_CLASS_IID (ITestFactory, 0xE77EA913, 0x58AA4838, 0x986A4620, 0x53579080)
#endif

//------------------------------------------------------------------------
} // namespace Steinberg
