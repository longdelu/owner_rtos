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
 * \brief RTOS 事件控制块指针
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#ifndef __RTOS_FLAGGROUP_H
#define __RTOS_FLAGGROUP_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
typedef struct rtos_flag_group {
    
    /** \brief 事件控制块   */
    rtos_task_event_t flagg_rp_event;
    
    /** \brief 当前事件标记 */
    uint32_t flag;
    
    
}rtos_flag_group_t;


/**
 * \brief 初始化事件标记组
 * \param[in] p_flag_grp: 指向事件标记组结构体的指针
 * \param[in] flag:  初始的事件标志
 * \return 无
 */  
void rtos_flaggroup_init(rtos_flag_group_t *p_flag_grp, uint32_t flag);
    
    
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_FLAGGROUP_H */

