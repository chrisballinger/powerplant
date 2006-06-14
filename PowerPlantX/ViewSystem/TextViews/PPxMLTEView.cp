// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMLTEView.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

//	$$$ Incomplete $$$	Class is still under construction

#include <PPxMLTEView.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCFData.h>
#include <SysCFDictionary.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	InitializeSystem

void
MLTEView::InitializeSystem(
	TXNInitOptions							inOptions,
	const TXNMacOSPreferredFontDescription*	inFonts,
	ItemCount								inFontCount)
{
	static	bool	sSystemIsInitialized = false;
	
	if (not sSystemIsInitialized) {
		OSStatus	status = ::TXNInitTextension(inFonts, inFontCount, inOptions);
		
			// It's OK if TXN is already initialized. In fact,
			// OS 10.3 and later automatically initialize TXN.
		
		if ( (status != noErr)  &&
			 (status != kTXNAlreadyInitializedErr) ) {
		
			PPx_ThrowOSError_(status, "TXNInitTextension failed");
		}

		sSystemIsInitialized = true;
	}
}


// ---------------------------------------------------------------------------
//	MLTEView

MLTEView::MLTEView()
{
	InitializeSystem();

	mTXNObject		= nil;
	mTXNFrameID		= 0;
	mFrameOptions	= 0;
	mFrameType		= kTXNTextEditStyleFrameType;
	mFileType		= kTXNTextensionFile;
	mEncodingType	= kTXNSystemDefaultEncoding;
}


// ---------------------------------------------------------------------------
//	~MLTEView

MLTEView::~MLTEView()
{
	if (mTXNObject != nil) {
		::TXNDeleteObject(mTXNObject);
	}
}


// ---------------------------------------------------------------------------
//	Initialize

void
MLTEView::Initialize(
	View*				inSuperView,
	const HIRect&		inFrame,
	bool				inVisible,
	bool				inEnabled,
	TXNFrameOptions		inFrameOptions,
	TXNFrameType		inFrameType,
	TXNFileType			inFileType,
	TXNPermanentTextEncodingType	inEncodingType)
{
	mFrameOptions	= inFrameOptions;
	mFrameType		= inFrameType;
	mFileType		= inFileType;
	mEncodingType	= inEncodingType;

	BaseView::Initialize(inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetTextObject

TXNObject
MLTEView::GetTextObject() const
{
	return mTXNObject;
}


// ---------------------------------------------------------------------------
//	SetText															  [public]

void
MLTEView::SetText(
	CFStringRef	inText)
{
	CFString		textStr(inText);
	
		// Try to get a pointer to the characters in the CFString
	
	const UniChar*	textPtr		= textStr.GetUniStringPtr();
	SInt32			strLength	= textStr.GetLength();
	CFData			buffer(0);
	
	if (textPtr == nil) {
	
			// Couldn't get a pointer to the CFString characters. We
			// need to allocate a buffer and copy from the CFString
			
		buffer.SetLength(strLength * sizeof(UniChar));
		textStr.GetSubstring( CFRangeMake(0, strLength),
							  (UniChar*) buffer.GetMutableBytePtr() );
		
		textPtr = (const UniChar*) buffer.GetBytePtr();
	}
	
	OSStatus	status = ::TXNSetData( mTXNObject,
									   kTXNUnicodeTextData,
									   (void*) textPtr,
									   strLength * sizeof(UniChar),
									   kTXNStartOffset,
									   kTXNEndOffset );
	
	PPx_ThrowIfOSError_(status, "TXNSetData failed");
}


// ---------------------------------------------------------------------------
//	GetText															  [public]

CFString
MLTEView::GetText() const
{
	CFString	theText;

	Handle		dataH  = nil;
	OSStatus	status = ::TXNGetDataEncoded( mTXNObject,
											  kTXNStartOffset,
											  kTXNEndOffset,
											  &dataH,
											  kTXNUnicodeTextData );

	PPx_SignalIf_(status != noErr);
											  
	if ((status == noErr)  &&  (dataH != nil)) {
		AutoHandle	autoData(dataH);
		Size	textLength = (Size) (::GetHandleSize(dataH) / sizeof(UniChar));
		
		theText = CFString(reinterpret_cast<UniChar*>(*dataH), textLength);
	}
	
	return theText;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetOneControlTag

void
MLTEView::SetOneControlTag(
	TXNControlTag		inTag,
	TXNControlData		inData)
{
	OSStatus	status = ::TXNSetTXNObjectControls(
								mTXNObject, false, 1, &inTag, &inData );
								
	PPx_ThrowIfOSError_(status, "TXNSetTXNObjectControls failed");
}


// ---------------------------------------------------------------------------
//	SetCGContext

void
MLTEView::SetCGContext(
	CGContextRef	inCGContext)
{
	TXNControlData	data;
	data.uValue = (UInt32) inCGContext;
	
	SetOneControlTag(kATSUCGContextTag, data);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetOneTypeAttribute

void
MLTEView::SetOneTypeAttribute(
	TXNTypeAttributes*	inAttribute,
	TXNOffset			inStartOffset,
	TXNOffset			inEndOffset)
{
	OSStatus	status = ::TXNSetTypeAttributes( mTXNObject, 1, inAttribute,
											  inStartOffset, inEndOffset );

	PPx_ThrowIfOSError_(status, "TXNSetTypeAttributes failed");
}


// ---------------------------------------------------------------------------
//	SetOneTypeAttributeForSelection

void
MLTEView::SetOneTypeAttributeForSelection(
	TXNTypeAttributes*	inAttribute)
{
	OSStatus	status = ::TXNSetTypeAttributes( mTXNObject, 1, inAttribute,
												 kTXNUseCurrentSelection,
												 kTXNUseCurrentSelection );

	PPx_ThrowIfOSError_(status, "TXNSetTypeAttributes failed");
}


// ---------------------------------------------------------------------------
//	SetFontName

void
MLTEView::SetFontName(
	ConstStringPtr	inName)
{
	TXNTypeAttributes	attribute;
	
	attribute.tag	= kTXNQDFontNameAttribute;
	attribute.size	= kTXNQDFontNameAttributeSize;
	attribute.data.dataPtr = (void*) inName;
									  
	SetOneTypeAttributeForSelection(&attribute);
}


// ---------------------------------------------------------------------------
//	SetFontSize

void
MLTEView::SetFontSize(
	Fixed	inSize)
{
	TXNTypeAttributes	attribute;
	
	attribute.tag	= kTXNQDFontSizeAttribute;
	attribute.size	= kTXNFontSizeAttributeSize;
	attribute.data.dataValue = (UInt32) inSize;
									  
	SetOneTypeAttributeForSelection(&attribute);
}


// ---------------------------------------------------------------------------
//	SetFontStyle

void
MLTEView::SetFontStyle(
	Style	inStyle)
{
	TXNTypeAttributes	attribute;
	
	attribute.tag	= kTXNQDFontStyleAttribute;
	attribute.size	= kTXNQDFontStyleAttributeSize;
	attribute.data.dataValue = inStyle;
									  
	SetOneTypeAttributeForSelection(&attribute);
}


// ---------------------------------------------------------------------------
//	SetFontColor

void
MLTEView::SetFontColor(
	const RGBColor&	inColor)
{
	TXNTypeAttributes	attribute;
	
	attribute.tag	= kTXNQDFontColorAttribute;
	attribute.size	= kTXNQDFontColorAttributeSize;
	attribute.data.dataPtr = (void*) &inColor;
									  
	SetOneTypeAttributeForSelection(&attribute);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	FinishInit													   [protected]
/**
	Finishes initialization */

void
MLTEView::FinishInit()
{
	HIRect	frame;
	GetFrame(frame);
	
	Rect	frameRect;
	ViewUtils::HIToQDRect(frame, frameRect);

	OSStatus	status;
	status = ::TXNNewObject( nil,				// FSSpec*
							 GetSysWindow(),
							 &frameRect,
							 mFrameOptions,
							 mFrameType,
							 mFileType,
							 mEncodingType,
							 &mTXNObject,
							 &mTXNFrameID,
							 0 );				// RefCon
							 
	PPx_ThrowIfOSError_(status, "TXNNewObject failed");
	
		// +++ Set up Carbon Event handling for MLTE itself
		
	CFStringRef		keys[] = { kTXNTextHandlerKey, kTXNWindowEventHandlerKey };
	EventTargetRef	targets[] = { ::GetWindowEventTarget(GetSysWindow()),
								  ::GetWindowEventTarget(GetSysWindow()) };
		
	CFDictionary<CFStringRef, EventTargetRef>	targetDict( keys,
															targets,
															2,
															&kCFTypeDictionaryKeyCallBacks);
		
	TXNCarbonEventInfo	eventInfo;
	eventInfo.useCarbonEvents = true;
	eventInfo.flags = kTXNNoAppleEventHandlersMask;
	eventInfo.fDictionary = targetDict.UseRef();
	
	TXNControlData	data;
	data.uValue  = (UInt32) &eventInfo;
	
	SetOneControlTag(kTXNUseCarbonEvents, data);
		
	EventTargetRef	target = GetSysEventTarget();

	ControlDrawDoer::Install(target);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
MLTEView::ClassName() const
{
	return CFSTR("PPx::MLTEView");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
MLTEView::InitState(
	const DataReader&	inReader)
{
	CFString		text;					// Data for MLTEView

	inReader.ReadOptional(key_TextContent,	text);
	
	PPx::BaseView::InitState(inReader);		// Data for superclass
	
	SetText(text);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
MLTEView::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for MLTEView
	ioWriter.WriteValue(key_TextContent,	GetText());
	
	PPx::BaseView::WriteState(ioWriter);	// Data for superclass
	
}


// ---------------------------------------------------------------------------
//	DoControlDraw												   [protected]

//	Note: We can't use the CGContextRef passed with MLTE because they
//	use different coordinate systems. MLTE expects a traditional CG
//	system with (0,0) at the bottom left. HIView uses a Quickdraw-compatible
//	system with (0,0) at the top left.

OSStatus
MLTEView::DoControlDraw(
	SysCarbonEvent&		/* ioEvent */,
	ControlRef			/* inControl */,
	ControlPartCode		/* inPartCode */,
	RgnHandle			/* inClipRgn */,
	CGContextRef		/* inContext */)
{
	HIRect	frame;
	GetFrame(frame);
	
	Rect	frameRect;
	ViewUtils::HIToQDRect(frame, frameRect);
	
	ThemeDrawState	drawState = IsActive() ? kThemeStateActive
										   : kThemeStateInactive;
										   
//	::DrawThemeListBoxFrame(&frameRect, drawState);
	
	::TXNDraw(mTXNObject, nil);
											 
	return noErr;
}


} // namespace PPx
