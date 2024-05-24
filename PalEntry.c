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

/* History:
  CJB: 01-Oct-09: Created this source file.
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 19-Apr-15: Recategorised per-entry debugging output as verbose.
  CJB: 09-Apr-16: Added brackets and modified format strings to avoid
                  GNU C compiler warnings.
  CJB: 18-Apr-16: Cast pointer parameters to void * to match %p.
  CJB: 22-Apr-19: Deleted redundant 'extern' linkage specifiers.
  CJB: 06-Jun-20: Component values are now clipped by an inline function.
                  Reordered functions to allow removal of pre-declarations.
                  Added more 'const' qualifiers to declarations.
 */

/* ISO library headers */
#include <stddef.h>
#include <limits.h>

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "PalEntry.h"

/* Weights given to each colour component when calculating brightness */
#define CIE_RED_WEIGHT   (0.299)
#define CIE_GREEN_WEIGHT (0.587)
#define CIE_BLUE_WEIGHT  (0.114)

#define CLIP_COMPONENT(comp) comp = clip_component(comp, #comp)

/* Other numeric constants */
enum
{
  FixedPointOne = 1 << 24,
  /* Weights given to each colour component when comparing palette entries.
     OS 2 ColourTrans used R2,G3,B1 whereas OS 3 ColourTrans uses R2,G4,B1. */
  RedWeight     = 2,
  GreenWeight   = 4,
  BlueWeight    = 1
};

/* ----------------------------------------------------------------------- */
/*                         Private functions                               */

static inline unsigned int clip_component(unsigned int comp,
  char const *const name)
{
  if (comp > MaxColourComponent)
  {
    DEBUGF("PalEntry: Clipping %s component %u\n", name, comp);
    comp = MaxColourComponent;
  }
  return comp;
}

/* ----------------------------------------------------------------------- */

static unsigned int calc_brightness(unsigned int const red,
                                    unsigned int const green,
                                    unsigned int const blue)
{
  assert(red <= MaxColourComponent);
  assert(green <= MaxColourComponent);
  assert(blue <= MaxColourComponent);

  /* Calculate brightness of given palette entry
    (use fixed point arithmetic for speed and fractional accuracy). */
  unsigned long brightness =
    (unsigned long)(CIE_RED_WEIGHT * FixedPointOne) * red +
    (unsigned long)(CIE_GREEN_WEIGHT * FixedPointOne) * green +
    (unsigned long)(CIE_BLUE_WEIGHT * FixedPointOne) * blue;

  brightness /= FixedPointOne;
  assert(brightness <= MaxBrightness);
  assert(MaxBrightness == MaxColourComponent); /* otherwise the calculation
                                                  would be more complex */

  DEBUGF("PalEntry: brightness of R:%u G:%u B:%u is %lu\n",
         red, green, blue, brightness);

  return (unsigned int)brightness;
}

/* ----------------------------------------------------------------------- */

static unsigned int find_nearest(PaletteEntry const palette[],
                                 unsigned int const ncols,
                                 unsigned int const red,
                                 unsigned int const green,
                                 unsigned int const blue)
{
  unsigned int least_dist = UINT_MAX, nearest = 0;

  assert(palette != NULL);
  assert(ncols > 0);
  assert(red <= MaxColourComponent);
  assert(green <= MaxColourComponent);
  assert(blue <= MaxColourComponent);

  for (unsigned int index = 0; index < ncols; index++)
  {
    int const red_diff = PALETTE_GET_RED(palette[index]) - red,
              green_diff = PALETTE_GET_GREEN(palette[index]) - green,
              blue_diff = PALETTE_GET_BLUE(palette[index]) - blue;

    /* Like ColourTrans we use a least squares function,
       but we have control over the weights */
    unsigned int const dist =
           RedWeight * red_diff * red_diff +
           GreenWeight * green_diff * green_diff +
           BlueWeight * blue_diff * blue_diff;

    if (dist < least_dist)
    {
      least_dist = dist;
      nearest = index;
    }
  }

  DEBUGF("PalEntry: colour %u (0x%08x) is nearest to R:%u G:%u B:%u in "
         "palette %p with %u entries\n", nearest, palette[nearest],
         red, green, blue, (void *)palette, ncols);

  return nearest;
}

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

unsigned int palette_entry_brightness(PaletteEntry const colour)
{
  return calc_brightness(PALETTE_GET_RED(colour),
                         PALETTE_GET_GREEN(colour),
                         PALETTE_GET_BLUE(colour));
}

/*----------------------------------------------------------------------- */

unsigned int rgb_brightness(unsigned int red,
                            unsigned int green,
                            unsigned int blue)
{
  CLIP_COMPONENT(red);
  CLIP_COMPONENT(green);
  CLIP_COMPONENT(blue);

  return calc_brightness(red, green, blue);
}

/*----------------------------------------------------------------------- */

PaletteEntry make_palette_entry(unsigned int red,
                                unsigned int green,
                                unsigned int blue)
{
  PaletteEntry colour;

  CLIP_COMPONENT(red);
  CLIP_COMPONENT(green);
  CLIP_COMPONENT(blue);

  colour = ((red << PaletteEntry_RedShift) & PaletteEntry_RedMask) |
           ((green << PaletteEntry_GreenShift) & PaletteEntry_GreenMask) |
           ((blue << PaletteEntry_BlueShift) & PaletteEntry_BlueMask);

  DEBUGF("PalEntry: made palette entry 0x%x from red %u, green %u, blue %u\n",
         colour, red, green, blue);

  return colour;
}

/*----------------------------------------------------------------------- */

unsigned int nearest_palette_entry(PaletteEntry const palette[],
                                   unsigned int const ncols,
                                   PaletteEntry const colour)
{
  assert(palette != NULL);
  assert(ncols > 0);

  return find_nearest(palette,
                      ncols,
                      PALETTE_GET_RED(colour),
                      PALETTE_GET_GREEN(colour),
                      PALETTE_GET_BLUE(colour));
}

/*----------------------------------------------------------------------- */

unsigned int nearest_palette_entry_rgb(PaletteEntry const palette[],
                                       unsigned int const ncols,
                                       unsigned int red,
                                       unsigned int green,
                                       unsigned int blue)
{
  assert(palette != NULL);
  assert(ncols > 0);

  CLIP_COMPONENT(red);
  CLIP_COMPONENT(green);
  CLIP_COMPONENT(blue);

  return find_nearest(palette, ncols, red, green, blue);
}
