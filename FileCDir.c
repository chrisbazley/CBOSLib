/*
 * CBOSLib: Veneer for SWI OS_File 8
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

/* History:
  CJB: 08-Nov-14: Created this source file
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
 */

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSFile.h"

/* Constant numeric values */
enum
{
  OS_File_CreateDirectory = 8
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *os_file_create_dir(const char *f, int n)
{
  _Optional _kernel_oserror *e;
  _kernel_osfile_block kosfb;

  assert(f != NULL);
  assert(n >= 0);
  DEBUGF("FileCDir: about to create directory '%s' with %d entries\n", f, n);

  kosfb.start = n;
  if (_kernel_osfile(OS_File_CreateDirectory, f, &kosfb) == _kernel_ERROR) {
    e = _kernel_last_oserror();
    assert(e != NULL);
    DEBUGF("FileCDir: _kernel_osfile set error %d:%s\n",
           e->errnum, e->errmess);
  } else {
    e = NULL;
  }

  return e;
}
