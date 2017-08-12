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
 * \brief RTOS 占有率接口文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_CPU_USE_H
#define __RTOS_CPU_USE_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_config.h"

/**
 * \brief 空闲任务计数运行次数++
 */ 
void rtos_cpu_idle_count_inc (void );
    
/**
 * \brief 为检查cpu使用率与系统时钟节拍同步
 * \param[in] 无
 * \return    无    
 */ 
void  rtos_cpu_use_sync_with_systick (void);   
    
    
/**
 * \brief  初始化cpu统计
 */  
void rtos_cpu_use_init (void);


/**
 * \brief 检查cpu使用率
 */ 
void rtos_cpu_use_check (void);

/**
 * \brief cpu使用率获取
 */ 
float rtos_cpu_use_get (void) ;

    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_CPU_USE_H */
