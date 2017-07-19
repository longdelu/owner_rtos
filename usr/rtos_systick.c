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


/** \brief  ������ʱ���� */
extern rtos_task_list_t rtos_task_delayedlist;


/** \brief ϵͳ�δ����ֵ */
uint32_t rtos_systick = 0;

  
/**
 * \brief  ������ʱ�δ���
 */
static void __rtos_task_delay_tick_handler (void)
{
    volatile uint32_t i = 0;
    
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 
    
     rtos_task_t * p_task  = NULL;
       
    /*���ٽ���������*/
    uint32_t status = rtos_task_critical_entry();
       
    /* ��õ�һ���û���� */
    p_next  = dlist_begin_get(&rtos_task_delayedlist.head_node);
    
    /* ����λ��Ϊͷ��㱾�� */
    p_end  = dlist_end_get(&rtos_task_delayedlist.head_node);
    
    
     /* ������������delayTicks���������0�Ļ�����1 */
    while(p_next != p_end) {   

        /* �ȼ�¼�µ�ǰ��� */
        p_tmp =  p_next;
           
        /* �ȼ�¼��һ��������Ϣ */
        p_next = dlist_next_get(&rtos_task_delayedlist.head_node, p_next);
        
        
        p_task = RTOS_CONTAINER_OF(p_tmp, rtos_task_t, delay_node);
        
        if (--p_task->delay_ticks == 0) {
            
            rtos_task_del_delayed_list(p_task);
            
            rtos_task_sched_ready(p_task);
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

