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
 * \brief ����ϵͳ��ͨ��ʱ����
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
#include "rtos_task_bitmap.h"

/** \brief ϵͳ�δ����ֵ */
extern  uint32_t rtos_systick;
 
/**
 * \brief  ����ϵͳ��ǰ�δ����
 */ 
uint32_t rtos_get_systick(void) 
{ 
     return rtos_systick; 
    
}

/**
 * \brief  ���õ�ǰϵͳ�δ����ֵ
 */ 
void rtos_set_systick (uint32_t rtos_systick_value_reload)
{
    rtos_systick =  rtos_systick_value_reload;   
}

/**
 * \brief  ����ϵͳ��ͨ��ʱ��û���������, ��10msΪһ��ʱ����λ
 */
void rtos_nosched_mdelay (int32_t ms) { 
    
    int32_t ms_end = rtos_systick + ms;
    
    /* 
     * ע��������Ҫǿ��ת����int32_t���ͣ������������������޷�λ32λ������ģ���������Ϊ0ʱ
     * ��һ���ж������ִ�е�ʱ���������⣬����Ϊ0ʱ�����û����ɣ����жϴ�ϣ���ʱrtos_systick�����ӣ���
     * ms_end - rtos_systick����0�ˣ�����һ���ܴ���޷�����
     */
    while (((int32_t)(ms_end - rtos_systick)) > 0) {
        ;
    }
}

/**
 * \brief  ����ϵ��ʱ�������������, ��10msΪһ��ʱ����λ
 */
void rtos_sched_mdelay (uint32_t ms)    
{   
    uint32_t status = rtos_task_critical_entry(); 
    
    /* ����������ʱ���У��������������ʱ�δ� */
    rtos_task_add_delayed_list(p_current_task, ms);
    
    /* ������Ӿ��������Ƴ�,������ͬһ���ȼ��Ķ���ʱʱ������ */
    rtos_task_sched_unready(p_current_task);
    
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 

    /*
     * Ȼ����������л����л�����һ�����񣬻��߿�������
     * delay_tikcs����ʱ���ж����Զ���1.������0ʱ�����л�������������
     */
    rtos_task_sched();              
}
    
    
 
 
 /* end of file */

