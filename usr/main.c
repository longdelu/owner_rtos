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
 * \brief main�������
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "arm.h"
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_delay.h"

int task_flag1 = 0;

uint32_t task_stack_buf[1024];

rtos_task_t run_task;

/** \brief ��ǰ���񣺼�¼��ǰ���ĸ������������� */
rtos_task_t * currentTask;

/** \brief ��һ���������е������ڽ��������л�ǰ�������úø�ֵ��Ȼ�������л������л���ж�ȡ��һ������Ϣ */
rtos_task_t * nextTask;

void PendSVC_Trigger(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

int main (void)
{    
    
    /* ϵͳ��������Ϊ1ms */
    rtos_systick_init(10);
    
//    /* �����ȼ���4λ�������ȼ�Ҳ��4λ */
//    NVIC_SetPriorityGrouping(0x03);
//    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
    
    
    run_task.task_stack = &task_stack_buf[1024];
    
    currentTask = &run_task;
    

    
    
    while(1) {
        
        task_flag1 = 0;
        
        /* ����PendSVC�쳣�ж� */
        PendSVC_Trigger();

        rtos_mdelay(100);
        
        task_flag1 = 1;
        
        /* ����PendSVC�쳣�ж� */
        PendSVC_Trigger();

        rtos_mdelay(100);        
               
    }
    
    
    return 0;    
}




/* end of file */

