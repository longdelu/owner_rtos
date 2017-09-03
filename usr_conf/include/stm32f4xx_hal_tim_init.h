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
  * @param  p_tim_handler:    定时器句柄
  * @param  p_tim_base:       定时器基址  
  * @param  arr:              重载计数值  
  * @param  psc:              分频计数值        
  * @retval None
  */
/**
  * @brief  This function Initializes tim
  * @retval None
  */
void stm32f4xx_hal_tim_init(TIM_HandleTypeDef *p_tim_handler, TIM_TypeDef *p_tim_base, uint32_t arr, uint32_t psc);

     
//extern TIM_HandleTypeDef TIM2_Handler;      /** < \brief 定时器2句柄  */
extern TIM_HandleTypeDef TIM3_Handler;      /** < \brief 定时器3句柄  */
//extern TIM_HandleTypeDef TIM4_Handler;      /** < \brief 定时器4句柄  */
//extern TIM_HandleTypeDef TIM5_Handler;      /** < \brief 定时器5句柄  */
//extern TIM_HandleTypeDef TIM9_Handler;      /** < \brief 定时器9句柄  */
//extern TIM_HandleTypeDef TIM10_Handler;     /** < \brief 定时器10句柄 */
//extern TIM_HandleTypeDef TIM11_Handler;     /** < \brief 定时器11句柄 */
//extern TIM_HandleTypeDef TIM12_Handler;     /** < \brief 定时器12句柄 */
//extern TIM_HandleTypeDef TIM13_Handler;     /** < \brief 定时器13句柄 */
//extern TIM_HandleTypeDef TIM14_Handler;     /** < \brief 定时器14句柄 */     

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_TIM_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




