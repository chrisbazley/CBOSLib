/*
 * CBOSLib: Veneers for operating system's VDU drivers
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

/* OSVDU.h declares a multi-purpose graphics plotting function and two
   functions to read information about screen modes or the VDU driver state.

Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 03-Oct-09: Created this header file.
  CJB: 22-Mar-19: Added the os_set_colour function.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

#ifndef OSVDU_h
#define OSVDU_h

/* ISO library headers */
#include <stdbool.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

#if !defined(USE_OPTIONAL) && !defined(_Optional)
#define _Optional
#endif

/* GCOL actions for use with the colourtrans_set_gcol function */
enum
{
  GCOLAction_OpaqueBG       = 0,  /* Ignore sprite mask */
  GCOLAction_TransparentBG  = 8,  /* Use sprite mask (also affects patterns) */
  GCOLAction_ColourPattern1 = 16,
  GCOLAction_ColourPattern2 = 32,
  GCOLAction_ColourPattern3 = 48,
  GCOLAction_ColourPattern4 = 64,
  GCOLAction_GiantPattern   = 80  /* Patterns 1 to 4 placed side by side */
};

/* Add one of the following values to the GCOL action */
enum
{
  GCOLAction_Overwrite,
  GCOLAction_Or,
  GCOLAction_And,
  GCOLAction_ExclusiveOr,
  GCOLAction_Invert,
  GCOLAction_None,
  GCOLAction_AndNot,
  GCOLAction_OrNot
};

/* Command codes for use with the os_plot function */
enum
{
  PlotOp_SolidInclBoth       = 0,
  PlotOp_SolidExclEnd        = 8,
  PlotOp_StartDottedInclBoth = 16,
  PlotOp_StartDottedExclEnd  = 24,
  PlotOp_SolidExclStart      = 32,
  PlotOp_SolidExclBoth       = 40,
  PlotOp_ContDottedExclStart = 48,
  PlotOp_ContDottedExclBoth  = 56,
  PlotOp_Point               = 64,
  PlotOp_HorFillLRToNonBG    = 72,
  PlotOp_TriangleFill        = 80,
  PlotOp_HorFillRToBG        = 88,
  PlotOp_RectangleFill       = 96,
  PlotOp_HorFillLRToFG       = 104,
  PlotOp_Parallelogram       = 112,
  PlotOp_HorFillRToNonFG     = 120,
  PlotOp_FloodToNonBG        = 128,
  PlotOp_FloodToFG           = 136,
  PlotOp_CircleOutline       = 144,
  PlotOp_CircleFill          = 152,
  PlotOp_CircleArc           = 160,
  PlotOp_Segment             = 168,
  PlotOp_Sector              = 176,
  PlotOp_Block               = 184,
  PlotOp_EllipseOutline      = 192,
  PlotOp_EllipseFill         = 200,
  PlotOp_Font                = 208,
  PlotOp_ReservedAcorn1      = 216,
  PlotOp_ReservedAcorn2      = 224,
  PlotOp_Sprite              = 232,
  PlotOp_ReservedUser1       = 240,
  PlotOp_ReservedUser2       = 248
};

/* Add one of the following values to the os_plot command code */
enum
{
  PlotOp_MoveRel,
  PlotOp_PlotFGRel,  /* Do not use with OS_Plot_Block */
  PlotOp_PlotInvRel, /* Do not use with OS_Plot_Block */
  PlotOp_PlotBGRel,  /* Do not use with OS_Plot_Block */
  PlotOp_MoveAbs,
  PlotOp_PlotFGAbs,  /* Do not use with OS_Plot_Block */
  PlotOp_PlotInvAbs, /* Do not use with OS_Plot_Block */
  PlotOp_PlotBGAbs   /* Do not use with OS_Plot_Block */
};

/* Add one of the following values to OS_Plot_Block */
enum
{
  PlotOp_BlockMoveRel = 1,
  PlotOp_BlockCopyRel = 2,
  PlotOp_BlockMoveAbs = 5,
  PlotOp_BlockCopyAbs = 6
};

/* Mode variable numbers for use with the os_read_mode_variable and
   os_read_vdu_variables functions */
typedef enum
{
  ModeVar_ModeFlags,
  ModeVar_ScrRCol,
  ModeVar_ScrBRow,
  ModeVar_NColour,
  ModeVar_XEigFactor,
  ModeVar_YEigFactor,
  ModeVar_LineLength,
  ModeVar_ScreenSize,
  ModeVar_YShftFactor,
  ModeVar_Log2BPP,
  ModeVar_Log2BPC,
  ModeVar_XWindLimit,
  ModeVar_YWindLimit
}
ModeVar;

/* Screen mode flags */
#define ModeFlag_NonGraphics  (1u << 0)
#define ModeFlag_Teletext     (1u << 1)
#define ModeFlag_LineGap      (1u << 2)
#define ModeFlag_BBCLineGap   (1u << 3)
#define ModeFlag_HighResMono  (1u << 4)
#define ModeFlag_DoubleHeight (1u << 5)
#define ModeFlag_NoHWScroll   (1u << 6)
#define ModeFlag_ExtraColours (1u << 7)

/* VDU variable numbers for use with the os_read_vdu_variables function */
typedef enum
{
  VDUVar_EndOfList = -1,
  VDUVar_GWLCol = 128,
  VDUVar_GWBRow,
  VDUVar_GWRCol,
  VDUVar_GWTRow,
  VDUVar_TWLCol,
  VDUVar_TWBRow,
  VDUVar_TWRCol,
  VDUVar_TWTRow,
  VDUVar_OrgX,
  VDUVar_OrgY,
  VDUVar_GCsX,
  VDUVar_GCsY,
  VDUVar_OlderCsX,
  VDUVar_OlderCsY,
  VDUVar_OldCsX,
  VDUVar_OldCsY,
  VDUVar_GCsIX,
  VDUVar_GCsIY,
  VDUVar_NewPtX,
  VDUVar_NewPtY,
  VDUVar_ScreenStart,
  VDUVar_DisplayStart,
  VDUVar_TotalScreenSize,
  VDUVar_GPLFMD,
  VDUVar_GPLBMD,
  VDUVar_GFCOL,
  VDUVar_GBCOL,
  VDUVar_TForeCol,
  VDUVar_TBackCol,
  VDUVar_GFTint,
  VDUVar_GBTint,
  VDUVar_TFTint,
  VDUVar_TBTint,
  VDUVar_MaxMode,
  VDUVar_GCharSizeX,
  VDUVar_GCharSizeY,
  VDUVar_GCharSpaceX,
  VDUVar_GCharSpaceY,
  VDUVar_HLineAddr,
  VDUVar_TCharSizeX,
  VDUVar_TCharSizeY,
  VDUVar_TCharSpaceX,
  VDUVar_TCharSpaceY,
  VDUVar_GcolOraEorAddr,
  VDUVar_VIDCClockSpeed,
  VDUVar_WindowWidth = 256,
  VDUVar_WindowHeight
}
VDUVar;

/* Constant for use with os_read_mode_variable */
enum
{
  OS_ReadModeVariable_CurrentMode = -1
};

/* Flags for use with the os_set_colour function */
#define OS_SetColour_Background (1u << 4) /* otherwise foreground */
#define OS_SetColour_UseECF     (1u << 5)

_Optional _kernel_oserror *os_set_colour(unsigned int /*flags*/,
                                         int          /*action*/,
                                         int          /*colour*/);
   /*
    * Sets the foreground or background graphics colour (based on the flags)
    * and plotting action (typically GCOLAction_Overwrite) used by os_plot.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_Optional _kernel_oserror *os_plot(int /*command*/, int /*x*/, int /*y*/);
   /*
    * Multi-purpose function to plot lines and shapes, move the graphics
    * cursor, or copy/move rectangular blocks (depending on the specified
    * command code). The remaining arguments specify a point on the screen
    * relative to the current graphics origin (if bit 2 of the command code is
    * set) or graphics cursor position (if bit 2 is clear). Uses external
    * graphics coordinates, which increase rightwards/upwards.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_Optional _kernel_oserror *os_read_vdu_variables(const VDUVar /*vars*/[],
                                                 int          /*values*/[]);
   /*
    * Reads information about the current state of the VDU drivers. The first
    * argument should point to an array of VDU or mode variable numbers to be
    * read, terminated by VDUVariable_EndOfList. When reading mode variables,
    * the initialisers for those array elements should be cast to type VDUVar.
    * The requested variable values will be written to the integer array
    * specified by the second argument.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_Optional _kernel_oserror *os_read_mode_variable(int      /*mode*/,
                                                 ModeVar  /*var*/,
                                                 int     */*value*/,
                                                 bool    */*valid*/);
   /*
    * Reads information about a given screen mode without having to change into
    * that mode. However, only one variable can be read at a time (unlike
    * os_read_vdu_variables). Unless the third argument is a null pointer, the
    * requested variable's value will be written to the specified integer.
    * Unless the fourth argument is a null pointer, the boolean that it points
    * to will be set to true to indicate that the variable and mode number were
    * valid; otherwise false.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
