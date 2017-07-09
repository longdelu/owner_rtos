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
#include "rtos_task_switch.h"

#define   TASK_STACK_SIZE  1024

int g_task_flag1 = 0;

int g_task_flag2 = 0;

/** \brief �����ջ */
taskstack_t run_task_stack_buf[TASK_STACK_SIZE];
taskstack_t next_task_stack_buf[TASK_STACK_SIZE];

/** \brief ��ǰ����ṹ�� */
rtos_task_t run_task;

/** \brief ��һ����ṹ�� */
rtos_task_t next_task;

/** \brief ��ǰ���񣺼�¼��ǰ���ĸ������������� */
rtos_task_t * p_current_task;

/** \brief ��һ���������е������ڽ��������л�ǰ�������úø�ֵ��Ȼ�������л������л���ж�ȡ��һ������Ϣ */
rtos_task_t * p_next_task;


/** \brief ���������ָ�����飺�������ֻʹ���������� */
rtos_task_t * p_task_table[TASK_COUNT]; 


/**
 * \brief ��ǰ������ں���
 */
void run_task_entry (void *p_arg)
{
          
     for (; ;) {
         
        *((uint32_t*) p_arg) = 1;
        rtos_mdelay(100); 
        *((uint32_t*) p_arg) = 0;
        rtos_mdelay(100);          
        rtos_task_sched();
         
     }
}

/**
 * \brief ��һ��������ں���
 */
void next_task_entry (void *p_arg)
{    
    for (; ;) {
        
        *((uint32_t*) p_arg) = 1;
        rtos_mdelay(100); 
        *((uint32_t*) p_arg) = 0;
        rtos_mdelay(100); 
        rtos_task_sched();        
    }
}

/**
 * \brief ��ں���
 */
int main (void)
{    
   
    /* ϵͳ��������Ϊ1ms */
    rtos_systick_init(1);
    
    /* �����ȼ���4λ�������ȼ�Ҳ��4λ */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
    
    /* �����ʼ������ */
    rtos_task_init(&run_task, run_task_entry, &g_task_flag1, 0,  run_task_stack_buf, sizeof(run_task_stack_buf)); 
    rtos_task_init(&next_task, next_task_entry, &g_task_flag2, 0,  next_task_stack_buf, sizeof(next_task_stack_buf));
    
    /* ��ʼ������ṹ���б� */
    p_task_table[0] = &run_task;
    p_task_table[1] = &next_task;
    
    /* ��һ�����е�������run_task */
    p_next_task =  p_task_table[0];
    
    /*  �л���p_next_task ָ����������������Զ���᷵�� */
    rtos_task_run_first();
      
    return 0;    
}




/* end of file */

