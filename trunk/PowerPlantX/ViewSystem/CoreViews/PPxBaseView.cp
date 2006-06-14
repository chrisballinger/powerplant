// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxBaseView.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:38 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxBaseView.h>
#include <PPxSerializer.h>

namespace PPx {

namespace {	// unnamed namespace

	const CFStringRef	name_Class		= CFSTR("PPx::BaseView");
	
	const CFStringRef	key_Features	= CFSTR("features");
}


// ---------------------------------------------------------------------------
//	BaseView														  [public]
/**
	Default constructor */

BaseView::BaseView()
{
	mControlFeatures = features_None;
}


// ---------------------------------------------------------------------------
//	~BaseView														  [public]
/**
	Destructor */

BaseView::~BaseView()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initializes from parameters
	
	@param	inFrame		Bounds for view, in local coordinates of parent
	@param	inFeatures	Control features supported by this view
	
	Creates BaseView with no superview in the default state, which is
	invisible and enabled.
	
	See <Controls.h> for a list of control features. The most common one
	that you may want to specify is kControlSupportsEmbedding, which
	allows the view to have subviews.										*/

void
BaseView::Initialize(
	const HIRect&	inFrame,
	OptionBits		inFeatures)
{
	mControlFeatures = inFeatures | GetFeatureFlags();

	View::Initialize(CreateSysView(), inFrame);
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initializes from parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inFeatures		Control features supported by this view
	
	See <Controls.h> for a list of control features. The most common one
	that you may want to specify is kControlSupportsEmbedding, which
	allows the view to have subviews.										*/

void
BaseView::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	OptionBits		inFeatures)
{
	mControlFeatures = inFeatures | GetFeatureFlags();

	View::Initialize(CreateSysView(), inSuperView, inFrame,
						inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
BaseView::ClassName() const
{
	return name_Class;
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data
	
	@note A subclass should call this function from its override			*/

void
BaseView::InitState(
	const DataReader&	inReader)
{
	inReader.ReadOptional(key_Features, mControlFeatures);
	mControlFeatures |= GetFeatureFlags();

	InitViewState(CreateSysView(), inReader);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data
	
	@note A subclass should call this function from its override			*/

void
BaseView::WriteState(
	DataWriter&	ioWriter) const
{
	ioWriter.WriteValue(key_Features, mControlFeatures);

	View::WriteState(ioWriter);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetFeatureFlags												   [protected]
/**
	Returns the control feature flags for the view

	@return Feature flags for the view
	
	Subclasses should override to return the features that they support		*/

OptionBits
BaseView::GetFeatureFlags() const
{
	return 0;
}


// ---------------------------------------------------------------------------
//	CreateSysView													 [private]
/**
	Creates the underlying system HIView for this view
	
	@return	System HIViewRef used by this view								*/

HIViewRef
BaseView::CreateSysView()
{
		// Local static ensures that we register our BaseView class with
		// the Toolbox the first time we are called
		
	static	bool	isRegistered = false;
	
	if (not isRegistered) {
	
			// We always create direct subclass of HIView
		
		SysHIView::RegisterSysViewClass(name_Class, kHIViewClassID);
						
		isRegistered = true;
	}
	
	return SysHIView::CreateSysView( name_Class, mControlFeatures );
}


} // namespace PPx
