// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRegisterAll.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxRegisterAll.h>
#include <PPxNibDecoderFuncs.h>
#include <PPxNibRegistrar.h>
#include <PPxXMLConstants.h>
#include <PPxXMLDecoder.h>
#include <PPxXMLEncoder.h>


namespace PPx {

#ifndef PPx_Doxygen_Parsing				// [Doxygen Bug Workaround] v 1.3.3
using namespace XMLConstants;			// Trouble parsing a using directive
#endif

// ---------------------------------------------------------------------------
//	RegisterCommonXMLDecoders
/**
	Registers XML Decoders for common data types. XML Decoders convert
	information from an XML Tree to Data Objects.
	
	You should call this function before attempting to read persistent
	data from XML information												*/
	
void
RegisterCommonXMLDecoders()
{
	XMLDecoder::Register(type_String,		   XMLDecoderFuncs::DecodeData<CFString>);
	XMLDecoder::Register(type_StringVector,	   XMLDecoderFuncs::DecodeVector<CFString>);
	XMLDecoder::Register(type_LocalizedString, XMLDecoderFuncs::DecodeData<LocalizedString>);
	
	XMLDecoder::Register(type_ObjectID,		   XMLDecoderFuncs::DecodeData<ObjectIDT>);
	XMLDecoder::Register(type_ObjectRef,	   XMLDecoderFuncs::DecodeData<ObjectStorageIDT>);
	XMLDecoder::Register(type_ObjectRefVector, XMLDecoderFuncs::DecodeVector<ObjectStorageIDT>);
	
	XMLDecoder::Register(type_bool,		  XMLDecoderFuncs::DecodeData<bool>);
	XMLDecoder::Register(type_boolVector, XMLDecoderFuncs::DecodeVector<bool>);
	
	XMLDecoder::Register(type_SInt8,  XMLDecoderFuncs::DecodeData<SInt8>);
	XMLDecoder::Register(type_UInt8,  XMLDecoderFuncs::DecodeData<UInt8>);
	XMLDecoder::Register(type_SInt16, XMLDecoderFuncs::DecodeData<SInt16>);
	XMLDecoder::Register(type_UInt16, XMLDecoderFuncs::DecodeData<UInt16>);
	XMLDecoder::Register(type_SInt32, XMLDecoderFuncs::DecodeData<SInt32>);
	XMLDecoder::Register(type_UInt32, XMLDecoderFuncs::DecodeData<UInt32>);
	
	XMLDecoder::Register(type_SInt16Vector, XMLDecoderFuncs::DecodeVector<SInt16>);
	
	XMLDecoder::Register(type_float,  XMLDecoderFuncs::DecodeData<float>);
	XMLDecoder::Register(type_double, XMLDecoderFuncs::DecodeData<double>);

	XMLDecoder::Register(type_Point,  XMLDecoderFuncs::DecodeData<Point>);
	XMLDecoder::Register(type_Rect,   XMLDecoderFuncs::DecodeData<Rect>);

	XMLDecoder::Register(type_CGPoint, XMLDecoderFuncs::DecodeData<CGPoint>);
	XMLDecoder::Register(type_CGSize,  XMLDecoderFuncs::DecodeData<CGSize>);
	XMLDecoder::Register(type_CGRect,  XMLDecoderFuncs::DecodeData<CGRect>);
}


// ---------------------------------------------------------------------------
//	RegisterCommonXMLEncoders
/**
	Registers XML Encoders for common data types. XML Encoders convert
	information from Data Objects to XML Trees.
	
	You should call this function before attempting to write persistent
	data in XML format														*/

void
RegisterCommonXMLEncoders()
{
	XMLEncoder::Register(typeid(TDataObject<CFString>), type_String, XMLEncoderFuncs::EncodeData<CFString>);
	XMLEncoder::Register(typeid(TDataVector<CFString>), type_StringVector, XMLEncoderFuncs::EncodeVector<CFString>);
	XMLEncoder::Register(typeid(TDataObject<LocalizedString>), type_LocalizedString, XMLEncoderFuncs::EncodeData<LocalizedString>);
	
	XMLEncoder::Register(typeid(TDataObject<ObjectIDT>), type_ObjectID, XMLEncoderFuncs::EncodeData<ObjectIDT>);
	XMLEncoder::Register(typeid(TDataObject<ObjectStorageIDT>), type_ObjectRef, XMLEncoderFuncs::EncodeData<ObjectStorageIDT>);
	XMLEncoder::Register(typeid(TDataVector<ObjectStorageIDT>), type_ObjectRefVector, XMLEncoderFuncs::EncodeVector<ObjectStorageIDT>);

	XMLEncoder::Register(typeid(TDataObject<bool>),   type_bool,	   XMLEncoderFuncs::EncodeData<bool>);
	XMLEncoder::Register(typeid(TDataVector<bool>),   type_boolVector, XMLEncoderFuncs::EncodeVector<bool>);
	
	XMLEncoder::Register(typeid(TDataObject<SInt8>),  type_SInt8,  XMLEncoderFuncs::EncodeData<SInt8>);
	XMLEncoder::Register(typeid(TDataObject<UInt8>),  type_UInt8,  XMLEncoderFuncs::EncodeData<UInt8>);
	XMLEncoder::Register(typeid(TDataObject<SInt16>), type_SInt16, XMLEncoderFuncs::EncodeData<SInt16>);
	XMLEncoder::Register(typeid(TDataObject<UInt16>), type_UInt16, XMLEncoderFuncs::EncodeData<UInt16>);
	XMLEncoder::Register(typeid(TDataObject<SInt32>), type_SInt32, XMLEncoderFuncs::EncodeData<SInt32>);
	XMLEncoder::Register(typeid(TDataObject<UInt32>), type_UInt32, XMLEncoderFuncs::EncodeData<UInt32>);
	
	XMLEncoder::Register(typeid(TDataVector<SInt16>), type_SInt16Vector, XMLEncoderFuncs::EncodeVector<SInt16>);
	
	XMLEncoder::Register(typeid(TDataObject<float>),  type_float,  XMLEncoderFuncs::EncodeData<float>);
	XMLEncoder::Register(typeid(TDataObject<double>), type_double, XMLEncoderFuncs::EncodeData<double>);
	
	XMLEncoder::Register(typeid(TDataObject<Point>),  type_Point,  XMLEncoderFuncs::EncodeData<Point>);
	XMLEncoder::Register(typeid(TDataObject<Rect>),   type_Rect,   XMLEncoderFuncs::EncodeData<Rect>);
	
	XMLEncoder::Register(typeid(TDataObject<CGPoint>), type_CGPoint, XMLEncoderFuncs::EncodeData<CGPoint>);
	XMLEncoder::Register(typeid(TDataObject<CGSize>),  type_CGSize,  XMLEncoderFuncs::EncodeData<CGSize>);
	XMLEncoder::Register(typeid(TDataObject<CGRect>),  type_CGRect,  XMLEncoderFuncs::EncodeData<CGRect>);
}


// ---------------------------------------------------------------------------
//	RegisterCommonNibDecoders
/**
	Registers decoders for common UI elements from Apple's Interface Builder
	nib files.  The nib decoders convert the information from Carbon-based
	nib files to PPx views.
	
	You should call this function before attempting to read persistent
	data from a nib file.  													*/
	
void
RegisterCommonNibDecoders()
{
	NibRegistrar::RegisterClass(CFSTR("IBCarbonBevelButton"),	NibDecoderFuncs::CreateBevelButton);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonButton"),		NibDecoderFuncs::CreateButton);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonChasingArrows"),	NibDecoderFuncs::CreateChasingArrows);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonCheckBox"),		NibDecoderFuncs::CreateCheckBox);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonClockDate"),		NibDecoderFuncs::CreateClockDate);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonComboBox"),		NibDecoderFuncs::CreateComboBox);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonDBColumnView"),	NibDecoderFuncs::CreateDBColumnView);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonDBListView"),	NibDecoderFuncs::CreateDBListView);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonEditText"),		NibDecoderFuncs::CreateEditText);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonGroupBox"),		NibDecoderFuncs::CreateGroupBox);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonHIView"),		NibDecoderFuncs::CreateHIView);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonIcon"),			NibDecoderFuncs::CreateIcon);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonImageView"),		NibDecoderFuncs::CreateImageView);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonImageWell"),		NibDecoderFuncs::CreateImageWell);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonLittleArrows"),	NibDecoderFuncs::CreateLittleArrows);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonPicture"),		NibDecoderFuncs::CreatePicture);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonPopupArrow"),	NibDecoderFuncs::CreatePopupArrow);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonPopupButton"),	NibDecoderFuncs::CreatePopupButton);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonProgressBar"),	NibDecoderFuncs::CreateProgressBar);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonRadioButton"),	NibDecoderFuncs::CreateRadioButton);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonRadioGroup"),	NibDecoderFuncs::CreateRadioGroup);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonRelevanceBar"),	NibDecoderFuncs::CreateRelevanceBar);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonRoundButton"),	NibDecoderFuncs::CreateRoundButton);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonScrollBar"),		NibDecoderFuncs::CreateScrollBar);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonScrollView"),	NibDecoderFuncs::CreateScrollView);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonSearchField"),	NibDecoderFuncs::CreateSearchField);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonSegmentView"),	NibDecoderFuncs::CreateSegmentedView);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonSeparator"),		NibDecoderFuncs::CreateSeparator);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonSlider"),		NibDecoderFuncs::CreateSlider);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonStaticText"),	NibDecoderFuncs::CreateStaticText);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonTab"),			NibDecoderFuncs::CreateTab);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonTextView"),		NibDecoderFuncs::CreateTextView);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonTriangle"),		NibDecoderFuncs::CreateTriangle);
	NibRegistrar::RegisterClass(CFSTR("IBCarbonUserPane"),		NibDecoderFuncs::CreateUserPane);
}


} // namespace PPx