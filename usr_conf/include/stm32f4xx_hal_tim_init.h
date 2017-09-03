/**
  ******************************************************************************
  * @file    stm32f4xx_hal_tim_init.h
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   Header file of TIM_INIT HAL module.
  ***********************************************************************  *******
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_TIM_INIT_H
#define __STM32F4xx_HAL_TIM_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
     
/**
  * @brief  This function Initializes  tim
  * @param  p_tim_handler:    ��ʱ�����
  * @param  p_tim_base:       ��ʱ����ַ  
  * @param  arr:              ���ؼ���ֵ  
  * @param  psc:              ��Ƶ����ֵ        
  * @retval None
  */
/**
  * @brief  This function Initializes tim
  * @retval None
  */
void stm32f4xx_hal_tim_init(TIM_HandleTypeDef *p_tim_handler, TIM_TypeDef *p_tim_base, uint32_t arr, uint32_t psc);

     
//extern TIM_HandleTypeDef TIM2_Handler;      /** < \brief ��ʱ��2���  */
extern TIM_HandleTypeDef TIM3_Handler;      /** < \brief ��ʱ��3���  */
//extern TIM_HandleTypeDef TIM4_Handler;      /** < \brief ��ʱ��4���  */
//extern TIM_HandleTypeDef TIM5_Handler;      /** < \brief ��ʱ��5���  */
//extern TIM_HandleTypeDef TIM9_Handler;      /** < \brief ��ʱ��9���  */
//extern TIM_HandleTypeDef TIM10_Handler;     /** < \brief ��ʱ��10��� */
//extern TIM_HandleTypeDef TIM11_Handler;     /** < \brief ��ʱ��11��� */
//extern TIM_HandleTypeDef TIM12_Handler;     /** < \brief ��ʱ��12��� */
//extern TIM_HandleTypeDef TIM13_Handler;     /** < \brief ��ʱ��13��� */
//extern TIM_HandleTypeDef TIM14_Handler;     /** < \brief ��ʱ��14��� */     

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_TIM_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




