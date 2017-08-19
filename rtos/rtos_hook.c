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
 * \brief RTOS hook�������
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_hook.h"
#include "rtos_task_switch.h"
 
#if RTOS_ENABLE_HOOK == 1

/**
 * \brief cpu����ʱ��hooks
 */ 
void  rtos_hook_cpu_idle (void)
{

}

/**
 * \brief ʱ�ӽ���Hooks
 */ 
void rtos_hook_systick (void)
{

}

/**
 * \brief �����л�hooks
 */ 
void rtos_hook_task_swtich (rtos_task_t *p_task_from, rtos_task_t *p_task_to)
{
    

}


/**
 * \brief �����ʼ����Hooks
 */ 
void rtos_hook_task_init (rtos_task_t *p_task)
{

}

#endif
 
 /* end of file */


