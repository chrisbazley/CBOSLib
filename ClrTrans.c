/*
 * CBOSLib: Veneers for colour translation SWIs
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
  CJB: 02-Oct-09: Created this source file.
  CJB: 05-May-12: Added support for stress-testing failure of _kernel_swi.
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 21-Apr-16: Modified format strings to avoid GNU C compiler warnings.
  CJB: 17-Mar-19: Reduced the scope of buff_req.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "PalEntry.h"
#include "MessTrans.h"
#include "ClrTrans.h"

/* Miscellaneous numeric constants */
enum
{
  ErrorNum_BufferTooSmall    = 0xa06,
  SpriteAreaPointerThreshold = 256,
  DUMMY_ERRNO = 255
};

/* ----------------------------------------------------------------------- */
/*                       Function prototypes                               */

static void assign_regs(int regs[], const ColourTransContext *context);

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *colourtrans_read_palette(
                                          unsigned int              flags,
                                          const ColourTransContext *source,
                                          _Optional PaletteEntry   *buffer,
                                          size_t                    buff_size,
                                          _Optional size_t         *nbytes)
{
  _Optional _kernel_oserror *e = NULL;
  _kernel_swi_regs regs;

  assert(source != NULL);

  if (!buffer)
  {
    buff_size = 0;
  }

  assign_regs(&regs.r[0], source);

  /* Find buffer size and/or read palette into caller's buffer */
  regs.r[2] = buffer ? (int)buffer : 0;
  regs.r[3] = buff_size;
  regs.r[4] = flags;
  DEBUGF("ClrTrans: Calling ColourTrans_ReadPalette with "
         "0x%x,0x%x,0x%x,0x%x,0x%x\n",
         regs.r[0], regs.r[1], regs.r[2], regs.r[3], regs.r[4]);

  e = _kernel_swi(ColourTrans_ReadPalette, &regs, &regs);

  DEBUGF("ClrTrans: %s is %d bytes\n",
         buffer == NULL ? "required buffer size" : "unused space",
         regs.r[3]);

  /* Remaining space is still returned with a 'buffer too small' error */
  if (e == NULL || e->errnum == ErrorNum_BufferTooSmall)
  {
    /* Output the required buffer size */
    size_t req_size;
    if (buffer == NULL)
    {
      assert(regs.r[3] >= 0);
      req_size = regs.r[3]; /* SWI returned the required buffer size */
    }
    else
    {
      req_size = buff_size - regs.r[3]; /* SWI returned remaining space */
    }
    DEBUGF("ClrTrans: buffer requirement is %zu\n", req_size);
    if (nbytes != NULL)
      *nbytes = req_size;
  }
  return e;
}

/* ----------------------------------------------------------------------- */

_Optional _kernel_oserror *colourtrans_generate_table(
                               unsigned int                         flags,
                               const ColourTransGenerateTableBlock *block,
                               _Optional void                      *buffer,
                               size_t                               buff_size,
                               _Optional size_t                    *nbytes)
{
  _Optional _kernel_oserror *e = NULL;
  _kernel_swi_regs regs;

  assert(block != NULL);
  assert(buffer != NULL || buff_size == 0);

  assign_regs(&regs.r[0], &block->source);

  /* ColourTrans doesn't allow a sprite to be specified as the destination */
  assert(block->destination.type == ColourTransContextType_Screen);
  assign_regs(&regs.r[2], &block->destination);

  regs.r[4] = 0; /* return required buffer size */
  regs.r[5] = flags;
  regs.r[6] = (int)block->workspace;
  regs.r[7] = (int)block->transfer;
  DEBUGF("ClrTrans: Calling ColourTrans_GenerateTable with "
         "0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",
         regs.r[0], regs.r[1], regs.r[2], regs.r[3], regs.r[4], regs.r[5],
         regs.r[6], regs.r[7]);

  /* Find the buffer size required for the colour translation table */
  e = _kernel_swi(ColourTrans_GenerateTable, &regs, &regs);
  if (e == NULL)
  {
    assert(regs.r[4] >= 0);
    size_t const buff_req = regs.r[4];
    if (nbytes != NULL)
    {
      /* Output the required buffer size */
      *nbytes = buff_req;
      DEBUGF("ClrTrans: Outputting buffer requirement %zu\n", *nbytes);
    }

    if (buffer != NULL)
    {
      /* The SWI doesn't detect buffer overflows but since this function does
         it should return an error in those circumstances */
      if (buff_size < buff_req)
      {
        e = messagetrans_error_lookup(NULL, DUMMY_ERRNO, "BufOFlo", 0);
      }
      else
      {
        /* Generate the colour translation table.
           R0-R3 and R5-R7 should have been preserved by the earlier SWI. */
        regs.r[4] = (int)buffer;
        DEBUGF("ClrTrans: Calling ColourTrans_GenerateTable with "
               "0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",
               regs.r[0], regs.r[1], regs.r[2], regs.r[3], regs.r[4], regs.r[5],
               regs.r[6], regs.r[7]);

        e = _kernel_swi(ColourTrans_GenerateTable, &regs, &regs);
        if (e != NULL)
        {
          DEBUGF("ClrTrans: SWI error 0x%x '%s' (2)\n", e->errnum, e->errmess);
        }
      }
    }
  }
  else
  {
    DEBUGF("ClrTrans: SWI error 0x%x '%s' (1)\n", e->errnum, e->errmess);
  }
  return e;
}

/* ----------------------------------------------------------------------- */
/*                         Private functions                               */

static void assign_regs(int regs[], const ColourTransContext *context)
{
  assert(regs != NULL);
  assert(context != NULL);

  /* This seems to optimise much better than the equivalent switch() */
  if (context->type == ColourTransContextType_Screen)
  {
    regs[0] = context->data.screen.mode;
    regs[1] = (int)context->data.screen.palette;

    /* Actual determinant of context type for SWI is the value of R0 */
    assert(regs[0] < SpriteAreaPointerThreshold);
  }
  else
  {
    assert(context->type == ColourTransContextType_Sprite);
    regs[0] = (int)context->data.sprite.sprite_area;
    regs[1] = (int)context->data.sprite.name_or_pointer;

    /* Actual determinant of context type for SWI is the value of R0 */
    assert(regs[0] >= SpriteAreaPointerThreshold);
  }
}
