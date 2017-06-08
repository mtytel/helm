//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstnoteexpressiontypes.h
// Created by  : Steinberg, 12/2010
// Description : VST Note Expression Type Info Implementation
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

#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "base/source/fobject.h"

#include <map>
#include <vector>

namespace Steinberg {
namespace Vst {
class Parameter;

//------------------------------------------------------------------------
/** Note expression type object.
\ingroup vstClasses */
//-----------------------------------------------------------------------------
class NoteExpressionType : public FObject
{
public:
	NoteExpressionType ();
	NoteExpressionType (const NoteExpressionTypeInfo& info);
	NoteExpressionType (NoteExpressionTypeID typeId, const TChar* title, const TChar* shortTitle, const TChar* units,
						int32 unitId, NoteExpressionValue defaultValue, NoteExpressionValue minimum, NoteExpressionValue maximum,
						int32 stepCount, int32 flags = 0, int32 precision = 4);
	NoteExpressionType (NoteExpressionTypeID typeId, const TChar* title, const TChar* shortTitle, const TChar* units,
						int32 unitId, Parameter* associatedParameter, int32 flags = 0);

	/** get the underlying NoteExpressionTypeInfo struct */
	NoteExpressionTypeInfo& getInfo () { return info; }

	/** convert a note expression value to a readable string */
	virtual tresult getStringByValue (NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/);
	/** convert a readable string to a note expression value */
	virtual tresult getValueByString (const TChar* string /*in*/, NoteExpressionValue& valueNormalized /*out*/);

	/** gets the current precision (used for string representation of float) */
	int32 getPrecision () const { return precision;}
	/** Sets the precision for string representation of float value (for example 4.34 with 2 as precision) */
	void setPrecision (int32 val) { precision = val;}
//-----------------------------------------------------------------------------
	OBJ_METHODS(NoteExpressionType, FObject)
protected:
	NoteExpressionTypeInfo info;
	IPtr<Parameter> associatedParameter;
	int32 precision;
};

//------------------------------------------------------------------------
/** Note expression type object representing a custom range.
\ingroup vstClasses */
//-----------------------------------------------------------------------------
class RangeNoteExpressionType : public NoteExpressionType
{
public:
	RangeNoteExpressionType (NoteExpressionTypeID typeId, const TChar* title, const TChar* shortTitle, const TChar* units,
							 int32 unitId, NoteExpressionValue defaultPlainValue, NoteExpressionValue plainMin, NoteExpressionValue plainMax,
							 int32 flags = 0, int32 precision = 4);

	/** Gets the minimum plain value */
	virtual ParamValue getMin () const {return plainMin;}
	/** Sets the minimum plain value */
	virtual void setMin (ParamValue value) {plainMin = value;}
	/** Gets the maximum plain value */
	virtual ParamValue getMax () const {return plainMax;}
	/** Sets the maximum plain value */
	virtual void setMax (ParamValue value) {plainMax = value;}

	virtual tresult getStringByValue (NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/) SMTG_OVERRIDE;
	virtual tresult getValueByString (const TChar* string /*in*/, NoteExpressionValue& valueNormalized /*out*/) SMTG_OVERRIDE;
//-----------------------------------------------------------------------------
	OBJ_METHODS(RangeNoteExpressionType, NoteExpressionType)
protected:
	NoteExpressionValue plainMin;
	NoteExpressionValue plainMax;
};

//------------------------------------------------------------------------
/** Collection of note expression types.
\ingroup vstClasses */
//------------------------------------------------------------------------
class NoteExpressionTypeContainer : public FObject
{
public:
	/** default constructor */
	NoteExpressionTypeContainer ();

	/** add a note expression type. The container owns the type. No need to release it afterwards. */
	bool addNoteExpressionType (NoteExpressionType* noteExpType);
	/** remove a note expression type */
	bool removeNoteExpressionType (NoteExpressionTypeID typeId);
	/** remove all note expression types */
	void removeAll ();

	/** get a note expression type object by ID */
	NoteExpressionType* getNoteExpressionType (NoteExpressionTypeID typeId);

	/** get the number of note expression types */
	int32 getNoteExpressionCount ();

	/** get note expression info */
	tresult getNoteExpressionInfo (int32 noteExpressionIndex, NoteExpressionTypeInfo& info /*out*/);
	/** convert a note expression value to a readable string */
	tresult getNoteExpressionStringByValue (NoteExpressionTypeID id, NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/);
	/** convert a string to a note expression value */
	tresult getNoteExpressionValueByString (NoteExpressionTypeID id, const TChar* string /*in*/, NoteExpressionValue& valueNormalized /*out*/);
//-----------------------------------------------------------------------------
	OBJ_METHODS(NoteExpressionTypeContainer, FObject)
protected:
	typedef std::vector<IPtr<NoteExpressionType> > NoteExprTypeVector;
	NoteExprTypeVector::const_iterator find (NoteExpressionTypeID typeId) const;

	NoteExprTypeVector noteExps;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
