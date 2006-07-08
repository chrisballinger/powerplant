// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxStreamUtils.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxStreamUtils.h>

using namespace std;

// ---------------------------------------------------------------------------
//	operator << ( ConstStringPtr )
/**
	Writes a Pascal string to an output stream
	
	@param	inStream		Output stream
	@param	inPascalStr		String to write
	
	@return	Reference to output stream object								*/

ostream&
operator << (
	ostream&		inStream,
	ConstStringPtr	inPascalStr)
{
	ostream::char_type *charPtr = (ostream::char_type *)&inPascalStr[1];
	streamsize byteCount = (streamsize)inPascalStr[0];

	return inStream.write(charPtr, byteCount);
}


// ---------------------------------------------------------------------------
//	operator << ( CFStringRef )
/**
	Writes a CFStringRef to an output stream
	
	@param	inStream		Output stream
	@param	inCFString		String to write
	
	@return	Reference to output stream object								*/

ostream&
operator << (
	ostream&		inStream,
	CFStringRef		inCFString)
{
	// $ Enhancement $ Try to get C string ptr first


	CFIndex stringLength = ::CFStringGetLength(inCFString);
	if (stringLength > 0)
	{
		CFRange allRange = ::CFRangeMake(0, stringLength);

		CFIndex usedBufferLength;
		stringLength = ::CFStringGetBytes(inCFString, allRange,
				kCFStringEncodingUTF8, 0, false, nil, 0, &usedBufferLength);

		if (stringLength > 0)
		{
			UInt8 *buffer = new UInt8[stringLength + 1];

			try
			{
				stringLength = ::CFStringGetBytes(inCFString, allRange,
									kCFStringEncodingUTF8, 0, false, buffer,
									stringLength, &usedBufferLength);
				buffer[stringLength] = 0;

				inStream << (const char *)buffer;
			}
			catch (...)
			{
				delete[] buffer;
				throw;
			}

			delete[] buffer;
		}
	}

	return inStream;
}


// ---------------------------------------------------------------------------
//	operator << ( Point )
/**
	Writes a Point struct to an output stream
	
	@param	inStream	Output stream
	@param	inPoint		Point to write
	
	@return	Reference to output stream object								*/

ostream&
operator << (
	ostream&	inStream,
	Point		inPoint)
{
	inStream << "(h: " << inPoint.h << ", ";
	inStream << "v: " << inPoint.v << ")";

	return inStream;
}


// ---------------------------------------------------------------------------
//	operator << ( const Rect& )
/**
	Writes a Rect struct to an output stream
	
	@param	inStream	Output stream
	@param	inRect		Rect to write
	
	@return	Reference to output stream object								*/

ostream&
operator << (
	ostream&		inStream,
	const Rect&		inRect)
{
	inStream << "(top: " << inRect.top << ", ";
	inStream << "left: " << inRect.left << ", ";
	inStream << "bottom: " << inRect.bottom << ", ";
	inStream << "right: " << inRect.right << ")";

	return inStream;
}


namespace PPx {

namespace StreamUtils {

// ---------------------------------------------------------------------------
//	WriteLinesOfText
/**
	Writes lines of text to an output stream
	
	@param	inOutputStream	Output stream
	@param	inTextPtr		Pointer to text
	@param	inTextLength	Lenght of text
	@param	inLineEndChar	Character used for line endings in text
	
	Lines of text end with an inLineEndChar. Text for a line is streamed,
	followed by a std::endl manipulator, which writes a new line and
	flushes the stream.
	
	This function is useful when writing large blocks of text to a
	buffered stream. Writing the whole block of text at once could
	exceed the buffer capacity, resulting in lost characters.				*/

void
WriteLinesOfText(
	std::ostream&		inOutputStream,
	const char*			inTextPtr,
	long				inTextLength,
	char				inLineEndChar)
{
	long		lineStart = 0;
	long		pos = 0;
	
	while (lineStart < inTextLength) {
	
		while (pos < inTextLength) {
			if ( *(inTextPtr + pos++) == inLineEndChar) {
				break;
			}
		}
		
		if (pos == inTextLength) ++pos;
		
		inOutputStream.write((inTextPtr + lineStart), (pos - lineStart - 1))
																<< std::endl;
		lineStart = pos;
	}
	
	inOutputStream << std::endl;
}


} // namespace StreamUtils

} // namespace PPx