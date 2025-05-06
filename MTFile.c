/*
 * CBOSLib: Veneers for MessageTrans file SWIs
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
  CJB: 21-Oct-09: Created this source file.
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 31-Jan-16: Substituted _kernel_swi for _swix because it's easier to
                  intercept for stress testing.
  CJB: 18-Apr-16: Cast pointer parameters to void * to match %p.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"
#include "toolbox.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "MessTrans.h"

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *messagetrans_file_info(const char   *filename,
                                        unsigned int *flags,
                                        size_t       *buff_size)
{
  _Optional _kernel_oserror *e;
  _kernel_swi_regs regs;

  assert(filename != NULL);

  DEBUGF("MTFile: Getting message file info for path '%s'\n", filename);
  regs.r[1] = (int)filename;
  e = _kernel_swi(MessageTrans_FileInfo, &regs, &regs);
  if (e == NULL)
  {
    if (flags != NULL)
      *flags = regs.r[0];

    if (buff_size != NULL)
      *buff_size = regs.r[2];
  }
  else
  {
    DEBUGF("MTFile: SWI error 0x%x '%s'\n", e->errnum, e->errmess);
  }
  return e;
}

/* ----------------------------------------------------------------------- */

_Optional _kernel_oserror *messagetrans_open_file(MessagesFD     *mfd,
                                                  const char     *filename,
                                                  _Optional void *buffer)
{
  _Optional _kernel_oserror *e;
  _kernel_swi_regs regs;

  assert(mfd != NULL);
  assert(filename != NULL);

  DEBUGF("MTFile: Opening message file %p at path '%s' with buffer %p\n",
         (void *)mfd, filename, buffer);

  regs.r[0] = (int)mfd;
  regs.r[1] = (int)filename;
  regs.r[2] = buffer ? (int)buffer : 0;
  e = _kernel_swi(MessageTrans_OpenFile, &regs, &regs);
  if (e != NULL)
  {
    DEBUGF("MTFile: SWI error 0x%x '%s'\n", e->errnum, e->errmess);
  }
  return e;
}

/* ----------------------------------------------------------------------- */

_Optional _kernel_oserror *messagetrans_close_file(MessagesFD *mfd)
{
  _Optional _kernel_oserror *e;
  _kernel_swi_regs regs;

  assert(mfd != NULL);

  DEBUGF("MTFile: Closing message file %p\n", (void *)mfd);
  regs.r[0] = (int)mfd;
  e = _kernel_swi(MessageTrans_CloseFile, &regs, &regs);
  if (e != NULL)
  {
    DEBUGF("MTFile: SWI error 0x%x '%s'\n", e->errnum, e->errmess);
  }
  return e;
}
