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
 * \brief 操作系统普通延时函数
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "c_lib.h"
 
extern  uint32_t rtos_systick;
 
/**
 * \brief  返回系统当前滴答计数
 */ 
uint32_t rtos_get_systick(void) 
{ 
     return rtos_systick; 
}

/**
 * \brief  操作系统普通延时，没有任务调度
 */
void rtos_mdelay(int ms) { 
    
  uint32_t ms_end = rtos_systick + ms;
    
  while ((ms_end - rtos_systick) > 0) {
      ;
  }
}
 
 
 /* end of file */

