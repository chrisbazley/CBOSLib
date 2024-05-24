/*
 * CBOSLib: Read the real-time clock
 * Copyright (C) 2004 Christopher Bazley
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

/* History:
  CJB: 14-Oct-09: Split this source file from c.DateStamp, added assertions
                  and replaced 'magic' values with named constants.
  CJB: 05-May-12: Added support for stress-testing failure of _kernel_osword.
                  Updated for change to definition of type 'OS_DateAndTime'.
                  Error pointer returned by get_current_time is no longer
                  const-qualified if CBLIB_OBSOLETE is defined.
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 11-Dec-20: Remove redundant use of the 'extern' keyword.
 */

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSWord.h"

/* Constant numeric values */
enum
{
  OS_Word_ReadCMOSClock  = 14, /* _kernel_osword reason code */
  ReadCMOSClockAsInteger = 3   /* sub-reason code in parameter block */
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_kernel_oserror *os_word_read_real_time(OS_DateAndTime *time)
{
  _kernel_oserror *e = NULL;

  assert(time != NULL);

   /* Read soft-copy of CMOS clock as 5-byte integer */
  time->bytes[0] = ReadCMOSClockAsInteger;
  if (_kernel_osword(OS_Word_ReadCMOSClock, (int *)time->bytes) == _kernel_ERROR)
  {
    e = _kernel_last_oserror();
    assert(e != NULL);
    DEBUGF("ReadClock: SWI returned error 0x%x '%s'\n", e->errnum, e->errmess);
  }
  else
  {
    DEBUGF("ReadClock: Current date and time is 0x%x%x%x%x%x\n",
           time->bytes[4], time->bytes[3], time->bytes[2], time->bytes[1],
           time->bytes[0]);
  }
  return e;
}
