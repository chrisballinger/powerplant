// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

/*
	Error Display Strategy:
	
	¥ Two routine display error dialogs:	DisplayError(), DisplayErrorString()
	
	¥ Both may display an optional 2nd string at the dialog bottom. This will
		be set to the error number if a 2nd string isn't specified.

	¥ Bitmap errors are numbered between 9000-9099. Some common Mac errors
		are mapped to our error numbers via an 'EMap' resource. Each error
		has a corresponding entry in the error STR# resource.
		
	¥ The following resources are used (the id values must match each other):
		'PPob'	9020		Error dialog
		'EMap'	9020		Maps Mac error numbers to our error numbers
		'STR#'	9020		Error strings (1..n)

	¥ Note:
		It would be safer to allocate the error dialog ahead of time and
		just make it visible as needed. Since we are a plug-in, however,
		this would be a bit harder to do. (???)
*/

#pragma once

#include "SU_Types.h"

class SUErrors
{
	public:
		static void			Initialize( ResFileIDT inFileID, ResIDT inMapID );
	
		static void			DisplayError( SInt32 inError, const char *addString = nil );
		static void			DisplayErrorString( const char *inString, const char *addString = nil );
		
		static void			DisplayPostponedError( SInt32 inError, const char *addString = nil );
		
		static Boolean		ConvertErrorNumberToString( SInt32 inError, char *outString1, char *outString2 );

	protected:
		static void			ChangeCaption( LWindow *, PaneIDT, const char * );
		static void			InternalInit();
};

