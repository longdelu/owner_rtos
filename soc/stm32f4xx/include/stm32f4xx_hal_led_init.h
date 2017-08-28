/**
  ******************************************************************************
  * @file    stm32f4xx_hal_led_init.h
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   Header file of LED_INIT HAL module.
  ***********************************************************************  *******
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_LED_INIT_H
#define __STM32F4xx_HAL_LED_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
     
/**
  * @brief  This function Initializes  led
  * @param  led_pin:          LED¹Ü½Å
  * @param  p_gpio_port:      GPIO¹Ü½Å´®¿Ú       
  * @retval None
  */
void stm32f4xx_led_init(uint32_t led_pin, GPIO_TypeDef *p_gpio_port);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_LED_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
