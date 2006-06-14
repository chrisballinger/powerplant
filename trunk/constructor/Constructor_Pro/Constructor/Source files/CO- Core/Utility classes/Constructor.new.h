// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	Constructor.new.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//	Implementation of operator new/delete
//
//   Created: 06/08/95
//     $Date: 2006/01/18 01:33:26 $
//	$History: Constructor.new.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/30/97   Time: 13:11
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed operator new with placement so that it comples properly in
//	non-debug builds.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:10
//	Updated in $/Constructor/Source files/Utility classes
//	Improved low-memory handling.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

void BetterDebugNewErrorHandler(short err);