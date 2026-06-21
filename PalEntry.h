/*
 * CBOSLib: Utilities for handling RISC OS palette entries
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

/* PalEntry.h declares numeric constants, macros and functions to allow
   manipulation of RISC OS palette entries (24 bit 0xBbGgRr00 format).

Dependencies: None.
Message tokens: None.
History:
  CJB: 01-Oct-09: Created this header file.
  CJB: 24-Dec-19: Input palette data can now be const.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
  CJB: 13-May-26: Use int instead of unsigned int for component values.
  CJB: 21-Jun-26: Use uint8_t as the type of PALETTE_GET_RED and friends.
*/

#ifndef PalEntry_h
#define PalEntry_h

#include <stdint.h>

typedef uint32_t PaletteEntry;

/* Bit masks to encode or decode a palette entry */
#define PaletteEntry_RedMask   (0x0000ff00u)
#define PaletteEntry_GreenMask (0x00ff0000u)
#define PaletteEntry_BlueMask  (0xff000000u) /* would overflow signed constant*/

enum
{
  MaxColourComponent      = 255, /* For each component of an RGB triplet */
  MaxBrightness           = 255, /* As returned by rgb_brightness or
                                    palette_entry_brightness */
  /* Bit shifts to encode or decode a palette entry: */
  PaletteEntry_RedShift   = 8,
  PaletteEntry_GreenShift = 16,
  PaletteEntry_BlueShift  = 24
};

/* Macros to extract individual colour components from a palette entry */
#define PALETTE_GET_RED(palette)                                               \
  ((uint8_t)(((palette) & PaletteEntry_RedMask) >> PaletteEntry_RedShift))

#define PALETTE_GET_GREEN(palette)                                             \
  ((uint8_t)(((palette) & PaletteEntry_GreenMask) >> PaletteEntry_GreenShift))

#define PALETTE_GET_BLUE(palette)                                              \
  ((uint8_t)(((palette) & PaletteEntry_BlueMask) >> PaletteEntry_BlueShift))

int palette_entry_brightness(PaletteEntry colour);
   /*
    * Calculates an integer value which reflects the brightness of a given
    * palette entry (0xBbGgRr00 format). Uses CIE luminance weights for red,
    * green and blue (standard conversion for broadcast television).
    * Returns: Brightness of the palette entry, between 0 and MaxBrightness.
    */

int rgb_brightness(int red, int green, int blue);
   /*
    * Equivalent to palette_entry_brightness except that the input colour is
    * expressed as red, green and blue components. Input values larger than
    * MaxColourComponent will be clipped to the valid range.
    * Returns: Brightness of the RGB triplet, between 0 and MaxBrightness.
    */

PaletteEntry make_palette_entry(int red, int green, int blue);
   /*
    * Combines the specified red, green and blue components to make a palette
    * entry. Input values larger than MaxColourComponent will be clipped to the
    * valid range.
    * Returns: Palette entry (0xBbGgRr00 format) equivalent to RGB triplet.
    */

int nearest_palette_entry(PaletteEntry const palette[],
                          int ncols, PaletteEntry colour);
   /*
    * Searches within a specified array of 'ncols' palette entries (0xBbGgRr00
    * format) for the one which most closely approximates a given colour, and
    * returns the index of that array element.
    * Returns: Index of the palette entry closest to that specified.
    */

int nearest_palette_entry_rgb(PaletteEntry const palette[],
                              int ncols, int red, int green, int blue);
   /*
    * Equivalent to nearest_palette_entry except that the target colour is
    * expressed as red, green and blue colour components. Input values larger
    * than MaxColourComponent will be clipped to the valid range.
    * Returns: Index of the palette entry closest to the specified RGB triplet.
    */

#endif
