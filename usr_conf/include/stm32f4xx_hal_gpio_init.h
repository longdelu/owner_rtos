/**
  ******************************************************************************
  * @file    stm32f4xx_hal_gpio_init.h
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   Header file of GPIO_INIT HAL module.
  ***********************************************************************  *******
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_GPIO_INIT_H
#define __STM32F4xx_HAL_GPIO_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
     
     
     
/**
  * @brief  This function Initializes  gpio
  * @param  gpio_pin:          GPIO管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @param  active_low:       是否低电平点亮
  * @retval None
  */
void stm32f4xx_gpio_init(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_port, uint8_t active_low);

/**
  * @brief  light on gpio
  * @param  gpio_pin:          GPIO管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @retval None  
  */     
void stm32f4xx_gpio_set(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_por);     

/**
  * @brief  light off gpio
  * @param  gpio_pin:          GPIO管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @retval None  
  */  
void stm32f4xx_gpio_get(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_port);       

/**
  * @brief  gpio state toggle
  * @param  gpio_pin:          GPIO管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @retval None  
  */  
void stm32f4xx_gpio_toggle(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_port);  

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_GPIO_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
