/*
 * CBOSLib: Veneers for the OS_Word SWI
 * Copyright (C) 2018 Christopher Bazley
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
  CJB: 29-Oct-18: Created this header file.
  CJB: 22-Apr-19: Added declaration of os_word_set_pointer_bbox.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
*/

#ifndef OSWord_h
#define OSWord_h

/* ISO library headers */
#include <stddef.h>
#include <stdint.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "wimp.h"

/* Local headers */
#include "OSDate.h"

_kernel_oserror *os_word_read_real_time(OS_DateAndTime * /*time*/);
   /*
    * Reads the current date and time from the CMOS clock in 5 byte UTC time
    * format. On entry 'utc' must point to an object in which to store the date
    * and time value.
    * Returns: a pointer to an OS error block, or else NULL for success. If no
    *          error occured then the current time will have been output.
    */

_kernel_oserror *os_word_set_pointer_bbox(const BBox * /*bbox*/);
   /*
    * Sets the mouse pointer bounding box in OS units relative to the
    * graphics coordinate origin.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
