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
 * \brief RTOS配置文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_INIT_H
#define __RTOS_INIT_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"

    
/**
 * \name RTOS 相关头文件
 * @{
 */
#include "arm.h"
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
#include "rtos_task_delay.h"
#include "rtos_task_switch.h"
#include "rtos_task_bitmap.h"
#include "rtos_task_list.h"
#include "rtos_task_event.h"
#include "rtos_sem.h"
#include "rtos_mbox.h"
#include "rtos_memblock.h"
#include "rtos_flaggroup.h"
#include "rtos_mutex_sem.h"
#include "rtos_soft_timer.h" 
#include "rtos_cpu_use.h"
#include "rtos_hook.h"


/** @} */

/**
 * \name RTOS 回调函数类型声明
 * @{
 */
 
/** \brief 回调函数定义，带参数 */
typedef void (*rtos_pfn_t) (void *p_arg); 

/** \brief 回调函数定义，无参数 */
typedef void (*rtos_pfn_no_arg_t) (void); 

/** @} */


/**
 * \name RTOS 相关全局变量声明
 * @{
 */

/** \brief 任务优先级的就绪表定义变量 */
extern rtos_task_bitmap_t task_priobitmap;

/** \brief  任务延时队列 */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief 当前任务：记录当前是哪个任务正在运行 */
extern rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
extern rtos_task_t * p_next_task;

/** \brief 空闲任务结构体 */
extern rtos_task_t idle_task;

/** \brief 空闲任务堆栈 */
extern taskstack_t idle_task_stack_buf[RTOS_IDLE_TASK_STACK_SIZE];

/** \brief 同一个优先级任务的链表头结点 */
extern rtos_task_list_t task_table[TASK_COUNT];

/** @} */


/**
 * \brief 空闲任务入口函数
 * \param[in]:无
 * \return 无
 */
int rtos_init (void);


/**
 * \brief rtos cpu占有率测试
 * \param[in]:无
 * \return 无
 */
void rtos_cpu_use_check_test (rtos_pfn_no_arg_t p_app_task_init);


/**
 * \brief rtos操作系统启动函数
 * \param[in]:无
 * \return 无
 */
void rtos_start (void);

    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_INIT_H */





