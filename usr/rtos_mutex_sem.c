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
 * \brief RTOS 互斥信号量实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_mutex_sem.h"
 
 /**
 * \brief 初始化互斥信号量
 */  
void rtos_mutex_sem_init (rtos_mutex_sem_t *p_mutex_sem)
{
    rtos_task_event_init(&p_mutex_sem->mutex_sem_event, RTOS_EVENT_TYPE_MUTEX_SEM);
    
    p_mutex_sem->p_task_ower = NULL;   

    p_mutex_sem->loced_count = 0;
    
    p_mutex_sem->task_ower_prior = RTOS_PRIO_COUNT;
}
 
 
 
 
 
 /* end of file */



