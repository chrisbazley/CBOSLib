/*
 * CBOSLib: Veneer for SWI OS_File 11
 * Copyright (C) 2026 Christopher Bazley
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
  CJB: 21-Sep-26: Created this source file.
 */

/* ISO library headers */
#include <stddef.h>
#include <stdint.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSFile.h"

/* Constant numeric values */
enum
{
  OS_File_CreateStampedFile = 11
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *os_file_create_stamped(const char *f,
                                                   int type,
                                                   long int length)
{
  _Optional _kernel_oserror *e;

  assert(f != NULL);
  assert(type >= 0);
  assert(type <= 0xfff);
  assert(length >= 0);
  assert(length <= INTPTR_MAX);
  DEBUGF("FileCStmp: about to create stamped file '%s' of type 0x%x "
         "and length %ld\n", f, type, length);

  _kernel_osfile_block kosfb = {
    .load = (intptr_t)type,
    .start = 0,
    .end = (intptr_t)length,
  };
  if (_kernel_osfile(OS_File_CreateStampedFile, f, &kosfb) == _kernel_ERROR) {
    e = _kernel_last_oserror();
    assert(e != NULL);
    DEBUGF("FileCStmp: _kernel_osfile set error %d:%s\n",
           e->errnum, e->errmess);
  } else {
    e = NULL;
  }

  return e;
}
