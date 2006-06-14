// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LWindowPositioner.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include <LAttachment.h>

const ResType	ResType_WindowPosition	= 'WSPC';
const MessageT	cmd_PositionWindow		= 'wpos';
const MessageT	cmd_RecordPosition		= 'rpos';


// ===========================================================================
//		* SWindowPosition
// ===========================================================================

#pragma options align=mac68k

struct SWindowPosition {
	Point			windowPosition;
	SDimension16	windowSize;
};

typedef struct SWindowPosition SWindowPosition, *SWindowPositionP, **SWindowPositionH;

#pragma options align=reset


// ===========================================================================
//		* LWindowPositioner
// ===========================================================================
//	An LWindowPositioner can be attached to an LWindow if you want to
//	have the window remember its location and/or size from one
//	instance to the next.

class LWindowPositioner : public LAttachment {

public:
	enum { class_ID = 'wpos' };

	static void*				CreateWindowPositionerStream(
										LStream*			inStream);

								LWindowPositioner(
										LWindow*			inWindow,
										ResIDT				inResID,
										SInt16				inHorizStagger = 16,
										SInt16				inVertStagger = 16,
										Boolean				inAdjustPosition = true,
										Boolean				inAdjustSize = false);
								LWindowPositioner(
										LStream*			inStream);
		
	virtual						~LWindowPositioner();

	// message dispatch
	
protected:
	virtual void				ExecuteSelf(
										MessageT			inMessage,
										void*				ioParam);

	virtual void				DoWindowPosition();
	virtual void				DoRecordWindowPosition();
	
	// window placement
	
	virtual void				ChooseWindowPosition(
										SWindowPositionH	inWindowPositionH);			
	virtual void				ChooseWindowSize(
										SWindowPositionH	inWindowPositionH);			
	virtual SWindowPositionH	RecordWindowPosition();			
	
	virtual GDHandle			ChooseDisplayForWindow(
										const Rect&			inWindowBounds);
	virtual void				CropWindowToDisplay(
										Rect&				ioWindowBounds,
										GDHandle			inDeviceH);
	virtual void				PlaceWindowOnDisplay(
										Rect&				ioWindowBounds,
										GDHandle			inDeviceH);

	virtual void				CalcUsableDisplayArea(
										GDHandle			inDeviceH,
										Rect&				outDeviceBounds,
										SDimension16&		outDeviceSize);
	virtual Boolean				ValidateWindowPosition(
										Point&				inStructUL,
										Rect&				inULBounds);
	
	// window position resource
	
	virtual SWindowPositionH	ReadWindowPosition(
										ResIDT				inResID);
	virtual void				WriteWindowPosition(
										ResIDT				inResID,
										SWindowPositionH	inWindowPositionH);			

	// accessor
	
	LWindowPositioner*		 	FindPositionerByKind(
										ResIDT				inResID);

	// common initialization

private:
	void						InitWindowPositioner();
	
	static OSStatus				FlipWindowPosition(
										OSType				dataDomain,
										OSType				dataType,
										short				id,
										void				*dataPtr,
										UInt32				dataSize,
										Boolean				currentlyNative,
										void				*refcon);
	

	// data members

protected:
	LWindow*					mWindow;					// window we're attached to
	ResIDT						mResID;						// resource ID for window position resource
	SDimension16				mWindowStagger;				// spacing between windows of the same type
	Boolean						mAdjustPosition;			// true to adjust position
	Boolean						mAdjustSize;				// true to adjust window size

	Boolean						mRecordWindowInfo;			// true to record window position when closed
	Boolean						mHasSetPosition;			// true if DoWindowPosition has been called

	static LArray				sWindowPositioners;			// list of window positioners

};
