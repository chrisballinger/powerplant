// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNavServices.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:27 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxNavServices.h
	@brief		Classs and functions for using Navigation Servicers
*/

#ifndef H_PPxNavServices
#define H_PPxNavServices
#pragma once

#include <PPxMemoryUtils.h>
#include <PPxPrefix.h>
#include <SysAEDesc.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	NavEventResponder
/**
	Abstract class for handling NavServices callbacks						*/

class	NavEventResponder {
public:
	virtual				~NavEventResponder();
	
	void				SetFileTypeList( NavTypeListHandle inFileTypeList );
	
	void				SetFileTypeList(
								SInt16		inNumFileTypes,
								OSType*		inFileTypes);
	
	NavTypeListHandle	GetFileTypeList() const;

	void				InvokeNavUserAction( NavCBRecPtr inParams );
	
	void				InvokeNavTerminate( NavCBRecPtr inParams );
	
	void				InvokeNavEventCallback(
								NavEventCallbackMessage	inMessage,
								NavCBRecPtr				inParams);

	bool				InvokeNavObjectFilterCallback(
								const AutoAEDesc&			inItem,
								const NavFileOrFolderInfo*	inFileOrFolderInfo,
								NavFilterModes				inFilterMode);

	bool				InvokeNavPreviewCallback( NavCBRecPtr inParams );

private:
	virtual void		DoNavUserAction( NavCBRecPtr inParams ) = 0;
	
	virtual void		DoNavTerminate( NavCBRecPtr inParams );
	
	virtual void		DoNavEventCallback(
								NavEventCallbackMessage	inMessage,
								NavCBRecPtr				inParams);

	virtual bool		DoNavObjectFilterCallback(
								const AutoAEDesc&			inItem,
								const NavFileOrFolderInfo*	inFileOrFolderInfo,
								NavFilterModes				inFilterMode);

	virtual bool		DoNavPreviewCallback( NavCBRecPtr inParams );

private:
	PPx::AutoHandle		mFileTypeList;
};


// ---------------------------------------------------------------------------
//	InvokeNavUserAction
/**
	Non-virtual wrapper for calling DoNavUserAction function
	
	@param	inParams		NavServices callback record						*/

inline void
NavEventResponder::InvokeNavUserAction(
	NavCBRecPtr		inParams)
{
	DoNavUserAction(inParams);
}


// ---------------------------------------------------------------------------
//	InvokeNavTerminate
/**
	Non-virtual wrapper for calling DoNavTerminate function
	
	@param	inParams		NavServices callback record						*/

inline void
NavEventResponder::InvokeNavTerminate(
	NavCBRecPtr		inParams)
{
	DoNavTerminate(inParams);
}


// ---------------------------------------------------------------------------
//	InvokeNavEventCallback
/**
	Non-virtual wrapper for calling DoNavEventCallback function
	
	@param	inMessage		NavServices callback message
	@param	inParams		NavServices callback record						*/

inline void
NavEventResponder::InvokeNavEventCallback(
	NavEventCallbackMessage	inMessage,
	NavCBRecPtr				inParams)
{
	DoNavEventCallback(inMessage, inParams);
}


// ---------------------------------------------------------------------------
//	InvokeNavObjectFilterCallback
/**
	Non-virtual wrapper for calling DoNavObjectFilterCallback function
	
	@param	inItem				NavServices callback item
	@param	inFileOrFolderInfo	NavServices callback info
	@param	inFilterMode		NavServices callback filter mode			*/

inline bool
NavEventResponder::InvokeNavObjectFilterCallback(
	const AutoAEDesc&			inItem,
	const NavFileOrFolderInfo*	inFileOrFolderInfo,
	NavFilterModes				inFilterMode)
{
	return DoNavObjectFilterCallback(inItem, inFileOrFolderInfo, inFilterMode);
}


// ---------------------------------------------------------------------------
//	InvokeNavPreviewCallback
/**
	Non-virtual wrapper for calling DoNavPreviewCallback function
	
	@param	inParams		NavServices callback record						*/

inline bool
NavEventResponder::InvokeNavPreviewCallback(
	NavCBRecPtr	inParams)
{
	return DoNavPreviewCallback(inParams);
}


// ===========================================================================
//	AutoNavReply
/**
	Manages ownership of a Toolbox NavReplyRecord. The constructor fills
	in the record and the destructor disposes of it.						*/

class	AutoNavReply {
public:
						AutoNavReply( NavDialogRef inNavDialog );
						
						~AutoNavReply();
						
	const NavReplyRecord&
						Get() const;

private:				
	NavReplyRecord		mNavReply;
};


// ===========================================================================
//	NavServices
/**
	Utility functions for displaying NavServices dialogs */

namespace NavServices {

	void	GetDefaultCreationOptions( NavDialogCreationOptions& outOptions );
	
									// Confirmation dialogs
	void	AskSaveChanges(
					NavEventResponder&				inResponder,
					NavAskSaveChangesAction			inAction,
					const NavDialogCreationOptions&	inOptions);
	
	void	AskSaveChanges(
					NavEventResponder&				inResponder,
					NavAskSaveChangesAction			inAction,
					WindowRef						inParentWindow);
					
	void	AskDiscardChanges(
					NavEventResponder&				inResponder,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskDiscardChanges(
					NavEventResponder&				inResponder,
					WindowRef						inParentWindow);
					
	void	AskReviewDocuments(
					NavEventResponder&				inResponder,
					UInt32							inDocumentCount,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskReviewDocuments(
					NavEventResponder&				inResponder,
					UInt32							inDocumentCount);
					
	NavUserAction
			AskReviewDocuments( UInt32 inDocumentCount );
					
									// Designating
	void	AskDesignateFile(
					NavEventResponder&				inResponder,
					OSType							inFileType,
					OSType							inFileCreator,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskDesignateFile(
					NavEventResponder&				inResponder,
					OSType							inFileType,
					CFStringRef						inDefaultName,
					WindowRef						inParentWindow);
					
									// Choosing files
	void	AskGetFile(
					NavEventResponder&				inResponder,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskGetFile( NavEventResponder& inResponder );
					
	void	AskGetFile(
					NavEventResponder&				inResponder,
					NavTypeListHandle				inTypeList,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskGetFile(
					NavEventResponder&				inResponder,
					NavTypeListHandle				inTypeList);
					
	void	AskChooseFile(
					NavEventResponder&				inResponder,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskChooseFile( NavEventResponder& inResponder );

	void	AskChooseFile(
					NavEventResponder&				inResponder,
					NavTypeListHandle				inTypeList,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskChooseFile(
					NavEventResponder&				inResponder,
					NavTypeListHandle				inTypeList);

									// Choosing folders
	void	AskChooseFolder(
					NavEventResponder&				inResponder,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskChooseFolder( NavEventResponder& inResponder );

									// Choosing volumes
	void	AskChooseVolume(
					NavEventResponder&				inResponder,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskChooseVolume( NavEventResponder& inResponder );

									// Choosing objects
	void	AskChooseObject(
					NavEventResponder&				inResponder,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskChooseObject( NavEventResponder& inResponder );

									// Creating folders
	void	AskNewFolder(
					NavEventResponder&				inResponder,
					const NavDialogCreationOptions&	inOptions);
					
	void	AskNewFolder( NavEventResponder& inResponder );

}


// ===========================================================================
//	SysNavEventUPP
/**
	Wrapper class for a Navigation Services Event callback function UPP */

class	SysNavEventUPP {
public:
	explicit			SysNavEventUPP( NavEventProcPtr inCallbackFunc );
	
						~SysNavEventUPP();
						
	NavEventUPP			Get() const;
	
private:
	NavEventUPP			mNavEventUPP;
	
private:								// Forbid copy and assignment
						SysNavEventUPP( const SysNavEventUPP& );
	SysNavEventUPP&		operator = ( const SysNavEventUPP& );
};


// ---------------------------------------------------------------------------
//	Get

inline
NavEventUPP
SysNavEventUPP::Get() const
{
	return mNavEventUPP;
}


// ===========================================================================
//	SysNavObjectFilterUPP
/**
	Wrapper class for a Navigation Services Object Filter callback function UPP */

class	SysNavObjectFilterUPP {
public:
	explicit			SysNavObjectFilterUPP( NavObjectFilterProcPtr inCallbackFunc );
	
						~SysNavObjectFilterUPP();
						
	NavObjectFilterUPP	Get() const;
	
private:
	NavObjectFilterUPP	mNavObjectFilterUPP;
	
private:								// Forbid copy and assignment
						SysNavObjectFilterUPP( const SysNavObjectFilterUPP& );
	SysNavObjectFilterUPP&
						operator = ( const SysNavObjectFilterUPP& );
};


// ---------------------------------------------------------------------------
//	Get

inline
NavObjectFilterUPP
SysNavObjectFilterUPP::Get() const
{
	return mNavObjectFilterUPP;
}


// ===========================================================================
//	SysNavPreviewUPP
/**
	Wrapper class for a Navigation Services Preview callback function UPP */

class	SysNavPreviewUPP {
public:
	explicit			SysNavPreviewUPP( NavPreviewProcPtr inCallbackFunc );
	
						~SysNavPreviewUPP();
						
	NavPreviewUPP		Get() const;
	
private:
	NavPreviewUPP		mNavPreviewUPP;
	
private:								// Forbid copy and assignment
						SysNavPreviewUPP( const SysNavPreviewUPP& );
	SysNavPreviewUPP&	operator = ( const SysNavPreviewUPP& );
};


// ---------------------------------------------------------------------------
//	Get

inline
NavPreviewUPP
SysNavPreviewUPP::Get() const
{
	return mNavPreviewUPP;
}


} // namespace PPx

#endif	// H_PPxNavServices
