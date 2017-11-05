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
#include "rtos_task_critical.h"
#include "rtos_task_delay.h"
#include "rtos_task_switch.h"
#include "rtos_task_bitmap.h"
#include "rtos_task_list.h"
#include "rtos_task_event.h"
#include "rtos_sem.h"
#include "rtos_mbox.h"

#define   TASK_STACK_SIZE  1024

/** \brief ����1��� */
int g_task_flag1 = 0;

/** \brief ����2��� */
int g_task_flag2 = 0;

/** \brief ����3��� */
int g_task_flag3 = 0;

/** \brief ����4��� */
int g_task_flag4 = 0; 

/** \brief �������ȼ��ı��λ�ýṹȫ���� */
rtos_task_bitmap_t task_priobitmap = {0};

/** \brief  ������ʱ���� */
rtos_task_list_t rtos_task_delayedlist;

/** \brief �����ջ */
taskstack_t first_task_stack_buf[TASK_STACK_SIZE];
taskstack_t second_task_stack_buf[TASK_STACK_SIZE];
taskstack_t third_task_stack_buf[TASK_STACK_SIZE];
taskstack_t forth_task_stack_buf[TASK_STACK_SIZE];

/** \brief ��ǰ����ṹ�� */
rtos_task_t first_task;

/** \brief ��һ������ṹ�� */
rtos_task_t second_task;

/** \brief ����������ṹ�� */
rtos_task_t third_task;

/** \brief ���ĸ�����ṹ�� */
rtos_task_t forth_task;

/** \brief ��ǰ���񣺼�¼��ǰ���ĸ������������� */
rtos_task_t * p_current_task;

/** \brief ��һ���������е������ڽ��������л�ǰ�������úø�ֵ��Ȼ�������л������л���ж�ȡ��һ������Ϣ */
rtos_task_t * p_next_task;

/** \brief ��������ṹ�� */
rtos_task_t idle_task;

/** \brief ���������ջ */
taskstack_t idle_task_stack_buf[TASK_STACK_SIZE];

/** \brief ͬһ�����ȼ����������ͷ��� */
rtos_task_list_t task_table[TASK_COUNT];

/** \brief �¼����ƿ����� */
rtos_task_event_t  g_event_waitnormal;


/**
 * \brief ����������ں���
 */
void idle_task_entry (void *p_arg)
{
    for (; ;) {
        /* ����������ʱʲô�������������Ա��û�������ռ */
        
    }
}

rtos_mbox_t mbox1;
rtos_mbox_t mbox2;  

/** \brief void *ָ�����飬&gp_msg_buf1[0], &gp_msg_buf1[1]���һ���ֽ� */
void *gp_msg_buf1[20];
void *gp_msg_buf2[20];

uint32_t g_msg_buf[20]; 

/**
 * \brief ��ǰ������ں���
 * \note  ϵͳ���ĳ�ʼ������Ҫ�������ʼ��������ɺ���ܵ���
 */
void first_task_entry (void *p_arg)
{  
    uint8_t i = 0;    
    
    /* ȷ�����񱻵����������ٳ�ʼ��ϵͳ��������Ϊ10ms�������������� */
    rtos_systick_init(10); 
   

    /* ��ʼ����Ϣ���䣬���20����Ϣһά����Ļ����� */    
    rtos_mbox_init(&mbox1, gp_msg_buf1, 10);
    
     for (; ;) {

        for (i = 0; i < 20; i++) {
             g_msg_buf[i] = i;     /*�����һά����ÿ��Ԫ�صĵ�ַ&g_msg_buf[i]��¼��gp_msg_buf1[i]���ָ���������� */
             rtos_mbox_notify(&mbox1, &g_msg_buf[i], RTOS_MBOX_SENDNORMAL); 
        }
        
        rtos_sched_mdelay(100);  


        // �󷢵���Ϣ���и������ȼ�
        // Ҳ���������task2~task3�õ�����Ϣֵ���19/18/...1�ݼ�
        // ��������������Ѿ����ڵȴ�����Ļ���ÿ��һ����Ϣ���������ĵ�����Ϣ
        // �����ʼ��˳��������仯
        for (i = 0; i < 20; i++) 
        {
            g_msg_buf[i] = i;
            rtos_mbox_notify(&mbox1, &g_msg_buf[i], RTOS_MBOX_SENDFRONT);
        }
        
        rtos_sched_mdelay(100);         
         
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(1); 
        *((uint32_t*) p_arg) = 0;
        rtos_sched_mdelay(1);              
     }
}

/**
 * \brief �ڶ���������ں���
 */
void second_task_entry (void *p_arg)
{   
    int error = 0;
    
    void *p_msg_buf = NULL;
        
    for (; ;) {
        error= rtos_mbox_wait(&mbox1, &p_msg_buf, 10);
        if (error == RTOS_OK) 
        {
            uint32_t value = *(uint32_t*)p_msg_buf;
            *((uint32_t*) p_arg) = value;
            rtos_sched_mdelay(1);
        }                
     
    }
}

/**
 * \brief ������������ں���
 */
void third_task_entry (void *p_arg)
{     
    int temp = 0;
    void *msg = (void *)&temp;
    
    /* ��ʼ����Ϣ���䣬���10����Ϣһά����Ļ����� */    
    rtos_mbox_init(&mbox2, gp_msg_buf2, 10);

    
    for (; ;) {
        
        /* ��ʱ�ȴ� */
        rtos_mbox_wait(&mbox2, &msg, 100);
             
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(1); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(1);      
    }
}

/**
 * \brief ���ĸ�������ں���
 */
void forth_task_entry (void *p_arg)
{        
    
    for (; ;) {
        
                
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(1); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(1); 
                      
    }
}

/**
 * \brief ��ں���
 */
int main (void)
{    
      
    /* �����ȼ���4λ�������ȼ�Ҳ��4λ */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
    
    /* ��ʼ�������� */
    rtos_task_schedlock_init();
    
    /* ��ʼ���������������λͼ���� */
    rtos_task_bitmap_init(&task_priobitmap);
    
    /* ��ʼ�����ȼ������������ */
    rtos_task_sched_init(task_table);
    
    /* ��ʼ��������ʱ���� */    
    rtos_task_delayed_init(&rtos_task_delayedlist);
    
    /* �����ʼ������ */
    rtos_task_init(&first_task,   first_task_entry,  &g_task_flag1, 0,  first_task_stack_buf,   sizeof(first_task_stack_buf)); 
    rtos_task_init(&second_task,  second_task_entry, &g_task_flag2, 1,  second_task_stack_buf,  sizeof(second_task_stack_buf));
    rtos_task_init(&third_task,   third_task_entry,  &g_task_flag3, 1,  third_task_stack_buf, sizeof(third_task_stack_buf));
    rtos_task_init(&forth_task,   forth_task_entry,  &g_task_flag4, 1,  forth_task_stack_buf, sizeof(forth_task_stack_buf));
        
    /* ���������ʼ�� */
    rtos_task_init(&idle_task, idle_task_entry, NULL, RTOS_PRIO_COUNT - 1,  idle_task_stack_buf, sizeof(idle_task_stack_buf));

    
    /* �Զ�����������ȼ����������� */
    p_next_task =  rtos_task_highest_ready();
    
    /*  �л���p_next_task ָ����������������Զ���᷵�� */
    rtos_task_run_first();
      
    return 0;    
}




/* end of file */

