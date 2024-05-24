/*
 * CBLibrary: Definitions for new or extended Wimp SWIs and message protocols
 * Copyright (C) 2006 Christopher Bazley
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

/* WimpExtra.h declares additional types and macro values for the message
   protocol described in Acorn application note 241 (drag and drop).

Dependencies: None.
Message tokens: None.
History:
  CJB: 07-Aug-06: Created this header.
  CJB: 24-Sep-06: Added new flags for Wimp_ReportError.
  CJB: 25-Sep-06: Added messages for selection model & clipboard protocol.
  CJB: 30-Sep-09: Defined named constants for Wimp colour numbers and scroll
                  request directions.
  CJB: 04-Oct-20: Made definition of Wimp_ReportError_UseCategory conditional
                  for compatibility with newer "wimp.h".
  CJB: 30-Oct-21: Added declarations of wimp_drag_box2 and wimp_auto_scroll.
*/

#ifndef WimpExtra_h
#define WimpExtra_h

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "wimp.h"


/*-- wimp message numbers --*/

#ifndef Wimp_MClaimEntity
#define Wimp_MClaimEntity                   0x0f
#endif

#ifndef Wimp_MDataRequest
#define Wimp_MDataRequest                   0x10
#endif

#ifndef Wimp_MDragging
#define Wimp_MDragging                      0x11
#endif

#ifndef Wimp_MDragClaim
#define Wimp_MDragClaim                     0x12
#endif

#ifndef Wimp_MReleaseEntity
#define Wimp_MReleaseEntity                 0x13
#endif


/*-- structures of wimp message data --*/

typedef struct
{
    unsigned int flags;
}
WimpClaimEntityMessage;

/* Flags for the above */

#define Wimp_MClaimEntity_CaretOrSelection (3u<<0)
#define Wimp_MClaimEntity_Clipboard        (1u<<2)


typedef struct
{
    int          destination_window;
    int          destination_icon;
    int          destination_x;
    int          destination_y;
    unsigned int flags;
    int          file_types[54]; /* in order of preference, terminated by
                                    FileType_Null */
} WimpDataRequestMessage;

/* Flags for the above */

#define Wimp_MDataRequest_Clipboard (1u<<2)


typedef struct
{
    int          window_handle;
    int          icon_handle;
    int          x;
    int          y;
    unsigned int flags;
    BBox         bbox; /* bounding box in 1/72000 inch (relative to pointer) */
    int          file_types[50]; /* in order of preference, terminated by
                                    FileType_Null */
}
WimpDraggingMessage;

/* Flags for the above */

#define Wimp_MDragging_DataFromSelection (1u << 1)
#define Wimp_MDragging_DataFromClipboard (1u << 2)
#define Wimp_MDragging_WillDeleteSource  (1u << 3)
#define Wimp_MDragging_DoNotClaimMessage (1u << 4)


typedef struct
{
    unsigned int flags;
    int          file_types[58]; /* in order of preference, terminated by
                                    FileType_Null */
}
WimpDragClaimMessage;

/* Flags for the above */

#define Wimp_MDragClaim_PtrShapeChanged  (1u << 0)
#define Wimp_MDragClaim_RemoveDragBox    (1u << 1)
#define Wimp_MDragClaim_DeleteSourceData (1u << 3)


typedef struct
{
    unsigned int flags;
}
WimpReleaseEntityMessage;

/* Flags for the above */

#define Wimp_MReleaseEntity_Clipboard    (1u << 2)

/*-- parameter block for SWI Wimp_AutoScroll --*/

typedef struct
{
  int   window_handle;
  BBox  pause_zones;
  int   pause_time;
  int   state_change_handler;
  void *workspace;
}
WimpAutoScrollBlock;

/*-- flags for SWI Wimp_AutoScroll --*/

#define Wimp_AutoScroll_Horizontal       (1u << 0)
#define Wimp_AutoScroll_Vertical         (1u << 1)
#define Wimp_AutoScroll_Delegate         (1u << 2)
#define Wimp_AutoScroll_ReadFlags        (1u << 7)
#define Wimp_AutoScroll_Started          (1u << 8)
#define Wimp_AutoScroll_PointerOutside   (1u << 9)
#define Wimp_AutoScroll_PointerInZone    (1u << 10)
#define Wimp_AutoScroll_PointerInCentre  (1u << 11)
#define Wimp_AutoScroll_PointerLeft      (1u << 12)
#define Wimp_AutoScroll_PointerBelow     (1u << 13)
#define Wimp_AutoScroll_PointerRight     (1u << 14)
#define Wimp_AutoScroll_PointerAbove     (1u << 15)
#define Wimp_AutoScroll_CanScrollLeft    (1u << 16)
#define Wimp_AutoScroll_CanScrollDown    (1u << 17)
#define Wimp_AutoScroll_CanScrollRight   (1u << 18)
#define Wimp_AutoScroll_CanScrollUp      (1u << 19)

_kernel_oserror *wimp_auto_scroll(unsigned int flags,
                                 WimpAutoScrollBlock *block,
                                 unsigned int *status);

/*-- flags for extended SWI Wimp_DragBox --*/

#define Wimp_DragBox_FixToWorkArea       (1u << 0)
#define Wimp_DragBox_ClipToWindow        (1u << 1)

_kernel_oserror *wimp_drag_box2(WimpDragBox *block, unsigned int flags);

/*-- new flags for SWI Wimp_ReportError --*/

#ifndef Wimp_ReportError_UseCategory
#define Wimp_ReportError_UseCategory     (1u << 8)
#endif
#define Wimp_ReportError_CatInform       (1u << 9)
#define Wimp_ReportError_CatWarning      (2u << 9)
#define Wimp_ReportError_CatProgram      (3u << 9)
#define Wimp_ReportError_CatQuestion     (4u << 9)


/*-- Wimp colour numbers --*/

enum
{
  WimpColour_Transparent = -1,
  WimpColour_White,
  WimpColour_VeryLightGrey,
  WimpColour_LightGrey,
  WimpColour_MidLightGrey,
  WimpColour_MidDarkGrey,
  WimpColour_DarkGrey,
  WimpColour_VeryDarkGrey,
  WimpColour_Black,
  WimpColour_DarkBlue,
  WimpColour_Yellow,
  WimpColour_LightGreen,
  WimpColour_Red,
  WimpColour_Cream,
  WimpColour_DarkGreen,
  WimpColour_Orange,
  WimpColour_LightBlue
};


/*-- Wimp scroll request directions --*/

enum
{
  WimpScrollRequest_AutoLeftDown = -3,
  WimpScrollRequest_PageLeftDown,
  WimpScrollRequest_LeftDown,
  WimpScrollRequest_NoChange,
  WimpScrollRequest_RightUp,
  WimpScrollRequest_PageRightUp,
  WimpScrollRequest_AutoRightUp,
};

#endif
