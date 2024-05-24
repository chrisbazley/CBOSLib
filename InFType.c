/*
 * CBOSLib: Function to search for a RISC OS file type
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
  CJB: 26-Oct-19: Created this source file.
*/

/* ISO library headers */
#include <stddef.h>
#include <stdlib.h>

/* Local headers */
#include "FileTypes.h"
#include "Internal/CBMisc.h"

bool in_file_types(int const file_type, const int *const file_types)
{
  assert(file_types != NULL);

  for (size_t i = 0; file_types[i] != FileType_Null; i++)
  {
    DEBUGF("InFType: Checking file type 0x%x\n", file_types[i]);
    if (file_type == file_types[i])
    {
      DEBUGF("InFType: Matched file type\n");
      return true;
    }
  }

  DEBUGF("InFType: No match with file type 0x%x\n", file_type);
  return false;
}
