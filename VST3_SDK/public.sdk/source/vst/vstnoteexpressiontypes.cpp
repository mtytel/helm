//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstnoteexpressiontypes.cpp
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

#include "vstnoteexpressiontypes.h"
#include "vstparameters.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/futils.h"
#include "base/source/fstring.h"

#include <cstring>

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
NoteExpressionType::NoteExpressionType ()
: precision (4)
{
	memset (&info, 0, sizeof (info));
}

//-----------------------------------------------------------------------------
NoteExpressionType::NoteExpressionType (const NoteExpressionTypeInfo& _info)
: precision (4)
{
	memcpy (&info, &_info, sizeof (info));
}

//-----------------------------------------------------------------------------
NoteExpressionType::NoteExpressionType (NoteExpressionTypeID _typeId, const TChar* _title, const TChar* _shortTitle, const TChar* _units,
										int32 _unitId, NoteExpressionValue _defaultValue, NoteExpressionValue _minimum, NoteExpressionValue _maximum,
										int32 _stepCount, int32 _flags, int32 _precision)
: precision (_precision)
{
	memset (&info, 0, sizeof (info));
	info.typeId = _typeId;
	if (_title)
		UString (info.title, str16BufferSize (String128)).assign (_title);
	if (_shortTitle)
		UString (info.shortTitle, str16BufferSize (String128)).assign (_shortTitle);
	if (_units)
		UString (info.shortTitle, str16BufferSize (String128)).assign (_units);
	info.unitId = _unitId;
	info.valueDesc.defaultValue = _defaultValue;
	info.valueDesc.minimum = _minimum;
	info.valueDesc.maximum = _maximum;
	info.valueDesc.stepCount = _stepCount;
	info.flags = _flags;
}

//-----------------------------------------------------------------------------
NoteExpressionType::NoteExpressionType (NoteExpressionTypeID _typeId, const TChar* _title, const TChar* _shortTitle, const TChar* _units,
										int32 _unitId, Parameter* _associatedParameter, int32 _flags)
: associatedParameter (_associatedParameter)
, precision (4)
{
	memset (&info, 0, sizeof (info));
	info.typeId = _typeId;
	if (_title)
		UString (info.title, str16BufferSize (String128)).assign (_title);
	if (_shortTitle)
		UString (info.shortTitle, str16BufferSize (String128)).assign (_shortTitle);
	if (_units)
		UString (info.shortTitle, str16BufferSize (String128)).assign (_units);
	info.unitId = _unitId;
	info.valueDesc.defaultValue = 0.5;
	info.valueDesc.minimum = 0.;
	info.valueDesc.maximum = 1.;
	info.flags = _flags;
	if (_associatedParameter)
	{
		info.valueDesc.stepCount = _associatedParameter->getInfo ().stepCount;
		info.valueDesc.defaultValue = _associatedParameter->getInfo ().defaultNormalizedValue;
		info.associatedParameterId = associatedParameter->getInfo ().id;
		info.flags |= NoteExpressionTypeInfo::kAssociatedParameterIDValid;
	}
}

//-----------------------------------------------------------------------------
tresult NoteExpressionType::getStringByValue (NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/)
{
	if (associatedParameter)
	{
		associatedParameter->toString (valueNormalized, string);
		return kResultTrue;
	}
	UString128 wrapper;
	if (info.valueDesc.stepCount > 0)
	{
		int32 value = Min<int32> (info.valueDesc.stepCount, (int32)(valueNormalized * (info.valueDesc.stepCount + 1)));
		wrapper.printInt (value);
	}
	else
	{
		wrapper.printFloat (valueNormalized, precision);
	}
	wrapper.copyTo (string, 128);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult NoteExpressionType::getValueByString (const TChar* string /*in*/, NoteExpressionValue& valueNormalized /*out*/)
{
	if (associatedParameter)
	{
		return associatedParameter->fromString (string, valueNormalized) ? kResultTrue : kResultFalse;
	}
	String wrapper (string);
	if (info.valueDesc.stepCount > 0)
	{
		int32 value;
		if (wrapper.scanInt32 (value) && value <= info.valueDesc.stepCount)
		{
			valueNormalized = (NoteExpressionValue)value / (NoteExpressionValue)info.valueDesc.stepCount;
			return kResultTrue;
		}
		return kResultFalse;
	}
	double value;
	wrapper.scanFloat (value);
	if (value < info.valueDesc.minimum)
		return kResultFalse;
	if (value > info.valueDesc.maximum)
		return kResultFalse;
	valueNormalized = value;
	return kResultTrue;
}

//-----------------------------------------------------------------------------
RangeNoteExpressionType::RangeNoteExpressionType (NoteExpressionTypeID _typeId, const TChar* _title, const TChar* _shortTitle, const TChar* _units,
						 int32 _unitId, NoteExpressionValue _defaultPlainValue, NoteExpressionValue _plainMin, NoteExpressionValue _plainMax,
						 int32 _flags, int32 _precision)
: NoteExpressionType (_typeId, _title, _shortTitle, _units, _unitId, 0, 0, 1, 0, _flags, _precision)
, plainMin (_plainMin)
, plainMax (_plainMax)
{
	info.valueDesc.defaultValue = (_defaultPlainValue - getMin ()) / (getMax () - getMin ());
}

//-----------------------------------------------------------------------------
tresult RangeNoteExpressionType::getStringByValue (NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/)
{
	NoteExpressionValue plain = valueNormalized * (getMax () - getMin ()) + getMin ();
	UString128 wrapper;
	wrapper.printFloat (plain, precision);
	wrapper.copyTo (string, 128);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult RangeNoteExpressionType::getValueByString (const TChar* string /*in*/, NoteExpressionValue& valueNormalized /*out*/)
{
	String wrapper (string);
	double value = 0;
	if (wrapper.scanFloat (value))
	{
		value = (value - getMin ()) / (getMax () - getMin ());
		if (value >= 0. && value <= 1.)
		{
			valueNormalized = value;
			return kResultTrue;
		}
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
NoteExpressionTypeContainer::NoteExpressionTypeContainer ()
{
}

//-----------------------------------------------------------------------------
NoteExpressionTypeContainer::NoteExprTypeVector::const_iterator NoteExpressionTypeContainer::find (
    NoteExpressionTypeID typeId) const
{
	for (NoteExprTypeVector::const_iterator it = noteExps.begin (), end = noteExps.end ();
	     it != end; ++it)
	{
		if ((*it)->getInfo ().typeId == typeId)
		{
			return it;
		}
	}
	return noteExps.end ();
}

//-----------------------------------------------------------------------------
bool NoteExpressionTypeContainer::addNoteExpressionType (NoteExpressionType* noteExpType)
{
	noteExps.push_back(IPtr<NoteExpressionType> (noteExpType, false));
	return true;
}

//-----------------------------------------------------------------------------
bool NoteExpressionTypeContainer::removeNoteExpressionType (NoteExpressionTypeID typeId)
{
	NoteExprTypeVector::const_iterator it = find (typeId);
	if (it != noteExps.end ())
	{
		noteExps.erase (it);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void NoteExpressionTypeContainer::removeAll ()
{
	noteExps.clear ();
}

//-----------------------------------------------------------------------------
NoteExpressionType* NoteExpressionTypeContainer::getNoteExpressionType (NoteExpressionTypeID typeId)
{
	NoteExprTypeVector::const_iterator it = find (typeId);
	if (it != noteExps.end ())
		return (*it);
	return 0;
}

//-----------------------------------------------------------------------------
int32 NoteExpressionTypeContainer::getNoteExpressionCount ()
{
	return static_cast<int32> (noteExps.size ());
}

//-----------------------------------------------------------------------------
tresult NoteExpressionTypeContainer::getNoteExpressionInfo (int32 noteExpressionIndex, NoteExpressionTypeInfo& info /*out*/)
{
	if (noteExpressionIndex < 0 || noteExpressionIndex >= static_cast<int32> (noteExps.size ()))
		return kInvalidArgument;
	std::memcpy (&info, &noteExps[noteExpressionIndex]->getInfo (), sizeof (info));
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult NoteExpressionTypeContainer::getNoteExpressionStringByValue (NoteExpressionTypeID id, NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/)
{
	NoteExpressionType* noteExpType = getNoteExpressionType (id);
	if (noteExpType)
	{
		return noteExpType->getStringByValue (valueNormalized, string);
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult NoteExpressionTypeContainer::getNoteExpressionValueByString (NoteExpressionTypeID id, const TChar* string /*in*/, NoteExpressionValue& valueNormalized /*out*/)
{
	NoteExpressionType* noteExpType = getNoteExpressionType (id);
	if (noteExpType)
	{
		return noteExpType->getValueByString (string, valueNormalized);
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
