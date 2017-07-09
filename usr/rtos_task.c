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
 * \brief ����ϵͳ������غ���ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "arm.h"
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_switch.h"
#include "rtos_task_delay.h"


/** \brief ��ǰ���񣺼�¼��ǰ���ĸ������������� */
extern rtos_task_t * p_current_task;

/** \brief ��һ���������е������ڽ��������л�ǰ�������úø�ֵ��Ȼ�������л������л���ж�ȡ��һ������Ϣ */
extern rtos_task_t * p_next_task;


/** \brief ���������ָ�����飺�������ֻʹ���������� */
extern rtos_task_t * p_task_table[TASK_COUNT];

/**
 * \brief �����ʼ��
 */
void rtos_task_init(rtos_task_t * task, 
                    void (*task_entry) (void *p_arg), 
                    void *p_arg, 
                    uint32_t task_prio, 
                    uint32_t *task_stack,
                    uint32_t task_stack_size)
{
    uint32_t * p_task_stack_top = NULL;
    
    
    task->stack_base = task_stack;
    task->stack_size = task_stack_size;
    
    memset(task_stack, 0, task_stack_size);
    
    
    p_task_stack_top =  task_stack +  task_stack_size / sizeof(taskstack_t);
    
    
    /*
     * ע���������㣺
     * 1������Ҫ�õ��ļĴ�����ֱ�����˼Ĵ����ţ�������IDE����ʱ�鿴Ч����
     * 2��˳���ܱ䣬Ҫ���PendSV_Handler�Լ�CPU���쳣�Ĵ�������������
     *
     */
    *(--p_task_stack_top) = (unsigned long)(1<<24);                // XPSR, ������Thumbģʽ���ָ���Thumb״̬����ARM״̬����
    *(--p_task_stack_top) = (unsigned long)task_entry;             // �������ڵ�ַPC(R15)
    *(--p_task_stack_top) = (unsigned long)0x14;                   // R14(LR), ���񲻻�ͨ��return xxx�����Լ�������δ��
    *(--p_task_stack_top) = (unsigned long)0x12;                   // R12, δ��
    *(--p_task_stack_top) = (unsigned long)0x3;                    // R3, δ��
    *(--p_task_stack_top) = (unsigned long)0x2;                    // R2, δ��
    *(--p_task_stack_top) = (unsigned long)0x1;                    // R1, δ��
    *(--p_task_stack_top) = (unsigned long)p_arg;                  // R0 = param, �����������ں���
    *(--p_task_stack_top) = (unsigned long)0x11;                   // R11, δ��
    *(--p_task_stack_top) = (unsigned long)0x10;                   // R10, δ��
    *(--p_task_stack_top) = (unsigned long)0x9;                    // R9, δ��
    *(--p_task_stack_top) = (unsigned long)0x8;                    // R8, δ��
    *(--p_task_stack_top) = (unsigned long)0x7;                    // R7, δ��
    *(--p_task_stack_top) = (unsigned long)0x6;                    // R6, δ��
    *(--p_task_stack_top) = (unsigned long)0x5;                    // R5, δ��
    *(--p_task_stack_top) = (unsigned long)0x4;                    // R4, δ��
    
    
    task->task_stack_top = p_task_stack_top;                                // �������յ�ֵ 
}    


/**
 * \brief �л�����һ���������У���������ϵͳ��һ������
 *
 * \note  ���жϵ�ǰ�л���tinyOS����ʱ�л�����1�����񣬻��Ƕ������Ѿ���������ִ�е��л�
 *
 */
void rtos_task_run_first (void)
{
    /* ���������ʹ�õ�����ջMSP(��Ȩ����*/  
    __set_PSP(0);
    
    PendSVC_Trigger();
    
}


/**
 * \brief �������
 */
void rtos_task_sched(void)
{
    
    if (p_current_task ==  p_task_table[0]) {
                
        p_next_task =  p_task_table[1];
        
    } else {
        
        p_next_task =  p_task_table[0];
        
    }
    
    rtos_task_switch();
    
}





/* end of file */
