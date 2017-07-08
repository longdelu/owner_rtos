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
 
 uint32_t rtos_systick = 0;
 
/**
 * \brief  ����ϵͳ�δ�ʱ�ӳ�ʼ������
 */
void rtos_systick_init(uint32_t ms)
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
 * \param[in] ms: 
 * \param[in] pin       : ���ű�ţ�ֵΪ PIO* (#PIO0_0)
 *
 * \return    ��
 */
void SysTick_Handler (void) 
{
    rtos_systick++;
}


 
 
 /* end of file */

