/*
 * CBOSLib: Veneers for TaskManager SWIs
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

/*
Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 21-Sep-26: Created this header file.
*/

#ifndef TaskMan_h
#define TaskMan_h

/* ISO library headers */
#include <stdint.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

#if !defined(USE_OPTIONAL) && !defined(_Optional)
#define _Optional
#endif

typedef struct
{
  int task_handle;
  const char *task_name;
  int slot_size;
  unsigned int flags;
}
TaskManagerTaskInfo;

_Optional _kernel_oserror *taskmanager_enumerate_tasks(
  intptr_t * /*context*/, TaskManagerTaskInfo * /*task_info*/);
   /*
    * Reads information about the next desktop task. On the first call,
    * 'context' must point to zero. It is updated for each subsequent call and
    * becomes negative when there are no more tasks. The task name is owned by
    * TaskManager and must be copied if it is needed after the next call.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
