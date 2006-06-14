// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

const SInt32	err_FirstError				=	9000;
const SInt32	err_LastError				=	9099;		// can't go higher than this


const SInt32	err_Generic					= 	9000;	

const SInt32	err_InvalidImageDepth		=	9001;
const SInt32	err_InvalidImageFormat		=	9002;
const SInt32	err_InvalidImageSize		=	9003;

const SInt32	err_CorruptedResource		=	9004;
const SInt32	err_CorruptedFile			=	9005;
const SInt32	err_UnknownDataType			=	9006;

	// we map PP's errors to our own number for display
const SInt32	err_AssertionProxy			=	9007;
const SInt32	err_NilPointerProxy			=	9008;

	// these are used for error string mapping (ie, they're in the STR# resource)
	// be sure to change the 'Emap' resource if you change these numbers
const SInt32	err_OutOfMemory				= 	9009;
const SInt32	err_ResourceNotFound		=	9010;
const SInt32	err_DiskFull				=	9011;
const SInt32	err_FileNotFound			=	9012;
const SInt32	err_IOError					=	9013;

	// this is a string passed to sprintf for an invalid resize value
const SInt32	err_ResizeFormatString		=	9014;

const SInt32	err_FileLocked				=	9015;
const SInt32	err_BadFontSize				=	9016;
