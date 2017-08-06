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
 * \brief RTOS 软件定时器原理实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#include "rtos_soft_timer.h"


/**
 * \brief 初始化定时器
 */
void rtos_timer_init (rtos_timer_t * p_timer, 
                      uint32_t start_delay_ticks, 
                      uint32_t duration_ticks,
                      void (*pfn_timer_func)(void *p_arg), 
                      void *p_arg, 
                      uint32_t config)
{
    
    dlist_init(&p_timer->timer_node);
    p_timer->start_delay_ticks = start_delay_ticks;
    p_timer->duration_ticks =  duration_ticks;
    
    p_timer->pfn_timer_func = pfn_timer_func;
    p_timer->p_arg          = p_arg;
    p_timer->config         = config;
    
    if (start_delay_ticks == 0) {
        p_timer->start_delay_ticks = duration_ticks;
             
    } else {
        p_timer->start_delay_ticks = start_delay_ticks;
    }
    
    
    p_timer->state = RTOS_TIMER_CREATED;
     
}






/* end of file */




