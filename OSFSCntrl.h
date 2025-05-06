/*
 * CBOSLib: Veneers for filing system control SWIs
 * Copyright (C) 2009 Christopher Bazley
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

/*
Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 12-Oct-09: Created this header file.
  CJB: 14-Mar-19: Added the os_fscontrol_copy function.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

#ifndef OSFSCntrl_h
#define OSFSCntrl_h

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

#if !defined(USE_OPTIONAL) && !defined(_Optional)
#define _Optional
#endif

_Optional _kernel_oserror *os_fscontrol_canonicalise(
  _Optional char * /*buffer*/, size_t /*buff_size*/,
  _Optional const char * /*pv*/, _Optional const char * /*ps*/,
  const char * /*f*/, _Optional size_t * /*nbytes*/);
   /*
    * Converts 'f' (which may contain wildcards or references to environment
    * variables) into a canonical file path from the root directory of a filing
    * system. Up to buff_size-1 characters of the result string will be copied
    * to the 'buffer' array and a NUL terminator appended. If 'buff_size' is 0
    * then nothing will be written and 'buffer' may be a null pointer. 'pv'
    * should point to the name of an environment variable containing a comma-
    * separated path, or else a null pointer to use no path. 'ps' should point
    * to a path string to be used if 'pv' is null or the specified path variable
    * does not exist. Unless 'nbytes' is a null pointer, the integer to which
    * it points will be set to the number of bytes that would have been output
    * had a large enough buffer been provided.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

/* Flags for use with the os_fscontrol_copy function */
#define OS_FSControl_Recurse   (1u << 0)
#define OS_FSControl_Force     (1u << 1)
#define OS_FSControl_Confirm   (1u << 3)
#define OS_FSControl_Verbose   (1u << 4)
#define OS_FSControl_Quick     (1u << 5)
#define OS_FSControl_Prompt    (1u << 6)
#define OS_FSControl_Delete    (1u << 7)
#define OS_FSControl_Print     (1u << 8)
#define OS_FSControl_Access    (1u << 9)
#define OS_FSControl_Stamp     (1u << 10)
#define OS_FSControl_Structure (1u << 11)
#define OS_FSControl_Newer     (1u << 12)
#define OS_FSControl_Look      (1u << 14)

_Optional _kernel_oserror *os_fscontrol_copy(const char * /*src*/,
  const char * /*dst*/, unsigned int /*flags*/);
   /*
    * Copy files conforming to a given wildcarded filename to a destination
    * filename. According to the flags, the operation may recurse and source
    * files may be deleted after being copied.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
