// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GReferencedWindow.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/09/95
//     $Date: 2006/04/12 08:47:44 $
//	$History: GReferencedWindow.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 03/17/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Implemented option-click in close box to close all windows. (Suggestion
//	#1067.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:57
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:28
//	Updated in $/Constructor/Source files/Display classes (Eric)/Window classes
//	Added AttemptClose override. It now treats Close as an essential
//	command, which means it can allocate memory when memory reserves are
//	low. Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

// ===========================================================================
//		* Forward class declaration
// ===========================================================================

class GZoomer;


// ===========================================================================
//		* GReferencedWindow
// ===========================================================================
//
//	Variant on LWindow that provides several behaviors important
//	to Constructor:
//
//	  *	Ensures that the application's resource fork is topmost before
//		attempting any drawing behavior.
//    *	Identifies itself as a property of another LModelObject when
//		so directed.
//	  *	Provides hooks for implementing smart zooming.
//
//	In Constructor, this class is registered in place of LWindow.
//	It takes the same stream data as LWindow.
//
// ===========================================================================

class GReferencedWindow : public LWindow {

public:
	enum { class_ID = 'REFw' };

	static LWindow*			CreateFromStream(LStream* inStream)
									{ return new GReferencedWindow(inStream); }

							GReferencedWindow(LStream* inStream);
							GReferencedWindow(const SWindowInfo& inWindowInfo);
	virtual					~GReferencedWindow();
	
	// configuration
	
	void					SetReference(
									DescType			inWindowType,
									LModelObject*		inSuperModel);
	void					SetZoomer(
									GZoomer*			inZoomer);
	
	// zooming behavior
	
	virtual void			CalcStandardBoundsForScreen(
									const Rect&			inScreenBounds,
									Rect&				outStdBounds) const;
	virtual Boolean			CalcStandardBounds(
									Rect&				outStdBounds) const;

	// zoom window command

	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);
	virtual Boolean			ObeyCommand(
									CommandT			inCommand,
									void*				ioParam);

	// option-click
	
	virtual void			ClickInGoAway(
									const EventRecord&	inMacEvent);

	// overrides to enforce resource context
	
	virtual void			Activate();
	virtual void			Deactivate();
	virtual void			UpdatePort();

	virtual void			HandleClick(
									const EventRecord&	inMacEvent,
									SInt16				inPart);
	
	// overrides to handle low-memory situations

	virtual void			AttemptClose();


	// AEOM behavior overrides

protected:
	virtual void			HandleAppleEvent(
									const AppleEvent&	inAppleEvent,
									AppleEvent&			outAEReply,
									AEDesc&				outResult,
									long				inAENumber);

	virtual void			MakeSelfSpecifier(
									AEDesc&				inSuperSpecifier,
									AEDesc&				outSelfSpecifier) const;

	// drawing overrides
	
	virtual void			DrawSizeBox();


	Boolean					mHasReference;			// true if SetReference() has been called
	GZoomer*				mZoomer;				// reference to GZoomer sizing object
	
};


// ===========================================================================
//		* GZoomer
// ===========================================================================
//	Mix-in class for objects which wish to control the zooming behavior of a window.

class GZoomer {

public:
	inline 					GZoomer() {}
	virtual inline			~GZoomer() {}
	
	virtual void			CalcStandardSize(GReferencedWindow* inWindow) = 0;

};
