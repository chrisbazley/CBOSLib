/*
 * CBOSLib: Veneer for SWI OS_File 17
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
  CJB: 23-Nov-14: Created this source file
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
  OS_File_ReadCatalogueEntryNoPath = 17
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *os_file_read_cat_no_path(const char *f, OS_File_CatalogueInfo *catalogue_entry)
{
  _Optional _kernel_oserror *e;
  _kernel_osfile_block kosfb;

  assert(f != NULL);
  assert(catalogue_entry != NULL);
  DEBUGF("FileRCat: about to read catalogue entry for '%s'\n", f);

  catalogue_entry->object_type = _kernel_osfile(OS_File_ReadCatalogueEntryNoPath, f, &kosfb);

  if (catalogue_entry->object_type == _kernel_ERROR) {
    e = _kernel_last_oserror();
    assert(e != NULL);
    DEBUGF("FileRCat: _kernel_osfile set error %d:%s\n", e->errnum, e->errmess);
  } else {
    e = NULL;
    catalogue_entry->load = kosfb.load;
    catalogue_entry->exec = kosfb.exec;
    catalogue_entry->length = kosfb.start;
    catalogue_entry->attributes = kosfb.end;
  }

  return e;
}
