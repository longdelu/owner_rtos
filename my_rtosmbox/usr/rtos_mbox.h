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

/** \brief 操作消息顺序 */
#define RTOS_MBOX_SENDNORMAL             0x00        // 消息发送至缓冲区尾部
#define RTOS_MBOX_SENDFRONT              0x01        // 消息发送至缓冲区头部    
    
/** \brief 消息邮箱事件控制块 */       
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
    
    /** 
     * \brief 消息存储缓冲区, 一个p_msg_buf[i]消息缓冲区就指向一个一维的消息数组，所以是一个二维指针
     * \note 记住，指针当中 int _a[10]; int *p = _a; 其 *(p + 0) = p[0] = a[0]; 
     *       eg:  int a[10];
                  int b[10]
                  int c[10];
                  void *g_msg_buf[3] = {a, b,c}
     *              
     *            p_msg_buf    = g_msg_buf;    
     *            p_msg_buf[0] = g_msg_buf[0]
     *            p_msg_buf[0][0] = g_msg_buf[0][0] = a[0]  
     * 
     */ 
    void **p_msg_buf;    
    
}rtos_mbox_t;  

/** \brief 消息邮箱状态信息结构体 */  
typedef struct rtos_mbox_info {
    /** \brief 当前的消息数量 */
    uint32_t msg_cur_count;

    /** \brief 最大允许容纳的消息数量 */
    uint32_t max_msg_count;

    /** \brief 当前等待的任务计数  */
    uint32_t task_count;
}rtos_mbox_info_t;


/**
 * \brief 初始化消息邮箱
 * \param[in] p_mbox: 指向等待初始化的消息邮箱结构体的指针
 * \param[in] p_msg_buf:  消息存储缓冲区
 * \param[in] max_count: 消息的最大数量, 通俗来说最大有多少个一维数组
 * \return  无
 */  
void rtos_mbox_init(rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t msg_max_count);


/**
 * \brief 等待邮箱, 获取一则消息
 * \param[in] p_mbox: 指向消息邮箱结构体的指针
 * \param[in] p_msg_buf:  消息存储缓冲区
 * \param[in] wait_ticks: 最大等待的ticks数，为0表示无限等待
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */ 
int32_t rtos_mbox_wait (rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t wait_ticks);

/**
 * \brief  获取一则消息，如果没有消息，则立即退回
 * \param[in] p_mbox: 指向消息邮箱结构体的指针
 * \param[in] p_msg_buf:  消息存储缓冲区
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK,
 */ 
int32_t rtos_mbox_nowait_get (rtos_mbox_t *p_mbox, void **p_msg_buf);


/**
 * \brief  通知消息可用，唤醒等待队列中的一个任务，或者将消息插入到邮箱中
 * \param[in] p_mbox: 指向消息邮箱结构体的指针
 * \param[in] p_msg:  操作的消息
 * \param[in] notify_option:  操作的消息选项 RTOS_MBOX_SENDNORMAL 或 RTOS_MBOX_SENDFRONT
 * \return  -RTOS_FULL RTOS_OK  
 */ 
int32_t  rtos_mbox_notify (rtos_mbox_t *p_mbox, void *p_msg, uint32_t notify_option);

/**
 * \brief   清空邮箱中所有消息
 * \param[in] p_mbox: 指向消息邮箱结构体的指针
 * \return  无 
 */ 
void rtos_mbox_flush (rtos_mbox_t *p_mbox);

/**
 * \brief   销毁邮箱
 * \param[in] p_mbox: 指向消息邮箱结构体的指针
 * \return  因销毁该信号量而唤醒的任务数量
 */ 
uint32_t rtos_mbox_destroy (rtos_mbox_t *p_mbox);

/**       
 * \brief 查询消息邮箱状态信息
 * \param[in] p_sem:  指向计数信号量结构体的指针
 * \param[in] p_info: 指向消息邮箱状态信息结构体的指针  
 * \return  无
 */  
void rtos_mbox_info_get (rtos_mbox_t *p_mbox, rtos_mbox_info_t *p_info);   
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MBOX_H */

