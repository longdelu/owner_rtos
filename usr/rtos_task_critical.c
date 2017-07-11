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
 * \brief 操作系统临界区保护（通过开关总中断来实现）
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_task_critical.h"
 

/**
 * \brief  进入临界区
 */
uint32_t rtos_task_critical_entry (void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();        /*  CPSID I */
    return primask;    
}

/**
 * \brief  退出临界区
 */
void rtos_task_critical_exit (uint32_t status)
{
     __set_PRIMASK(status);       
}



 /* end of file */
