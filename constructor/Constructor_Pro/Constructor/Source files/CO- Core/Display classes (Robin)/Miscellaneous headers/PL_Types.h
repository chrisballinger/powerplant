// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					PL_Types.h

CLASSES:				

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.01.22

CHANGE HISTORY :

		96.01.22		rtm	Initial creation of file
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once


//==================================================================================== 
//	TYPES
//==================================================================================== 

//	This type is used to define IDs that are OSTypes for pages, it is used in
// conjunction with the page controlled and its associated SPagePec struct

typedef		Int32			PageIDT;		

/*
// ¥ The various modes supported by the Mode Control, these modes determine how the
// control responds to a mouse click

enum EControlMode
{
	controlMode_None,
	controlMode_Button,
	controlMode_RadioButton,
	controlMode_Switch
};


// ¥ Used by the GIconMixin class and any of the classes it is mixed into, it
// defines a number of constants for the various positions an icon can be placed
// within its pane

enum EIconPosition
{
	iconPosition_None,
	iconPosition_TopLeft,
	iconPosition_TopCenter,
	iconPosition_TopRight,
	iconPosition_RightCenter,
	iconPosition_BottomRight,
	iconPosition_BottomCenter,
	iconPosition_BottomLeft,
	iconPosition_LeftCenter,
	iconPosition_Center
};

// ¥ Constants for the various size supported by the icon suite class

enum ESizeSelector
{
	sizeSelector_None,
	sizeSelector_LargeIconSize = 32,
	sizeSelector_SmallIconSize = 16,
	sizeSelector_MiniIconSize = 12
};
*/

// ¥ Constants for the scrolling directions within the page controller

enum EScrollDirection
{
	scrollDirection_None,
	scrollDirection_Left,
	scrollDirection_Right
};


