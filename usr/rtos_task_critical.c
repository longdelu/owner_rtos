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
 * \brief ����ϵͳ�ٽ���������ͨ���������ж���ʵ�֣�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_task_critical.h"
 

/**
 * \brief  �����ٽ���
 */
uint32_t rtos_task_critical_entry (void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();        /*  CPSID I */
    return primask;    
}

/**
 * \brief  �˳��ٽ���
 */
void rtos_task_critical_exit (uint32_t status)
{
     __set_PRIMASK(status);       
}



 /* end of file */
