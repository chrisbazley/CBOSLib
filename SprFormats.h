/*
 * CBOSLib: Definitions relating to the RISC OS sprite format
 * Copyright (C) 2003 Christopher Bazley
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

/* SprFormats.h declares two types and defines many constants for programs that
   manipulate Sprite files or use the sprite facilities provided by RISC OS.

Dependencies: None
Message tokens: None
History:
  CJB: 24-Jun-04: Added extra constant definitions to describe the layout of
                  the sprite info word.
  CJB: 04-Nov-04: Added summary and dependency information.
  CJB: 10-Jul-05: Added definitions of OS_SpriteOp plot codes.
  CJB: 18-Apr-06: Now uses unsigned integers from <stdint.h> within data
                  structures (PRM 5a says that OS_SpriteOp 17 treats offsets as
                  unsigned). Width is important (sprite file has same format).
  CJB: 02-Nov-06: Added new macro SPRITE_RIGHT_BIT.
  CJB: 18-Nov-06: Added symbolic definition of OS_SpriteOp reason code 24.
  CJB: 03-Dec-06: Fixed the SPRITE_RIGHT_BIT macro to work with pixel sizes
                  other than 8 bits.
  CJB: 09-Dec-06: Added conditional definition of sprite file type value.
  CJB: 30-Sep-09: Redefined many macro values as enumerated constants. Renamed
                  type spriteheader as SpriteHeader and spriteareaheader as
                  SpriteAreaHeader.
  CJB: 05-Oct-09: Properly parenthesised all arguments in the definition of
                  macro SPRITE_RIGHT_BIT to ensure correct order of evaluation.
                  Created an alternative macro SPRITE_RIGHT_BIT_LOG2 which is
                  more efficient when dealing with non-constant colour depths.
  CJB: 26-Jun-10: Made definition of deprecated type names conditional upon
                  definition of CBLIB_OBSOLETE.
  CJB: 27-Aug-15: Switched to signed integer types for all data words in a
                  sprite area, matching OSLib and RISCOSLib. The offsets in a
                  Wimp tool sprites area header are negative.
  CJB: 21-Sep-19: Added definitions of reason code SPRITEOP_READ_SAVE_AREA_SIZE
                  and struct ScaleFactors (for use with SPRITEOP_PLOTSCALED).
  CJB: 26-Jun-22: Added definitions of reason codes SPRITEOP_FLIP_X,
                  SPRITEOP_FLIP_Y, SPRITEOP_PLOT_TRANSFORMED_MASK and
                  SPRITEOP_PLOT_TRANSFORMED_SPRITE, as well as flags and
                  structs for transformed sprite plotting.
*/
#ifndef SprFormats_h
#define SprFormats_h

/* ISO library headers */
#include <stdint.h>

/* Modifiers for OS_SpriteOp reason code (add one) */
enum
{
  SPRITEOP_SYSTEMAREA       = 0,
  SPRITEOP_USERAREA_SPRNAME = (1u << 8),
  SPRITEOP_USERAREA_SPRPTR  = (1u << 9)
};

 /* Flags for transformed sprite plotting */
enum
{
  SPRITEOP_PLOT_TRANSFORMED_DST_QUAD = (1u << 0),
  SPRITEOP_PLOT_TRANSFORMED_SRC_RECT = (1u << 1),
};

/* Error numbers */
enum
{
  SPRITE_ERR_CREATEMEM   = 0x82,
  SPRITE_ERR_MEMORY      = 0x85,
  SPRITE_ERR_DOESNTEXIST = 0x86
};

/* OS_SpriteOp reason codes */
enum
{
  SPRITEOP_READCTRLBLOCK       = 8,
  SPRITEOP_INIT_AREA           = 9,
  SPRITEOP_LOAD_AREA           = 10,
  SPRITEOP_MERGE_AREA          = 11,
  SPRITEOP_SAVE_AREA           = 12,
  SPRITEOP_GET_SPR             = 14,
  SPRITEOP_CREATE              = 15,
  SPRITEOP_GET_SPR_COORDS      = 16,
  SPRITEOP_VERIFY_AREA         = 17,
  SPRITEOP_SELECT_SPRITE       = 24,
  SPRITEOP_DELETE              = 25,
  SPRITEOP_RENAME              = 26,
  SPRITEOP_PLOT                = 28,
  SPRITEOP_CREATE_MASK         = 29,
  SPRITEOP_FLIP_X              = 33,
  SPRITEOP_PLOTCOORDS          = 34,
  SPRITEOP_SETPTRSHAPE         = 36,
  SPRITEOP_READINFO            = 40,
  SPRITEOP_READPIXCOL          = 41,
  SPRITEOP_FLIP_Y              = 47,
  SPRITEOP_PLOTSCALED          = 52,
  SPRITEOP_PLOT_TRANSFORMED_MASK = 55,
  SPRITEOP_PLOT_TRANSFORMED_SPRITE = 56,
  SPRITEOP_SWITCHOUTPUT_SPRITE = 60,
  SPRITEOP_SWITCHOUTPUT_MASK   = 61,
  SPRITEOP_READ_SAVE_AREA_SIZE = 62
};

#define SPRITE_INFO_NOT_MODE_SEL   (1u << 0)
#define SPRITE_INFO_HOZ_DPI_SHIFT  1
#define SPRITE_INFO_HOZ_DPI_MASK   0x00003ffe
#define SPRITE_INFO_VER_DPI_SHIFT  14
#define SPRITE_INFO_VER_DPI_MASK   0x07ffc000
#define SPRITE_INFO_TYPE_SHIFT     27
#define SPRITE_INFO_TYPE_MASK      0xf8000000

enum
{
  SPRITE_TYPE_OLD,
  SPRITE_TYPE_1BPP,
  SPRITE_TYPE_2BPP,
  SPRITE_TYPE_4BPP,
  SPRITE_TYPE_8BPP,
  SPRITE_TYPE_16BPP,
  SPRITE_TYPE_32BPP,
  SPRITE_TYPE_CMYK
};

/* OS_SpriteOp plot codes */
enum
{
  SPRITE_ACTION_OVERWRITE,
  SPRITE_ACTION_OR,
  SPRITE_ACTION_AND,
  SPRITE_ACTION_EOR,
  SPRITE_ACTION_INVERT,
  SPRITE_ACTION_LEAVE,
  SPRITE_ACTION_AND_NOT,
  SPRITE_ACTION_OR_NOT,
  SPRITE_ACTION_USE_MASK
};

/* Calculate the last bit used (0 - 31) in the last word of each row of pixel
   data for a sprite. Assumes no lefthand wastage. */
#define SPRITE_RIGHT_BIT(width, bpp) (width > 0 ? \
   (((bpp) * (width)) % 32 > 0 ? ((bpp) * (width)) % 32 : 32) - 1 : 0)

/* It is more efficient to use binary shifts when dealing with a non-constant
   number of bits per pixel */
#define SPRITE_RIGHT_BIT_LOG2(width, log2bpp) (width > 0 ? \
   (((width) << (log2bpp)) % 32 > 0 ? ((width) << (log2bpp)) % 32 : 32) -1 : 0)

/* Structure of sprite area header */
typedef struct
{
  int size;
  int sprite_count;
  int first;
  int used;
  int extension_words[];
}
SpriteAreaHeader;

/* Structure of sprite header */
typedef struct
{
  int size;
  char name[12];
  int width;
  int height;
  int left_bit;
  int right_bit;
  int image;
  int mask;
  int type;
  int palette_data[];
}
SpriteHeader;

typedef struct {
  int xmul;
  int ymul;
  int xdiv;
  int ydiv;
} ScaleFactors;

typedef struct {
  int xxmul;
  int xymul;
  int yxmul;
  int yymul;
  int xadd;
  int yadd;
} TransformMatrix;

typedef struct {
  struct {
    int x;
    int y;
  } coords[4];
} TransformQuad;

/* Deprecated type names */
#define spriteheader SpriteHeader
#define spriteareaheader SpriteAreaHeader

#endif /* SprFormats_h */
