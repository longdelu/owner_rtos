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
 
extern  uint32_t rtos_systick;
 
/**
 * \brief  ����ϵͳ��ǰ�δ����
 */ 
uint32_t rtos_get_systick(void) 
{ 
     return rtos_systick; 
}

/**
 * \brief  ����ϵͳ��ͨ��ʱ��û���������, ��10msΪһ��ʱ����λ
 */
void rtos_mdelay (uint32_t ms) { 
    
  uint32_t ms_end = rtos_systick + ms;
    
  while ((ms_end - rtos_systick) > 0) {
      ;
  }
}

/**
 * \brief  ����ϵ��ʱ�������������, ��10msΪһ��ʱ����λ
 */
void rtos_sched_mdelay ( uint32_t ms)    
{   
    uint32_t status = rtos_task_critical_entry(); 
    
    p_current_task->delay_ticks = ms;
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 

    /*
     * Ȼ����������л����л�����һ�����񣬻��߿�������
     * delay_tikcs����ʱ���ж����Զ���1.������0ʱ�����л�������������
     */
    rtos_task_sched();              
}
    
    
 
 
 /* end of file */

