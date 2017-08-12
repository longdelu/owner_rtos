/*******************************************************************************
*                                 Embedded_rtos
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Free soft.
* All rights reserved.
*
* Contact information:
* web site:    
* e-mail:      
*******************************************************************************/


/**
 * \file
 * \brief RTOS hook函数添加
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_hook.h"
 
#if RTOS_ENABLE_HOOK == 1

/**
 * \brief cpu空闲时的hooks
 */ 
void  rtos_hook_cpu_idle (void)
{

}

/**
 * \brief 时钟节拍Hooks
 */ 
void rtos_hook_systick (void)
{

}

/**
 * \brief 任务切换hooks
 */ 
void rtos_hook_task_swtich (rtos_task_t *p_task_from, rtos_task_t *p_task_to)
{

}


/**
 * \brief 任务初始化的Hooks
 */ 
void rtos_hook_task_init (rtos_task_t *p_task)
{

}

#endif
 
 /* end of file */


