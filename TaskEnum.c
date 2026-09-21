/*
 * CBOSLib: Veneer for SWI TaskManager_EnumerateTasks
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

/* History:
  CJB: 21-Sep-26: Created this source file.
*/

/* ISO library headers */
#include <inttypes.h>
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "TaskMan.h"

_Optional _kernel_oserror *taskmanager_enumerate_tasks(
  intptr_t *context, TaskManagerTaskInfo *task_info)
{
  assert(context != NULL);
  assert(task_info != NULL);

  _kernel_swi_regs regs = {
    .r = {
      *context,
      (intptr_t)(void *)task_info,
      sizeof(*task_info),
    }
  };
  _Optional _kernel_oserror *const e =
    _kernel_swi(TaskManager_EnumerateTasks, &regs, &regs);

  if (e == NULL)
  {
    *context = regs.r[0];
    if (*context >= 0)
    {
      DEBUGF("TaskEnum: task handle %d, name '%s', context %" PRIdPTR "\n",
             task_info->task_handle, task_info->task_name, *context);
    }
    else
    {
      DEBUGF("TaskEnum: no more tasks\n");
    }
  }
  else
  {
    DEBUGF("TaskEnum: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  return e;
}
