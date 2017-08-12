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
 
 #include "rtos_init.h"
 
/**
 * \name RTOS ���ȫ�ֱ�������
 * @{
 */

/** \brief �������ȼ��ľ���������� */
rtos_task_bitmap_t task_priobitmap = {0};

/** \brief  ������ʱ���� */
rtos_task_list_t rtos_task_delayedlist;

/** \brief ��ǰ���񣺼�¼��ǰ���ĸ������������� */
rtos_task_t * p_current_task;

/** \brief ��һ���������е������ڽ��������л�ǰ�������úø�ֵ��Ȼ�������л������л���ж�ȡ��һ������Ϣ */
rtos_task_t * p_next_task;

/** \brief ��������ṹ�� */
rtos_task_t idle_task;

/** \brief ���������ջ */
taskstack_t idle_task_stack_buf[RTOS_IDLE_TASK_STACK_SIZE];

/** \brief ͬһ�����ȼ����������ͷ��� */
rtos_task_list_t task_table[TASK_COUNT];

taskstack_t idle_task_stack_buf[RTOS_IDLE_TASK_STACK_SIZE];

/* CPUռ���ʲ�����ʱ��,�����ʼ��Ҫ���ڿ�������ʱʱ��ִ�� */    

/** \brief ָ���û�Ӧ�õĺ���ָ�� */
rtos_pfn_no_arg_t pfn_app_task_init;  
 


/** @} */

/**
 * \brief ����������ں���
 */
void idle_task_entry (void *p_arg)
{
  
#if RTOS_ENABLE_CPU_USE_CHECK == 1
    
    /* CPUռ���ʲ�����ʱ�� ������Ƚ�ֹ */   
    rtos_task_sched_disable();
    
#if RTOS_ENABLE_TIMER == 1        
   /* CPUռ���ʲ�����ʱ��,�����ʼ��Ҫ���ڿ�����������ִ�� */    
    rtos_timer_task_init();
#endif    
    
    /* Ӧ�������ʼ�� */
    pfn_app_task_init();       

    /* ȷ�����񱻵����������ٳ�ʼ��ϵͳ��������Ϊ10ms�������������� */
    rtos_systick_init(RTOS_SYSTICK_PERIOD); 
    
    /* �ȴ�ʱ��ͬ�� */
    rtos_cpu_use_sync_with_systick();
    
#endif    
    
    for (; ;) {
        
    /* ��������������һЩ���Ӻ��� */
        
#if RTOS_ENABLE_CPU_USE_CHECK == 1
        /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
        uint32_t status = rtos_task_critical_entry(); 
        
        /*��ͳ�����п�������ļ��� */
        rtos_cpu_idle_count_inc();
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 
                 
#endif

        
    }
}



/**
 * \brief rtos��ʼ������
 */
int rtos_init (void)
{
    /* ��ʼ�������� */
    rtos_task_schedlock_init();
    
    /* ��ʼ���������������λͼ���� */
    rtos_task_bitmap_init(&task_priobitmap);
    
    /* ��ʼ�����ȼ������������ */
    rtos_task_sched_init(task_table);
    
    /* ��ʼ��������ʱ���� */    
    rtos_task_delayed_init(&rtos_task_delayedlist);
    
#if RTOS_ENABLE_TIMER == 1    
    /* ��ʼ����ʱ��ģ�� */  
    rtos_timer_moudule_init(); 

    /* ��ʼ����ʱ������ */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0
    rtos_timer_task_init();
    
#endif

#endif

    /* ���������ʼ�� */
    rtos_task_init(&idle_task, idle_task_entry, NULL, RTOS_PRIO_COUNT - 1,  idle_task_stack_buf, sizeof(idle_task_stack_buf));

    return RTOS_OK;
}


/**
 * \brief rtos cpuռ���ʲ���
 */
void rtos_cpu_use_check_test (rtos_pfn_no_arg_t p_app_task_init)
{
     pfn_app_task_init  =  p_app_task_init;      
}



/**
 * \brief rtos����ϵͳ��������
 */
void rtos_start (void)
{
    /* �Զ�����������ȼ����������� */
    p_next_task =  rtos_task_highest_ready();
    
    /*  �л���p_next_task ָ����������������Զ���᷵�� */
    rtos_task_run_first();
}










/* end of file */

