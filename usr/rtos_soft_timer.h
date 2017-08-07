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
 * \brief RTOS 软件定时器实现接口文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 
#ifndef __RTOS_SOFT_TIMER_H
#define __RTOS_SOFT_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"


/** \brief 定时器的状态 */    
typedef  enum rtos_timer_sate {
    RTOS_TIMER_CREATED,     /** < \brief  定时器已经创建 */
    RTOS_TIMER_STARTED,     /** < \brief  定时器已经启动 */
    RTOS_TIMER_RUNNING,     /** < \brief  定时器正在执行回调函数 */
    RTOS_TIMER_STOPPED,     /** < \brief  定时器已经停止 */
    RTOS_TIMER_DESTROYED    /** < \brief  定时器已经销毁 */   
}rtos_timer_sate_t;

/** \brief  软定时器结构体声明 */
typedef struct rtos_timer
{
    /** \brief  链表结点 */
    dlist_node_t timer_node;

    /** \brief  初次启动延后的ticks数 */
    uint32_t start_delay_ticks;

    /** \brief  周期定时时的周期tick数 */
    uint32_t duration_ticks;

    // 当前定时递减计数值,可以指示 start_delay_ticks 或  duration_ticks 计数完成
    uint32_t delay_ticks;

    // 定时回调函数
    void (*pfn_timer_func) (void *p_arg);

    // 传递给回调函数的参数
    void *p_arg;

    // 定时器配置参数
    uint32_t config;

    // 定时器状态
    rtos_timer_sate_t state;
}rtos_timer_t;


/** \brief 软硬定时器种类 */
#define TIMER_CONFIG_TYPE_HARD          (1 << 0)   /** \brief 在硬件中断中处理 */
#define TIMER_CONFIG_TYPE_SOFT          (0 << 0)   /** \brief 在任务中断中处理 */ 


/**
 * \brief 初始化定时器
 * \param[in] p_timer: 指向定时器结构体的指针
 * \param[in] delay_ticks:  定时器初始启动的延时ticks数
 * \param[in] duration_ticks:  给周期性定时器用的周期tick数，一次性定时器无效
 * \param[in] pfn_timer_func: 定时器回调函数
 * \param[in] p_arg:  传递给定时器回调函数的参数
 * \param[in] config: 定时器的初始配置， 值为  TIMER_CONFIG_TYPE_HARD 或 TIMER_CONFIG_TYPE_SOFT
 * \return 无
 */
void rtos_timer_init (rtos_timer_t * p_timer, 
                      uint32_t start_delay_ticks, 
                      uint32_t duration_ticks,
                      void (*pfn_timer_func)(void *p_arg), 
                      void *p_arg, 
                      uint32_t config);
                      
/**
 * \brief 启动定时器
 * \param[in] p_timer: 指向定时器结构体的指针
 * \return 无
 */
void rtos_timer_start (rtos_timer_t * p_timer);  


/**
 * \brief 停止定时器
 * \param[in] p_timer: 指向定时器结构体的指针
 * \return 无
 */
void rtos_timer_stop (rtos_timer_t * p_timer);                      



/**
 * \brief 通知系统滴答时钟已经发生了，因为它是在硬件中断处理函数中调用的，也可以在里面调用硬件定时器的处理
 * \param[in] 无
 * \return 无
 */
void rtos_timer_moudule_tick_notify (void); 


/**
 * \brief 定时器模块初始化函数
 * \param[in] 无
 * \return 无
 */
void  rtos_timer_moudule_init (void);                       
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_SOFT_TIMER_H */



