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
 * \brief RTOS ռ���ʲ���
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_cpu_use.h"
#include "rtos_task_delay.h"
#include "rtos_task_critical.h"

/* \brief  ʱ��ͬ�� */
static uint32_t rtos_enable_use_stat = 0; 

/* \brief  CPUռ���� */
static float  rtos_cpu_float_use  = 0.0f;
 
/* \brief  ������������������� */
static uint32_t idle_count = 0;
static uint32_t idle_max_count = 0; 

 
 
/**
 * \brief Ϊ���cpuʹ������ϵͳʱ�ӽ���ͬ��
 */ 
void rtos_cpu_use_sync_with_systick (void)
{
    while (rtos_enable_use_stat == 0) {
       ;
    }
 
}

/**
 * \brief ��������������д���++
 */ 
void rtos_cpu_idle_count_inc (void )
{
    idle_count++; 
}
 
/**
 * \brief  ��ʼ��cpuͳ��
 */  
void rtos_cpu_use_init (void)
{
    idle_count = 0;
    idle_max_count = 0;
    
    rtos_cpu_float_use = 0;
    
    rtos_enable_use_stat = 0;
}

/**
 * \brief ���cpuʹ����
 */ 
void rtos_cpu_use_check (void)  
{
    /* ������������ʱ��ͬ�� */
    if (rtos_enable_use_stat == 0) {
        rtos_enable_use_stat = 1;
        rtos_set_systick(0);  
        
        return;
    }
    
    if (rtos_get_systick() == RTOS_PER_SEC_SYSTICK_COUNT) {
        /* ͳ�����1s�ڵ�������ֵ */
        idle_max_count = idle_count;
        idle_count = 0;
        
        /* ������ϣ������������������л����������� */   
        rtos_task_sched_enable();
    } else if (rtos_get_systick() % RTOS_PER_SEC_SYSTICK_COUNT == 0) {
        
        /* ֮��ÿ��1sͳ��һ�Σ�ͬʱ����cpu������ */
        rtos_cpu_float_use = 100 - (idle_count * 100.0f / idle_max_count);
        idle_count = 0;

    }        
}

/**
 * \brief cpuʹ���ʻ�ȡ
 */ 
float rtos_cpu_use_get (void) 
{
    float cpu_use = 0.0f;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    cpu_use = rtos_cpu_float_use;
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
      
    return cpu_use;
}







 
 
 
 /* end of file */

