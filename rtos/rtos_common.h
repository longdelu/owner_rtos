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


#ifndef __RTOS_TASK_COMMON_H
#define __RTOS_TASK_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"

    
/**
 * \name RTOS 操作结果 
 * @{
 */     
#define  RTOS_OK                         0   /** \brief 操作成功OK */               
#define  RTOS_ERROR                      1   /** \brief 操作失败 */
#define  RTOS_EINVAILD                   2   /** \brief 操作无效 */  
#define  RTOS_TIMEOUT                    3   /** \brief 操作超时 */
#define  RTOS_UNUSED                     4   /** \brief 资源不可用 */
#define  RTOS_DEL                        5   /** \brief 资源被删除 */
#define  RTOS_FULL                       6   /** \brief 资源已满 */
#define  RTOS_ERROR_OWER                 7   /** \brief不匹配的所有者 */


/** @} */
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_COMMON_H */





