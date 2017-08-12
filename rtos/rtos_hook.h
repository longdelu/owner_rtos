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
 * \brief RTOS 勾子函数接口文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_HOOK_H
#define __RTOS_HOOK_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task.h"
    
/**
 * \brief cpu空闲时的hooks
 * \param 无 
 * \return 无
 */ 
void rtos_hook_cpu_idle (void);


/**
 * \brief 时钟节拍Hooks
 * \param 无 
 * \return 无
 */ 
void rtos_hook_systick (void);

/**
 * \brief 任务切换hooks
 * \param[in] p_task_from: 任务结构体指针
 * \param[in] p_task_to: 任务结构体指针
 * \return 无
 */ 
void rtos_hook_task_swtich (rtos_task_t *p_task_from, rtos_task_t *p_task_to);


/**
 * \brief 任务初始化的Hooks
 * \param[in] p_task: 任务结构体指针
 * \return 无
 */ 
void rtos_hook_task_init (rtos_task_t *p_task); 
    

    

#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_HOOK_H */
