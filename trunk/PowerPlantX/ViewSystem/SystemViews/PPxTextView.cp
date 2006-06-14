// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTextView.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxTextView.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCFData.h>
#include <SysCreateView.h>

// The system text view is only available on Mac OS 10.3 or later
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

namespace PPx {


// ---------------------------------------------------------------------------
//	TextView														  [public]
/**
	Default constructor */

TextView::TextView()
{
}


// ---------------------------------------------------------------------------
//	~TextView														  [public]
/**
	Destructor */

TextView::~TextView()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from text view creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inOptions			Any options desired for the view
	@param	inFrameOptions		Any frame options desired for the TXN
								object creation								*/

void
TextView::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	OptionBits		inOptions,
	TXNFrameOptions	inFrameOptions)
{
	HIViewRef	viewRef = SysCreateView::TextView(inOptions, inFrameOptions);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
TextView::ClassName() const
{
	return CFSTR("PPx::TextView");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
TextView::InitState(
	const DataReader&	inReader)
{
	// ### Not Finished
	OptionBits		options = 0;
	TXNFrameOptions	frameOptions = 0;

	HIViewRef	viewRef = SysCreateView::TextView(options, frameOptions);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
TextView::WriteState(
	DataWriter&	ioWriter) const
{
	// ### Not Finished
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetTextObject													  [public]
/**
	Gets the TXNObject that backs the text view
	
	@return TXNObject that backs the text view								*/

TXNObject
TextView::GetTextObject() const
{
	return ::HITextViewGetTXNObject(GetSysView());
}


// ---------------------------------------------------------------------------
//	SetText															  [public]

void
TextView::SetText(
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
	
	OSStatus	status = ::TXNSetData( GetTextObject(),
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
TextView::GetText() const
{
	CFString	theText;

	Handle		dataH  = nil;
	OSStatus	status = ::TXNGetDataEncoded( GetTextObject(),
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


} // namespace PPx

#endif // 10.3 or later
