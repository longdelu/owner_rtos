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


#ifndef __RTOS_TASK_CONFIG_H
#define __RTOS_TASK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_common.h"

/**
 * \name RTOS任务配置信息 
 * @{
 */    
    

/** \brief 任务的优先级序号         */    
#define   RTOS_PRIO_COUNT                32   
    
/** \brief 任务最大运行的时间片计数 */     
#define   RTOS_SLICE_MAX                 10
    
/** \brief 空闲任务堆栈大小         */        
#define   RTOS_IDLE_TASK_STACK_SIZE  1024    
       
/** \brief 定时器任务的优先级序号         */    
#define   RTOS_TIMER_TASK_PRIO      RTOS_PRIO_COUNT - 2        

/** \brief 定时器任务堆栈大小         */       
#define   TIMER_TASK_STACK_SIZE     1024   

/** \brief RTOS节拍间隔周期，单位为ms */ 
#define   RTOS_SYSTICK_PERIOD        10

/** \brief 1秒（S)时间内的SYSTICK计数 */ 
#define   RTOS_PER_SEC_SYSTICK_COUNT      (1000 / RTOS_SYSTICK_PERIOD)


/** \brie 内核功能裁剪部分 */
#define   RTOS_ENABLE_SEM               0     /**< \brief 是否使能信号量 */
#define   RTOS_ENABLE_MUTEX_SEM         0     /**< \brief 是否使能互斥信号量 */
#define   RTOS_ENABLE_FLAG_GRP          0     /**< \brief 是否使能事件标志组 */
#define   RTOS_ENABLE_MBOX              0     /**< \brief 是否使能邮箱 */
#define   RTOS_ENABLE_MEMBLOCK          0     /**< \brief 是否使能存储块 */
#define   RTOS_ENABLE_TIMER             0     /**< \brief 是否使能定时器 */
 
#define   RTOS_ENABLE_CPU_USE_CHECK     0     /**< \brief 是否检查CPU的占有率  */ 
    
    
/** @} */
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CONFIG_H */





