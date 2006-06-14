// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLModelTabControl.h		© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "DMIntegerAttribute.h"
#include "DMListAttribute.h"
#include "DMObject.h"


// ===========================================================================
//		* TCLModelTabControl
// ===========================================================================
//	TCLModelTabControl is the data model object for a single menu item.

class TCLModelTabControl : public DMObject
{
public:
	enum
	{
		class_ID				= 'TCTC',
		attrKey_Version			= 'mVER',
		attrKey_TabInfo			= 'mTBI'
	};

	virtual Boolean		IsDerivedFrom(FourCharCode inClassID) const;
	static DMElement*	CreateFromStream(LStream* inStream);
	virtual DMElement*	Clone() const;
	virtual DMElement*	ShallowClone() const;

	inline DMIntegerAttribute*	GetVersionAttribute() const
								{
									return FindIntegerAttribute(attrKey_Version);
								}
	inline SInt32		GetVersion() const
						{
							return GetVersionAttribute()->GetIntValue();
						}
	inline void			SetVersion(SInt32 inValue) const
						{
							GetVersionAttribute()->SetIntValue(inValue);
						}


	inline DMListAttribute*	GetTabInfoAttribute() const
							{
								return dynamic_cast<DMListAttribute*>
									(FindContainerAttribute(attrKey_TabInfo));
							}
	inline DMListAttribute*	GetTabInfo() const
							{
								return GetTabInfoAttribute();
							}



	// constructor/destructor

public:
							TCLModelTabControl(LStream* inStream);
							TCLModelTabControl(const TCLModelTabControl& inStream);
	virtual					~TCLModelTabControl();

protected:

};
