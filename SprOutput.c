/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 60
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
  CJB: 16-Mar-19: Created this source file.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSSpriteOp.h"
#include "MessTrans.h"

enum
{
  DUMMY_ERRNO = 255
};

static _kernel_oserror *switch_output_common(SpriteAreaHeader *const area,
  const char *const name, void *const buffer, size_t const buff_size,
  size_t *const nbytes, SpriteRestoreOutputBlock *const output,
  int const reason)
{
  assert(area != NULL);
  assert(buffer != NULL || buff_size == 0);
  DEBUGF("SprOutput: Switching output to %s '%s' in area %p "
         "with save area %p of size %zu (output size to %p)\n",
         reason == SPRITEOP_SWITCHOUTPUT_SPRITE ? "sprite" : "mask",
         name ? name : "<screen>", (void *)area,
         buffer, buff_size, (void *)nbytes);

  /* Find the buffer size required for the save area */
  _kernel_swi_regs regs;
  regs.r[0] = SPRITEOP_USERAREA_SPRNAME + SPRITEOP_READ_SAVE_AREA_SIZE;
  regs.r[1] = (int)area;
  regs.r[2] = name ? (int)name : 0;

  _kernel_oserror *e = _kernel_swi(OS_SpriteOp, &regs, &regs);

  if (e == NULL)
  {
    assert(regs.r[3] >= 0);
    size_t const buff_req = regs.r[3];
    DEBUGF("SprOutput: Buffer requirement is %zu\n", buff_req);
    if (nbytes != NULL)
    {
      *nbytes =  buff_req;
    }

    if (buffer != NULL)
    {
      /* The SWI doesn't detect buffer overflows but since this function
         does it should return an error in those circumstances */
      if (buff_size < buff_req)
      {
        DEBUGF("SprOutput: Save area is too small\n");
        e = messagetrans_error_lookup(NULL, DUMMY_ERRNO, "BufOFlo", 0);
      }
      else
      {
        DEBUGF("SprOutput: Switching output\n");
        assert(buffer != NULL);
        regs.r[0] = reason + (area ? SPRITEOP_USERAREA_SPRNAME :
                                     SPRITEOP_SYSTEMAREA);
        regs.r[1] = (int)area;
        regs.r[2] = name ? (int)name : 0;
        regs.r[3] = (int)buffer;
        e = _kernel_swi(OS_SpriteOp, &regs, &regs);
        if (e != NULL)
        {
          DEBUGF("SprOutput: SWI error 0x%x '%s' (1)\n",
                 e->errnum, e->errmess);
        }
        else if (output != NULL)
        {
          output->reason = regs.r[0];
          output->area = (SpriteAreaHeader *)regs.r[1];
          output->name_or_addr = (void *)regs.r[2];
          output->buffer = (void *)regs.r[3];

          DEBUGF("SprOutput: Outputting restore parameters %d,%p,%p,%p\n",
                 output->reason, (void *)output->area, output->name_or_addr,
                 output->buffer);
        }
      }
    }
    else
    {
      DEBUGF("SprOutput: No save area was supplied\n");
    }
  }
  else
  {
    DEBUGF("SprOutput: SWI error 0x%x '%s' (1)\n", e->errnum, e->errmess);
  }
  return e;
}

_kernel_oserror *os_sprite_op_output_to_mask(SpriteAreaHeader *const area,
  const char *const name, void *const buffer, size_t const buff_size,
  size_t *const nbytes, SpriteRestoreOutputBlock *const output)
{
  return switch_output_common(area, name, buffer, buff_size, nbytes,
                              output, SPRITEOP_SWITCHOUTPUT_MASK);
}

_kernel_oserror *os_sprite_op_output_to_sprite(SpriteAreaHeader *const area,
  const char *const name, void *const buffer, size_t const buff_size,
  size_t *const nbytes, SpriteRestoreOutputBlock *const output)
{
  return switch_output_common(area, name, buffer, buff_size, nbytes,
                              output, SPRITEOP_SWITCHOUTPUT_SPRITE);
}
