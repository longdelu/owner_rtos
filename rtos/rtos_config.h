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
 * \brief RTOS�����ļ�
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
 * \name RTOS����������Ϣ 
 * @{
 */    
    

/** \brief ��������ȼ����         */    
#define   RTOS_PRIO_COUNT                32   
    
/** \brief ����������е�ʱ��Ƭ���� */     
#define   RTOS_SLICE_MAX                 10
    
    
#define   RTOS_IDLE_TASK_STACK_SIZE  1024    
    
    
/** \brief ��������ȼ����         */    
#define   RTOS_TIMER_TASK_PRIO      RTOS_PRIO_COUNT - 2        

    
#define   TIMER_TASK_STACK_SIZE     1024   


/** \brief �Ƿ���CPU��ռ����        */  
#define   RTOS_CPU_USAGESTAT        1

/** \brief RTOS���ļ�����ڣ���λΪms */ 
#define   RTOS_SYSTICK_PERIOD        10

/** \brief 1�루S)ʱ���ڵ�SYSTICK���� */ 
#define   RTOS_PER_SEC_SYSTICK_COUNT      (1000 / RTOS_SYSTICK_PERIOD)
    
    
/** @} */
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CONFIG_H */





