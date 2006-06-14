// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLModelTabInfo.h			© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "DMIntegerAttribute.h"
#include "DMStringAttribute.h"
#include "DMObject.h"


// ===========================================================================
//		* TCLModelTabInfo
// ===========================================================================
//	TCLModelTabInfo is the data model object for a single menu item.

class TCLModelTabInfo : public DMObject
{
public:
	enum
	{
		class_ID				= 'Tabi',
		attrKey_IconSuiteID		= 'mICN',
		attrKey_TabName			= 'mNAM'
	};

	virtual Boolean		IsDerivedFrom(FourCharCode inClassID) const;
	static DMElement*	CreateFromStream(LStream* inStream);
	virtual DMElement*	Clone() const;
	virtual DMElement*	ShallowClone() const;

	inline DMIntegerAttribute*	GetIconSuiteIDAttribute() const
								{
									return FindIntegerAttribute(attrKey_IconSuiteID);
								}
	inline SInt32		GetIconSuiteID() const
						{
							return GetIconSuiteIDAttribute()->GetIntValue();
						}
	inline void			SetIconSuiteID(SInt32 inValue) const
						{
							GetIconSuiteIDAttribute()->SetIntValue(inValue);
						}


	inline DMStringAttribute*	GetTabNameAttribute() const
								{
									return FindStringAttribute(attrKey_TabName);
								}
	inline void			GetTabName(LStr255& outValue) const
						{
							GetTabNameAttribute()->GetTextValue(outValue);
						}
	inline void			SetTabName(ConstStringPtr inValue) const
						{
							GetTabNameAttribute()->SetTextValue(inValue);
						}



	// constructor/destructor

public:
							TCLModelTabInfo(LStream* inStream);
							TCLModelTabInfo(const TCLModelTabInfo& inStream);
							TCLModelTabInfo();
	virtual					~TCLModelTabInfo();

	// accessors

	virtual void			GetDisplayableName(
									LStr255&			outTitle) const;


	// update notifications

protected:
	bool					mCopyValues;
	SInt16					mIconSuiteID;
	LStr255					mTabName;
	
	virtual void			FinishCreateSelf();

};
