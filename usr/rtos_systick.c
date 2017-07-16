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
 * \brief ����ϵͳ�δ�ʱ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "arm.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
#include "rtos_task_bitmap.h"


/** \brief ϵͳ�δ����ֵ */
uint32_t rtos_systick = 0;

  
/**
 * \brief  ������ʱ�δ���
 */
static void __rtos_task_delay_tick_handler (void)
{
    volatile uint32_t i = 0;
       
    /*���ٽ���������*/
    uint32_t status = rtos_task_critical_entry();
    
      
    /* ������������delayTicks���������0�Ļ�����1 */
    
    for (i = 0; i < TASK_COUNT; i++) {
        
        if (p_task_table[i]->delay_ticks > 0) {
            
            p_task_table[i]->delay_ticks--;
            
        } else {
            
            /* ʹ���ȼ����� */
            rtos_task_bitmap_set(&task_priobitmap, i);            
            
        }
        
    }

    /* �˳��ٽ������� */
    rtos_task_critical_exit(status);    
}
 
/**
 * \brief  ����ϵͳ�δ�ʱ�ӳ�ʼ������
 */
void rtos_systick_init (uint32_t ms)
{
    SysTick->LOAD  = ms * SystemCoreClock / 1000 - 1; 
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    SysTick->VAL   = 0;                           
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk; 
}

/**
 * \brief  ����ϵͳ�δ��жϴ�����
 *
 * \param[in] �� 
 *
 * \return    ��
 */
void SysTick_Handler (void) 
{
    /*���ٽ���������*/
    uint32_t status = rtos_task_critical_entry();
    
    /* �δ���� */
    rtos_systick++;
        
    __rtos_task_delay_tick_handler();
    
    /* �˳��ٽ������� */
    rtos_task_critical_exit(status);
            
    /* ��������п�����������ʱ���(delayTicks = 0)������һ�ε��� */
    rtos_task_sched(); 
}


 
 
 /* end of file */

