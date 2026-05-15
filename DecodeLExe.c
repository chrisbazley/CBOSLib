/*
 * CBOSLib: Decode the load and execution addresses of an object
 * Copyright (C) 2012 Christopher Bazley
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
  CJB: 08-Apr-12: Created this source file.
  CJB: 10-May-25: Make the output of decode_load_exec optional.
  CJB: 15-May-26: Use intptr_t instead of int for addresses.
 */

/* ISO library headers */
#include <string.h>
#include <inttypes.h>
#include <limits.h>

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "FileTypes.h"
#include "OSFile.h"

enum
{
  LoadAddressHasTypeMask  = 0xfff,
  LoadAddressHasTypeShift = 20,
  LoadAddressTypeMask     = 0xfff,
  LoadAddressTypeShift    = 8,
  LoadAddressStampMask    = 0xff,
  LoadAddressStampShift   = 0
};

int decode_load_exec(intptr_t load, intptr_t exec, _Optional OS_DateAndTime *time)
{
  int file_type;
  const uintptr_t has_type = ((uintptr_t)load >> LoadAddressHasTypeShift) &
                             LoadAddressHasTypeMask;

  if (has_type == LoadAddressHasTypeMask)
  {
    /* Object has date stamp and file type */
    const uintptr_t type_bits = ((uintptr_t)load >> LoadAddressTypeShift) & LoadAddressTypeMask;
    DEBUGF("DateStamp: File's type is 0x%" PRIxPTR "\n", type_bits);
    assert(type_bits <= (unsigned)INT_MAX);
    file_type = (int)type_bits;

    if (time != NULL)
    {
      const uintptr_t stamp_bits = ((uintptr_t)load >> LoadAddressStampShift) & LoadAddressStampMask;
      memcpy(&time->bytes, &exec, 4);
      time->bytes[4] = stamp_bits;
      DEBUGF("DateStamp: File's date stamp is 0x%2" PRIxPTR "%8" PRIxPTR "\n", stamp_bits, exec);
    }
  }
  else
  {
    /* Object is unstamped and untyped */
    file_type = FileType_None; /* unlike OS_FSControl 38 */

    if (time != NULL)
    {
      memset(time, 0, sizeof(time->bytes));
    }
    DEBUGF("DateStamp: File is untyped\n");
  }

  return file_type;
}
