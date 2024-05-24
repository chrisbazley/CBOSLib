/*
 * CBOSLib: Veneers for file and catalogue read/write SWIs
 * Copyright (C) 2014 Christopher Bazley
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* OSFSCntrl.h currently declares one function to canonicalise file paths.

Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 23-Nov-14: Created this header file.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
*/

#ifndef OSGBPB_h
#define OSGBPB_h

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

/* Local headers */
#include "OSFile.h"

/* Constant for use with the os_gbpb_read_cat_no_path function */
enum
{
  OS_GBPB_ReadCat_PositionStart = 0,
  OS_GBPB_ReadCat_PositionEnd = -1
};

/* Catalogue information for a filing system object (OS_GBPB 10 format).
   The first four members are equivalent to a _kernel_osfile_block and are
   typed accordingly. */
typedef struct
{
  OS_File_CatalogueInfo info;
  char name[];      /* Variable-length buffer for nul-terminated leaf name. */
}
OS_GBPB_CatalogueInfo;

_kernel_oserror *os_gbpb_read_cat_no_path(const char */*f*/, void */*buffer*/, size_t /*buff_size*/, unsigned int */*n*/, int */*pos*/, const char */*pattern*/);
   /*
    * Reads catalogue information for up to 'n' objects from the directory
    * named 'f', starting at 'pos' (or OS_GBPB_ReadCat_PositionStart for
    * the first call). Upon return from this function, 'n' and 'pos' are
    * updated to reflect the number of objects actually read and the
    * position from which to continue reading (or OS_GBPB_ReadCat_PositionEnd
    * if there are no more matching catalogue entries). Only objects with
    * names that match the wildcarded string 'pattern' will be included.
    * If 'pattern' is a null pointer or "*" then all names will match.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
