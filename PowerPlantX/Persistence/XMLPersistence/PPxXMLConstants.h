// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxXMLConstants.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:24 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxXMLConstants.h
	@brief		Constants for XML identifiers
*/

#ifndef H_PPxXMLConstants
#define H_PPxXMLConstants
#pragma once

#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	XMLConstants
/**
	Constants for XML identifiers */

namespace XMLConstants {

			// Element and atribute names

	const CFStringRef	elem_Persistent			= CFSTR("Persistent");

	const CFStringRef	attr_Name				= CFSTR("name");
	const CFStringRef	attr_StorageID			= CFSTR("id");
	const CFStringRef	attr_Class				= CFSTR("class");
	const CFStringRef	attr_Table				= CFSTR("table");
	

	const CFStringRef	bool_True				= CFSTR("true");
	const CFStringRef	bool_False				= CFSTR("false");
	
	const int			max_TabLevel			 = 5;
	const CFStringRef	whitespace_NewLineTabs[] = { CFSTR("\n"),
													 CFSTR("\n\t"),
													 CFSTR("\n\t\t"),
													 CFSTR("\n\t\t\t"),
													 CFSTR("\n\t\t\t\t"),
													 CFSTR("\n\t\t\t\t\t") };

			// Data types

	const CFStringRef	type_String				= CFSTR("String");
	const CFStringRef	type_StringVector		= CFSTR("StringVector");
	const CFStringRef	type_LocalizedString	= CFSTR("LocalizedString");
	const CFStringRef	type_ObjectID			= CFSTR("ObjectID");
	const CFStringRef	type_ObjectRef			= CFSTR("ObjectRef");
	const CFStringRef	type_ObjectRefVector	= CFSTR("ObjectRefVector");
	
	const CFStringRef	type_bool				= CFSTR("bool");
	const CFStringRef	type_boolVector			= CFSTR("boolVector");
	
	const CFStringRef	type_SInt8				= CFSTR("SInt8");
	const CFStringRef	type_UInt8				= CFSTR("UInt8");
	const CFStringRef	type_SInt16				= CFSTR("SInt16");
	const CFStringRef	type_UInt16				= CFSTR("UInt16");
	const CFStringRef	type_SInt32				= CFSTR("SInt32");
	const CFStringRef	type_UInt32				= CFSTR("UInt32");
	
	const CFStringRef	type_SInt16Vector		= CFSTR("SInt16Vector");
	
	const CFStringRef	type_float				= CFSTR("float");
	const CFStringRef	type_double				= CFSTR("double");
	
	
			// Struct types and field names

	const CFStringRef	type_Point				= CFSTR("Point");
	const CFStringRef	field_Point_h			= CFSTR("h");
	const CFStringRef	field_Point_v			= CFSTR("v");

	const CFStringRef	type_Rect				= CFSTR("Rect");
	const CFStringRef	field_Rect_top			= CFSTR("top");
	const CFStringRef	field_Rect_left			= CFSTR("left");
	const CFStringRef	field_Rect_bottom		= CFSTR("bottom");
	const CFStringRef	field_Rect_right		= CFSTR("right");
	
	const CFStringRef	type_CGPoint			= CFSTR("CGPoint");
	const CFStringRef	field_CGPoint_x			= CFSTR("x");
	const CFStringRef	field_CGPoint_y			= CFSTR("y");
	
	const CFStringRef	type_CGSize				= CFSTR("CGSize");
	const CFStringRef	field_CGSize_width		= CFSTR("width");
	const CFStringRef	field_CGSize_height		= CFSTR("height");
	
	const CFStringRef	type_CGRect				= CFSTR("CGRect");
	const CFStringRef	field_CGRect_origin		= CFSTR("origin");
	const CFStringRef	field_CGRect_size		= CFSTR("size");
}


} // namespace PPx

#endif // H_PPxXMLConstants