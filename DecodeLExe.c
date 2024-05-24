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
 */

/* ISO library headers */
#include <string.h>

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

int decode_load_exec(int load, int exec, OS_DateAndTime *time)
{
  int file_type;
  const int has_type = (load >> LoadAddressHasTypeShift) &
                       LoadAddressHasTypeMask;

  if (has_type == LoadAddressHasTypeMask)
  {
    /* Object has date stamp and file type */
    file_type = (load >> LoadAddressTypeShift) & LoadAddressTypeMask;
    DEBUGF("DateStamp: File's type is 0x%x\n", file_type);

    if (time != NULL)
    {
      const int st = (load >> LoadAddressStampShift) & LoadAddressStampMask;
      memcpy(&time->bytes, &exec, 4);
      time->bytes[4] = st;
      DEBUGF("DateStamp: File's date stamp is 0x%2x%8x\n", st, exec);
    }
  }
  else
  {
    /* Object is unstamped and untyped */
    file_type = FileType_None; /* unlike OS_FSControl 38 */

    if (time != NULL)
      memset(time, 0, sizeof(time->bytes));

    DEBUGF("DateStamp: File is untyped\n");
  }

  return file_type;
}
