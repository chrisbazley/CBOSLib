/*
 * CBOSLib: Veneer for SWI OS_File 18
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

/* History:
  CJB: 29-May-16: Created this source file based on SetFType.c.
 */

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSFile.h"

/* Constant numeric values */
enum
{
  OS_File_SetTypeOfNamedObject = 18
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_kernel_oserror *os_file_set_type(const char *f, int type)
{
  _kernel_oserror *e = NULL;
  _kernel_osfile_block os_file;

  assert(f != NULL);
  DEBUGF("FileSType: Setting type of file '%s' to 0x%x\n", f, type);

  os_file.load = type;
  if (_kernel_osfile(OS_File_SetTypeOfNamedObject, f, &os_file) == _kernel_ERROR)
     e = _kernel_last_oserror();

  return e;
}
