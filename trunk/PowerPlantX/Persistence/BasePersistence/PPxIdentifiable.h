// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxIdentifiable.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxIdentifiable.h
	@brief		Mix-in class for objects with an Object ID
*/

#ifndef H_PPxIdentifiable
#define H_PPxIdentifiable
#pragma once

#include <PPxPrefix.h>

namespace PPx {

// ===========================================================================
/**	Mix-in class for objects with an Object ID */

class	Identifiable {
public:
						Identifiable();

	virtual				~Identifiable();
	
	void				SetID( ObjectIDT inID );
	
	ObjectIDT			GetID() const;
	
	bool				HasID( ObjectIDT inID ) const;
	
private:
	ObjectIDT			mObjectID;
};


// ---------------------------------------------------------------------------
//	Identifiable
/**
	Default constructor. Sets object ID to a default value.					*/

inline
Identifiable::Identifiable()
{
	mObjectID = objectID_None;
}


// ---------------------------------------------------------------------------
//	~Identifiable
/**
	Destructor */

inline
Identifiable::~Identifiable()
{
}


// ---------------------------------------------------------------------------
//	SetID
/**
	Sets the ID for an Identiable object
	
	@param	inID	ID number to store for the Identifiable object			*/

inline
void
Identifiable::SetID(
	ObjectIDT	inID)
{
	mObjectID = inID;
}


// ---------------------------------------------------------------------------
//	GetID
/**
	Returns the ID for an Identiable object
	
	@return the ID for an Identiable object									*/

inline
ObjectIDT
Identifiable::GetID() const
{
	return mObjectID;
}


// ---------------------------------------------------------------------------
//	HasID
/**
	Returns whether the Identiable object has the specified ID number
	
	@return whether the Identiable object has the specified ID number		*/

inline
bool
Identifiable::HasID(
	ObjectIDT	inID) const
{
	return (mObjectID == inID);
}


} // namespace PPx

#endif	// H_PPxIdentifiable
