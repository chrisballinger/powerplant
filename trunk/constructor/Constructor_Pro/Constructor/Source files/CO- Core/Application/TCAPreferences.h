/*****************************************************************************/
/*																			 */
/*	© 2011, Aurbach & Associates, Inc.  All rights reserved.				 */
/*																			 */
/*	Redistribution and use in source and binary forms, with or without		 */
/*	modification, are permitted provided that the following condition		 */
/*	are met:																 */
/*																			 */
/*	  *	Redistributions of source code must retain the above copyright		 */
/*		notice, this list of conditions and the following disclaimer.		 */
/*																			 */
/*	  *	Redistributions in binary form must reproduce the above copyright	 */
/*		notice, this list of conditions and the following disclaimer in the	 */
/*		documentation and/or other materials provided with the distribution. */
/*																			 */
/*	  *	Neither the name of Aurbach & Associates, Inc. nor the names of any	 */
/*		of its employees may be used to endorse or promote products derived	 */
/*		from this software without specific prior written permission.		 */
/*																			 */
/*	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		 */
/*	ÒAS ISÓ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		 */
/*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A	 */
/*	PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER */
/*	OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, */
/*	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,		 */
/*	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR		 */
/*	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF	 */
/*	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING	 */
/*	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS		 */
/*	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.			 */
/*																			 */
/*****************************************************************************/
/*
 *	Class Name:			TCAPreferences
 *
 *	Change History:
 *
 *	Who     	Date	    Description
 * ---------------------------------------------------------------------------
 *	RLA		09-Oct-2006		Original Code (UCFPreferences.h)
 *	RLA		23-Aug-2011		Converted for use in the Constructor environment
 */

#pragma once

#if !defined(__MACH__)
	#include <CFPreferences.h>
#endif

#include	"CAPreferences.h"

namespace	TCAPreferences		{

	template <typename T>
	UInt32				GetValue (
							CFStringRef						inKey,
							T &								inBlock )
							{
								return CAPreferences::GetValueAsBlock(inKey, &inBlock, sizeof(T));
							}
	
	template <typename T>
	void				SetValue (
							CFStringRef						inKey,
							const T &						inBlock )
							{
								return CAPreferences::SetValueAsBlock(inKey, &inBlock, sizeof(T));
							}
}
