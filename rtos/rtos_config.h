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
    
/** \brief ���������ջ��С         */        
#define   RTOS_IDLE_TASK_STACK_SIZE  1024    
       
/** \brief ��ʱ����������ȼ����         */    
#define   RTOS_TIMER_TASK_PRIO      RTOS_PRIO_COUNT - 2        

/** \brief ��ʱ�������ջ��С         */       
#define   TIMER_TASK_STACK_SIZE     1024   

/** \brief RTOS���ļ�����ڣ���λΪms */ 
#define   RTOS_SYSTICK_PERIOD        10

/** \brief 1�루S)ʱ���ڵ�SYSTICK���� */ 
#define   RTOS_PER_SEC_SYSTICK_COUNT      (1000 / RTOS_SYSTICK_PERIOD)


/** \brie �ں˹��ܲü����� */
#define   RTOS_ENABLE_SEM               0     /**< \brief �Ƿ�ʹ���ź��� */
#define   RTOS_ENABLE_MUTEX_SEM         0     /**< \brief �Ƿ�ʹ�ܻ����ź��� */
#define   RTOS_ENABLE_FLAG_GRP          0     /**< \brief �Ƿ�ʹ���¼���־�� */
#define   RTOS_ENABLE_MBOX              0     /**< \brief �Ƿ�ʹ������ */
#define   RTOS_ENABLE_MEMBLOCK          0     /**< \brief �Ƿ�ʹ�ܴ洢�� */
#define   RTOS_ENABLE_TIMER             0     /**< \brief �Ƿ�ʹ�ܶ�ʱ�� */
 
#define   RTOS_ENABLE_CPU_USE_CHECK     0     /**< \brief �Ƿ���CPU��ռ����  */ 
#define   RTOS_ENABLE_HOOK              0     /**< \brief �Ƿ�ʹ�ù��Ӻ���     */     
    
    
/** @} */
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CONFIG_H */





