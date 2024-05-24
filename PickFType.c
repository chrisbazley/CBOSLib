/*
 * CBOSLib: Function to pick a RISC OS file type
 * Copyright (C) 2019 Christopher Bazley
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
  CJB: 20-Oct-19: Created this source file.
*/

/* ISO library headers */
#include <stddef.h>
#include <stdlib.h>

/* Local headers */
#include "FileTypes.h"
#include "Internal/CBMisc.h"

int pick_file_type(const int *const rec_file_types,
  const int *const send_file_types)
{
  /* Search receiver's list of acceptable file types for one that is also
     on the sender's list. Both lists are in order of preference. */
  assert(rec_file_types != NULL);
  assert(send_file_types != NULL);

  int file_type = common_file_type(rec_file_types, send_file_types);
  if (file_type == FileType_Null)
  {
    file_type = send_file_types[0];
    DEBUGF("PickFType: Can't produce receiver's file types (using 0x%x)\n",
           file_type);
  }

  return file_type;
}
