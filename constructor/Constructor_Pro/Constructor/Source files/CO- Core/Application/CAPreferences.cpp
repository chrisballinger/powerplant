/********************************************************************************/
/*																				*/
/*	© 2011, Aurbach & Associates, Inc.  All rights reserved.					*/
/*																				*/
/*	Redistribution and use in source and binary forms, with or without			*/
/*	modification, are permitted provided that the following condition			*/
/*	are met:																	*/
/*																				*/
/*	  ¥	Redistributions of source code must retain the above copyright			*/
/*		notice, this list of conditions and the following disclaimer.			*/
/*																				*/
/*	  ¥	Redistributions in binary form must reproduce the above copyright		*/
/*		notice, this list of conditions and the following disclaimer in the		*/
/*		documentation and/or other materials provided with the distribution.	*/
/*																				*/
/*	  ¥	Neither the name of Aurbach & Associates, Inc. nor the names of any		*/
/*		of its employees may be used to endorse or promote products derived		*/
/*		from this software without specific prior written permission.			*/
/*																				*/
/*	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS			*/
/*	ÒAS ISÓ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT			*/
/*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A		*/
/*	PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER	*/
/*	OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,	*/
/*	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,			*/
/*	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; */
/*	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,	*/
/*	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR		*/
/*	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF		*/
/*	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.									*/
/*																				*/
/********************************************************************************/
/*
 *	Module Name:	CAPreferences
 *
 *	SuperClass:		- none -
 *
 *	Notes:			This module provides a wrapped interface around the 
 *					CFPreferences API
 *
 *	Change History:
 *
 *	Who     	Date	    Description
 * ---------------------------------------------------------------------------
 *	RLA		13-Oct-2006		Original Code (UCFPreferences.cpp)
 *	RLA		23-Aug-2011		Converted for use in the Constructor environment.
 */

#include			"CAPreferences.h"
#include			<UMemoryMgr.h>



// ---------------------------------------------------------------------------
//	¥ IsDefined														  [static]
// ---------------------------------------------------------------------------
//	Determine if the specified key is defined in application preferences.

bool
CAPreferences::IsDefined (
	CFStringRef					inKey ) 
{
	bool						defined = false;
	CFPropertyListRef			ref = nil;
	
	ref = ::CFPreferencesCopyAppValue(inKey, kCFPreferencesCurrentApplication);
	if (ref != nil) {
		defined = true;
		::CFRelease(ref);
	}
	return defined;
}


// ---------------------------------------------------------------------------
//	¥ CopyValueAsPropertyList										  [static]
// ---------------------------------------------------------------------------
//	Return the value of the specified key as a property list. The caller is
//	responsible for releasing the value reference.

CFPropertyListRef
CAPreferences::CopyValueAsPropertyList (
	CFStringRef					inKey )
{
	CFPropertyListRef			ref;
	ref = ::CFPreferencesCopyAppValue(inKey, kCFPreferencesCurrentApplication);
	return ref;
}


// ---------------------------------------------------------------------------
//	¥ SetValueAsPropertyList										  [static]
// ---------------------------------------------------------------------------
//	Create or update a preference key with the specified value, specified as a
//	property list. The user must call Synchronize to make the value persistent.

void
CAPreferences::SetValueAsPropertyList (
	CFStringRef					inKey,
	CFPropertyListRef			inValue ) 
{
	::CFPreferencesSetAppValue(inKey, inValue, kCFPreferencesCurrentApplication);
}


// ---------------------------------------------------------------------------
//	¥ GetValueAsBlock												  [static]
// ---------------------------------------------------------------------------
//	Zero the output block. Read the value of the specified key. Verify that it 
//	is generalized data. If so, read data (up to the maximum specified) into
//	the output block, returning the amount read. If the value is not genealized
//	data, assume this is an error and return 0.

UInt32
CAPreferences::GetValueAsBlock (
	CFStringRef					inKey,
	void *						inBlock,
	UInt32						inMaxSize )
{
	UInt32						byteCt = 0;
	::BlockZero(inBlock, inMaxSize);
	CFPropertyListRef			ref;
	
	ref = ::CFPreferencesCopyAppValue(inKey, kCFPreferencesCurrentApplication);
	if (ref != nil) {
		if (::CFGetTypeID(ref) == ::CFDataGetTypeID()) {
			byteCt = ::CFDataGetLength((CFDataRef) ref);
			if (byteCt > inMaxSize) {
				byteCt = inMaxSize;
			}
			if (byteCt > 0) {
				::CFDataGetBytes((CFDataRef) ref, CFRangeMake(0, byteCt), (UInt8*) inBlock);
			}
		} else {
			::CFPreferencesSetAppValue(inKey, nil, kCFPreferencesCurrentApplication);
		}
		::CFRelease(ref);
	}
	
	return byteCt;
}


// ---------------------------------------------------------------------------
//	¥ SetValueAsBlock												  [static]
// ---------------------------------------------------------------------------
//	Write the contents of the specified data blcok as the value of the specified
//	key.

void
CAPreferences::SetValueAsBlock (
	CFStringRef					inKey,
	const void *				inBlock,
	UInt32						inSize )
{
	if ((inBlock != nil) && (inSize > 0)) {
		CFPropertyListRef		ref = nil;
		
		ref = ::CFDataCreate(kCFAllocatorDefault, (const UInt8*) inBlock, inSize);
		if (ref != nil) {
			::CFPreferencesSetAppValue(inKey, ref, kCFPreferencesCurrentApplication);
			::CFRelease(ref);
		} else {
			::CFPreferencesSetAppValue(inKey, nil, kCFPreferencesCurrentApplication);
		}
	}
}


// ---------------------------------------------------------------------------
//	¥ CopyValueAsHandle												  [static]
// ---------------------------------------------------------------------------
//	Read the value of the specified key. Verify that it is generalized data.
//	If so, create a new handle and copy the data into it. But if the preference
//	value is not generalized data, conclude that the preference is invalid and
//	remove it.

Handle
CAPreferences::CopyValueAsHandle (
	CFStringRef					inKey )
{
    Handle              		hdl = nil;
    CFPropertyListRef   		ref;
    Size                		byteCt;

    ref = ::CFPreferencesCopyAppValue(inKey, kCFPreferencesCurrentApplication);
    if (ref != nil) {
        if (::CFGetTypeID(ref) == ::CFDataGetTypeID()) {
            byteCt = ::CFDataGetLength((CFDataRef) ref);
            hdl = NewHandle(byteCt);
            if (hdl != nil) {
                ::CFDataGetBytes((CFDataRef) ref,CFRangeMake(0, byteCt), (UInt8*) *hdl);
			}
        } else {
            ::CFPreferencesSetAppValue(inKey, nil, kCFPreferencesCurrentApplication);
        }
        ::CFRelease(ref);
    }
    return hdl;
}


// ---------------------------------------------------------------------------
//	¥ SetValueAsHandle												  [static]
// ---------------------------------------------------------------------------
//	Write the contents of the specified handle as the value of the specified 
//	key. Pass a nil handle to remove the key.

void
CAPreferences::SetValueAsHandle (
	CFStringRef 				inKey, 
	Handle 						inValue )
{
	if (inValue != nil) {
		StHandleLocker			locker(inValue);
		Size					hdlSize = ::GetHandleSize(inValue);
		CFPropertyListRef		ref = nil;

		ref = ::CFDataCreate(kCFAllocatorDefault, (const UInt8*)(*inValue), hdlSize);
		if (ref != nil) {
			::CFPreferencesSetAppValue(inKey, ref, kCFPreferencesCurrentApplication);
			::CFRelease(ref);
		} else {
			::CFPreferencesSetAppValue(inKey, nil, kCFPreferencesCurrentApplication);
		}
	}
}


// ---------------------------------------------------------------------------
//	¥ Remove														  [static]
// ---------------------------------------------------------------------------
//	Remove the specified preference key from the preferences file

void
CAPreferences::Remove (
	CFStringRef					inKey )
{
	::CFPreferencesSetAppValue(inKey, nil, kCFPreferencesCurrentApplication);
}


// ---------------------------------------------------------------------------
//	¥ Synchronize													  [static]
// ---------------------------------------------------------------------------
//	Update the persistent storage of application preferences

bool
CAPreferences::Synchronize() 
{
	return ::CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
}
