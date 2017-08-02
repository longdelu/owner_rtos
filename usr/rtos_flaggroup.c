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
 * \brief RTOS 消息邮箱实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_flaggroup.h"
 
 
/**
 * \brief 初始化事件标记组
 */  
void rtos_flaggroup_init(rtos_flag_group_t *p_flag_grp, uint32_t flag)
{
    rtos_task_event_init(&p_flag_grp->flagg_rp_event, RTOS_EVENT_TYPE_FLAG_GRP);
    
    p_flag_grp->flag = flag;
                 
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/* end of file */

