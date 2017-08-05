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


#ifndef __RTOS_TASK_EVENT_H
#define __RTOS_TASK_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "c_lib.h"
#include "rtos_task_list.h"
#include "rtos_config.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
    
    
typedef enum rtos_task_event_type {
    
    /** \brief 未知类型的事件控制块 */
    RTOS_EVENT_TYPE_UNKNOW   = (1UL << 16) ,
    
    /** \brief 计数信号量事件控制块 */
    RTOS_EVENT_TYPE_SEM      = (1UL << 17),

    /** \brief 消息邮箱事件控制块 */
    RTOS_EVENT_TYPE_MBOX     = (1UL << 18), 
    
    /** \brief 存储块控制块 */
    RTOS_EVENT_TYPE_MEMBLOCK = (1UL << 19),       

    /** \brief 事件标记组 */
    RTOS_EVENT_TYPE_FLAG_GRP = (1UL << 20),         
  
}rtos_task_event_type_t;
    
    
    
typedef struct rtos_task_event {
   
   /** \brief event事件的类型 */    
   rtos_task_event_type_t event_type;  
    
   /** \brief 任务等待事件列链表 */    
   rtos_task_list_t event_wait_list;  
    

}rtos_task_event_t;

/**
 * \brief 初始化RTOS任务事件控制块
 * \param[in] p_event: RTOS任务事件控制块的指针
 * \param[in] event_type: 遍历的回调函数
 * \return  无
 */  
void rtos_task_event_init (rtos_task_event_t *p_event, rtos_task_event_type_t event_type);


/**
 * \brief 让RTOS任务指定在事件控制块上等待事件发生
 * \param[in] p_task: 任务结构体指针  
 * \param[in] p_event: RTOS任务事件控制块的指针
 * \param[in] p_event_msg: 事件消息存储的具体位置
 * \param[in] time_out: 等待多长时间,带超时的等待，超时值时其delay_ticks必为0；
 * \return  无
 */  
void rtos_task_event_wait (rtos_task_t *p_task, 
                           rtos_task_event_t *p_event, 
                           void *p_event_msg, 
                           uint32_t state, 
                           uint32_t time_out);   
    
/**
 * \brief 从指定事件控制块上唤醒首个等待的RTOS任务(本质是把它重新加入到就绪队列中），调用这个唤醒事件的函数，其task->delay_ticks必须为0；
 * \param[in] p_event: RTOS任务事件控制块的指针
 * \param[in] p_event_msg: 事件消息存储的具体位置
 * \param[in] event_result: 事件等待的结果
 * \param[in] 从指定事件控制块上唤醒首个等待的RTOS任务的任务结构体指针       
 */  
rtos_task_t  *rtos_task_first_event_wake_up (rtos_task_event_t *p_event, void *p_event_msg, int32_t event_result);


/**
 * \brief 从事件控制块中唤醒指定任务
 * \param[in] p_event: RTOS任务事件控制块的指针
 * \param[in] p_task : 等待唤醒的任务
 * \param[in] p_event_msg : 事件消息存储的具体位置
 * \param[in] event_result: 事件等待的结果
 * \return  无 
 */  
void rtos_task_event_wake_up(rtos_task_event_t *p_event, rtos_task_t *p_task, void * p_event_msg, int32_t event_result);

/**
 * \brief 从指定事件控制块上删除等待的RTOS任务，
 * \param[in] p_task: 任务结构体指针  
 * \param[in] p_event_msg: 事件消息存储的具体位置
 * \param[in] event_result: 事件等待的结果
 * \return  无                         
 */  
void rtos_task_event_del (rtos_task_t  *p_task, void *p_event_msg, int32_t event_result);
                           
                           
                           
/**
 * \brief 事件控制块中等待的任务数量
 * \param[in] p_event: RTOS任务事件控制块的指针
 * \return  返回事件控制块中等待的任务数量    
 */
uint32_t rtos_event_wait_count (rtos_task_event_t *p_event);

/**
 * \brief 清除所有等待中的任务，将事件发送给所有任务
 * \param[in] p_event: RTOS任务事件控制块的指针
 * \param[in] p_event_msg: 事件消息存储的具体位置
 * \param[in] event_result: 事件等待的结果
 * \return  唤醒的任务数量
 */ 
uint32_t  rtos_task_event_all_remove (rtos_task_event_t *p_event, void *p_event_msg, int32_t event_result);

    


 
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_EVENT_H */





