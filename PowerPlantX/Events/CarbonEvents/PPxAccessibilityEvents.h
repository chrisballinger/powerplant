// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAccessibilityEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxAccessibilityEvents.h
	@brief		Event handlers for accessibility Carbon Events
*/

#ifndef H_PPxAccessibilityEvents
#define H_PPxAccessibilityEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Returns child object hit by a specified global mouse point */

class	AccessibleGetChildAtPointDoer : public SpecificEventDoer<
											kEventClassAccessibility,
											kEventAccessibleGetChildAtPoint> {
protected:
	virtual OSStatus	DoAccessibleGetChildAtPoint(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								const HIPoint&		inPoint,
								AXUIElementRef&		outChild) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns child which is part of the focus chain */

class	AccessibleGetFocusedChildDoer : public SpecificEventDoer<
											kEventClassAccessibility,
											kEventAccessibleGetFocusedChild> {
protected:
	virtual OSStatus	DoAccessibleGetFocusedChild(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								AXUIElementRef&		outChild) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns names of all supported attributes */

class	AccessibleGetAllAttributeNamesDoer : public SpecificEventDoer<
										kEventClassAccessibility,
										kEventAccessibleGetAllAttributeNames> {
protected:
	virtual OSStatus	DoAccessibleGetAllAttributeNames(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								CFMutableArrayRef&	ioAttrNames) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the value of an attribute */

class	AccessibleGetNamedAttributeDoer : public SpecificEventDoer<
										kEventClassAccessibility,
										kEventAccessibleGetNamedAttribute> {
protected:
	virtual OSStatus	DoAccessibleGetNamedAttribute(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								CFStringRef			inAttrName) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Sets the value of an attribute */

class	AccessibleSetNamedAttributeDoer : public SpecificEventDoer<
										kEventClassAccessibility,
										kEventAccessibleSetNamedAttribute> {
protected:
	virtual OSStatus	DoAccessibleSetNamedAttribute(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								CFStringRef			inAttrName) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns whether an attriubute is settable */

class	AccessibleIsNamedAttributeSettableDoer : public SpecificEventDoer<
									kEventClassAccessibility,
									kEventAccessibleIsNamedAttributeSettable> {
protected:
	virtual OSStatus	DoAccessibleIsNamedAttributeSettable(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								CFStringRef			inAttrName,
								bool&				outIsSettable) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns names of all supported actions */

class	AccessibleGetAllActionNamesDoer : public SpecificEventDoer<
										kEventClassAccessibility,
										kEventAccessibleGetAllActionNames> {
protected:
	virtual OSStatus	DoAccessibleGetAllActionNames(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								CFMutableArrayRef&	ioActionNames) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Performs an action */

class	AccessiblePerformNamedActionDoer : public SpecificEventDoer<
									kEventClassAccessibility,
									kEventAccessiblePerformNamedAction> {
protected:
	virtual OSStatus	DoAccessiblePerformNamedAction(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								CFStringRef			inActionName) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns a description of an action's significance */

class	AccessibleGetNamedActionDescriptionDoer : public SpecificEventDoer<
									kEventClassAccessibility,
									kEventAccessibleGetNamedActionDescription> {
protected:
	virtual OSStatus	DoAccessibleGetNamedActionDescription(
								SysCarbonEvent&		ioEvent,
								AXUIElementRef		inAccessible,
								CFStringRef			inActionName,
								CFMutableStringRef	ioDescription) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxAccessibilityEvents
