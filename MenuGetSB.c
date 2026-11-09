/*
 * CBOSLib: Toolbox string-buffer getters
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
  CJB: 09-Nov-26: Added the string-buffer getter veneer.
*/

#include <stddef.h>

/* Acorn C/C++ library headers */
#include "menu.h"

/* Local headers */
#include "TboxExtra.h"
#include "MessTrans.h"
#include "Internal/CBOSMisc.h"

enum { DUMMY_ERRNO = 255 };

_Optional _kernel_oserror *menu_get_entry_text_sb(unsigned int flags, ObjectId object, ComponentId component,
  strb_t *output)
{
  assert(output != NULL);
  int size;
  _Optional _kernel_oserror *e =
    menu_get_entry_text(flags, object, component, NULL, 0, &size);
  if (e == NULL)
  {
    assert(size > 0);
    _Optional char *const buffer = strb_write(output, (size_t)size - 1);
    if (buffer)
    {
      e = menu_get_entry_text(flags, object, component, &*buffer, size, NULL);
      strb_restore(output);
    }
    else
    {
      e = messagetrans_error_lookup(NULL, DUMMY_ERRNO, "BufOFlo", 0);
    }
  }
  return e;
}
