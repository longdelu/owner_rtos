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
 
 #include "rtos_mbox.h"
 
 /**
 * \brief 初始化消息邮箱
 */  
void rtos_mbox_init(rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t max_count)
{
    rtos_task_event_init(&p_mbox->mbox_event, RTOS_EVENT_TYPE_MBOX);
    p_mbox->p_msg_buf   = p_msg_buf;
    p_mbox->write_index = 0;
    p_mbox->read_index  = 0;
    p_mbox->msg_cur_count  = 0;
    p_mbox->msg_max_count =  max_count;       
                 
}
 
 
 
  /* end of file */