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

/* ClrTrans.h declares several functions which perform colour translations.

Dependencies: ANSI C library & Acorn library kernel.
Message tokens: None.
History:
  CJB: 02-Oct-09: Created this header file.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
*/

#ifndef ClrTrans_h
#define ClrTrans_h

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

/* Local headers */
#include "SprFormats.h"
#include "PalEntry.h"

/* Flags for use with the colourtrans_set_gcol function */
#define ColourTrans_SetGCOL_Background (1u << 7) /* otherwise foreground */
#define ColourTrans_SetGCOL_UseECF     (1u << 8)

/* Flags for use with the colourtrans_read_palette function */
#define ColourTrans_ReadPalette_SpritePointer   (1u << 0) /* otherwise name */
#define ColourTrans_ReadPalette_FlashingColours (1u << 1)

/* Flags for use with the colourtrans_generate_table function */
#define ColourTrans_GenerateTable_SpritePointer    (1u << 0)
#define ColourTrans_GenerateTable_CurrentPalette   (1u << 1) /* when source is
                                                                a sprite */
#define ColourTrans_GenerateTable_TransferFunction (1u << 2)
#define ColourTrans_GenerateTable_AllowWideEntries (1u << 4) /* when destination
                                                                has >8 bpp */
#define ColourTrans_GenerateTable_TableFormatMask  (0xffu << ColourTrans_GenerateTable_TableFormatShift)

enum
{
  ColourTrans_GenerateTable_TableFormatShift = 24
};

/* Translation table output formats */
enum
{
  TableFormat_PixelTranslation,
  TableFormat_PhysicalPalette
};

/* Other constants for use with colourtrans_read_palette and
   colourtrans_generate_table */
#define ColourTrans_SpritePointer  ((SpriteAreaHeader *)256)
#define ColourTrans_CurrentPalette ((PaletteEntry *)-1)
#define ColourTrans_DefaultPalette ((PaletteEntry *)0)
/* PRM vol. 5a implies that ColourTrans_DefaultPalette is not supported by
   colourtrans_read_palette before RISC OS 3.5 */

enum
{
  ColourTrans_CurrentMode = -1
};

/* Type of source for a colour translation */
typedef enum
{
  ColourTransContextType_Screen,
  ColourTransContextType_Sprite
}
ColourTransContextType;

/* Source or destination specifier for a colour translation */
typedef struct
{
  ColourTransContextType type;
  union
  {
    struct
    {
      int           mode;
      PaletteEntry *palette;
    }
    screen;
    struct
    {
      SpriteAreaHeader  *sprite_area;
      void              *name_or_pointer;
    }
    sprite;
  }
  data;
}
ColourTransContext;

/* Parameters for generating a colour translation table */
typedef struct
{
  ColourTransContext  source;
  ColourTransContext  destination; /* ColourTransContext_Sprite not supported */
  PaletteEntry      (*transfer)(PaletteEntry);
  void               *workspace;
}
ColourTransGenerateTableBlock;

_kernel_oserror *colourtrans_set_gcol(unsigned int /*flags*/,
                                      int          /*action*/,
                                      PaletteEntry /*colour*/);
   /*
    * Sets the colour to be used for subsequent graphics plotting to the one
    * closest to a given palette entry (0xBbGgRr00 format) in the current
    * screen mode and palette. The foreground colour will be set unless the
    * ColourTrans_SetGCOL_Background flag is set. Extended Colour Fill patterns
    * will be used to give a better approximation of the true colour if the
    * ColourTrans_SetGCOL_UseECF flag is set.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_kernel_oserror *colourtrans_read_palette(
                             unsigned int              /*flags*/,
                             const ColourTransContext */*source*/,
                             PaletteEntry             */*buffer*/,
                             size_t                    /*buff_size*/,
                             size_t                   */*nbytes*/);
   /*
    * Reads a specified screen mode or sprite's palette into an array of
    * PaletteEntry elements provided by the caller. The output buffer size is
    * specified in bytes rather than array elements. If 'buff_size' is 0 then
    * nothing will be written and 'buffer' may be a null pointer (this is
    * useful to find the required buffer size). An error will be returned if a
    * buffer was specified but it is too small. In any case, unless 'nbytes' is
    * null, the integer to which it points will be set to the number of bytes
    * that would have been output had a large enough buffer been provided.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_kernel_oserror *colourtrans_generate_table(
                             unsigned int                         /*flags*/,
                             const ColourTransGenerateTableBlock */*block*/,
                             void                                */*buffer*/,
                             size_t                               /*buff_size*/,
                             size_t                              */*nbytes*/);
   /*
    * Generates a colour translation table for plotting sprites in screen modes
    * with a different palette and/or number of bits per pixel. The source
    * sprite or mode/palette and destination mode/palette for the colour
    * translation are specified by a separate parameter block, along with an
    * optional routine to be called to preprocess each palette entry.
    * If 'buff_size' is 0 then nothing will be written and 'buffer' may be a
    * null pointer (this is useful to find the required buffer size). An error
    * will be returned if a buffer was specified but it is too small. In any
    * case, unless 'nbytes' is null, the integer to which it points will be set
    * to the number of bytes that would have been output had a large enough
    * buffer been provided.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
