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


/** \brief 任务的优先级序号 */    
#define   RTOS_PRIO_COUNT                32   
    
    

/** \brief 操作成功OK */   
#define  RTOS_OK                         0

    
/** \brief 操作失败 */
#define  RTOS_ERROR                      1  


/** \brief 操作无效 */
#define  RTOS_EINVAILD                   2     
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CONFIG_H */





