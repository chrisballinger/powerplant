// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UNavServicesDialogs.h		PowerPlant 2.2.2	©1998-2005 Metrowerks Inc.
// ===========================================================================
//
//	Implementation of UStandardDialogs, LFileChooser, and LFileDesignator
//	that uses Navigation Services

#ifndef _H_UNavServicesDialogs
#define _H_UNavServicesDialogs
#pragma once

#include <PP_Prefix.h>
#include <LFileTypeList.h>
#include <UAppleEventsMgr.h>
#if !defined(__MACH__)
	#include <Navigation.h>
#endif

#if PP_Uses_Pragma_Import
	#pragma import on
#endif

PP_Begin_Namespace_PowerPlant

// ---------------------------------------------------------------------------

namespace UNavServicesDialogs {

	// -----------------------------------------------------------------------
	//	Wrapper class for a NavReplyRecord

	class	StNavReplyRecord {
		public:
								StNavReplyRecord();
								~StNavReplyRecord();

			void				SetDefaultValues();
			
			OSStatus			UpdateReply (
									NavDialogRef	inDialogRef );

			operator NavReplyRecord*()		{ return &mNavReply; }

			NavReplyRecord&		Get()		{ return mNavReply; }

			bool				IsValid() const
											{ return mNavReply.validRecord; }

			bool				IsReplacing() const
											{ return mNavReply.replacing; }

			bool				IsStationery() const
											{ return mNavReply.isStationery; }

			AEDescList			Selection()	const
											{ return mNavReply.selection; }

			ScriptCode			KeyScript()	const
											{ return mNavReply.keyScript; }

			void				GetFileSpec(
										FSSpec&		outFileSpec) const;
			
			void				GetFileSpec(
										FSRef*		outFileSpec) const;
			
			CFStringRef			CopySaveFileName () const;

		protected:
			NavReplyRecord		mNavReply;
			bool				mAllocated;

		private:					// Unimplemented to prevent copying
								StNavReplyRecord(
										const StNavReplyRecord&);
			StNavReplyRecord&	operator = (
										const StNavReplyRecord&);
	};

	// -----------------------------------------------------------------------

	#include <UStandardDialogs.i>

	// -----------------------------------------------------------------------

	class	LFileChooser {
		public:
			#include <LFileChooser.i>

	
			CFArrayRef			GetUTIList () const;
			
			void				CopyUTIList (
									CFArrayRef		inList );
			
			void				AddUTIToList (
									CFStringRef		inUTI );
			
			void				AddUTIListToList (
									CFArrayRef		inUTIList );
			
			void				DeriveUTIList (
									const LFileTypeList&	inFileTypes );

		protected:
			StNavReplyRecord		mNavReply;
//			NavDialogOptions		mNavOptions;
			NavDialogCreationOptions	mNavOptions;
			NavObjectFilterProcPtr	mNavFilterProc;
			NavPreviewProcPtr		mNavPreviewProc;
			StAEDescriptor			mDefaultLocation;
			bool					mSelectDefault;
			CFMutableArrayRef		mUTIList;
	};

	// -----------------------------------------------------------------------

	class	LFileDesignator {
		public:
			#include <LFileDesignator.i>

		protected:
			StNavReplyRecord	mNavReply;
//			NavDialogOptions	mNavOptions;
			NavDialogCreationOptions	mNavOptions;
			OSType				mFileType;
			OSType				mFileCreator;
			StAEDescriptor		mDefaultLocation;
			bool				mSelectDefault;
	};

	// -----------------------------------------------------------------------
	//	Event filter callback function

	pascal void	NavEventProc(
						NavEventCallbackMessage		inSelector,
						NavCBRecPtr					ioParams,
						NavCallBackUserData			ioUserData);

	class	StNavEventUPP {
		public:
			StNavEventUPP( NavEventProcPtr inProcPtr );
			~StNavEventUPP();

			operator NavEventUPP()		{ return mNavEventUPP; }

		protected:
			NavEventUPP		mNavEventUPP;
	};

	// -----------------------------------------------------------------------
	//	Object Filter UPP

	class	StNavObjectFilterUPP {
		public:
			StNavObjectFilterUPP( NavObjectFilterProcPtr inProcPtr );
			~StNavObjectFilterUPP();

			operator NavObjectFilterUPP()	{ return mNavObjectFilterUPP; }

		protected:
			NavObjectFilterUPP	mNavObjectFilterUPP;
	};

	// -----------------------------------------------------------------------
	//	Preview UPP

	class	StNavPreviewUPP {
		public:
			StNavPreviewUPP( NavPreviewProcPtr inProcPtr );
			~StNavPreviewUPP();

			operator NavPreviewUPP()		{ return mNavPreviewUPP; }

		protected:
			NavPreviewUPP		mNavPreviewUPP;
	};
}

PP_End_Namespace_PowerPlant

#if PP_Uses_Pragma_Import
	#pragma import reset
#endif

#endif
