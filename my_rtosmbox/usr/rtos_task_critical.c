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
 * \brief ����ϵͳ�ٽ���������ͨ���������ж���ʵ�֣�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_task_critical.h"
#include "c_lib.h"
#include "rtos_task_switch.h"
#include "rtos_task.h"

/** \brief ������������ */
uint16_t __schedlock_count = 0;
 

/**
 * \brief  �����ٽ���
 */
uint32_t rtos_task_critical_entry (void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();        /*  CPSID I */
    return primask;    
}

/**
 * \brief  �˳��ٽ���
 */
void rtos_task_critical_exit (uint32_t status)
{
     __set_PRIMASK(status);       
}

/**
 * \brief  ��ʼ��������
 */
void rtos_task_schedlock_init (void)
{
    __schedlock_count = 0;
}

/**
 * \brief  �������򿪣���ֹ�������
 */
void rtos_task_sched_disable (void)
{
     uint32_t status = rtos_task_critical_entry();
    
     if (__schedlock_count < 65535) {
         __schedlock_count++;
     }
     
     rtos_task_critical_exit(status); 
         
}

/**
 * \brief  �������رգ������������
 */
void rtos_task_sched_enable (void)
{
    uint32_t status = rtos_task_critical_entry();
    
    if (__schedlock_count > 0) {
        
        if (--__schedlock_count == 0) {
            
            /* 
             * �����ǰ����رյ�������ʱ�򣬸պÿ���ִ��������ȣ�
             * �����ǰ��������ȼ���Ϊ��ߣ���ִ���������
             */
            rtos_task_sched();
        }
    }
        
    rtos_task_critical_exit(status);            
}

/**
 * \brief  ��ѯ������״̬
 */
uint8_t rtos_task_schedlock_status (void)
{
    uint32_t status = rtos_task_critical_entry();
    
    uint8_t schedlock_status = 0;
    
    if (__schedlock_count > 0) {
        schedlock_status =  TASKSCHED_LOCK;
    } else {
        schedlock_status =  TASKSCHED_UNLOCK;
    }
        
    rtos_task_critical_exit(status); 

    return  schedlock_status;   
}

 /* end of file */