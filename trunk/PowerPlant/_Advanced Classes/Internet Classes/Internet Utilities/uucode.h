// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*                                                                   UU Encoding and Decoding
                             ENCODING TO PRINTABLE CHARACTERS

 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   File module provides functions HTUU_encode() and HTUU_decode() which convert a buffer
   of bytes to/from RFC 1113 printable encoding format. This technique is similar to the
   familiar Unix uuencode format in that it maps 6 binary bits to one ASCII character (or
   more aptly, 3 binary bytes to 4 ASCII characters).  However, RFC 1113 does not use the
   same mapping to printable characters as uuencode.

   This module is implemented by HTUU.c, and it is a part of the Library of Common Code.

 */
#ifndef HTUU_H
#define HTUU_H

#ifdef __cplusplus
    extern "C" {
#endif

extern int HTUU_encode (unsigned char *bufin,
                               unsigned int nbytes,
                               char *bufcoded);

extern int HTUU_decode (char *bufcoded,
                               unsigned char *bufplain,
                               int outbufsize);

#ifdef __cplusplus
    }
#endif

#endif/*

   End of file. */