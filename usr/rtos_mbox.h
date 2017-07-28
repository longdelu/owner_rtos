/**
 * \file
 * \brief RTOS 消息邮箱头文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_MBOX_H
#define __RTOS_MBOX_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
    
typedef struct rtos_mbox {
    /** \brief 事件控制块 */
    rtos_task_event_t mbox_event;

    /** \brief 当前的消息数量 */
    uint32_t msg_cur_count;  

    /** \brief 读消息的索引 */
    uint32_t read_index;   

    /** \brief 写消息的索引 */
    uint32_t write_index;  

    /** \brief  最大允许容纳的消息数量 */
    uint32_t msg_max_count;
    
    /** \brief  消息存储缓冲区 */
    void **p_msg_buf;    
    
}rtos_mbox_t;  


/**
 * \brief 初始化消息邮箱
 * \param[in] p_mbox: 指向等待初始化的邮箱结构体的指针
 * \param[in] p_msg_buf:  消息存储缓冲区
 * \param[in] max_count: 消息的最大数量
 * \return  无
 */  
void rtos_mbox_init(rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t max_count);
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MBOX_H */

