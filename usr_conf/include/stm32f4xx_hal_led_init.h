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
     
#define LED0 PBout(0)   /** \brief LED0 */
#define LED1 PBout(1)   /** \brief LED1 */
     
     
/**
  * @brief  This function Initializes  led
  * @param  led_pin:          LED管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @param  active_low:       是否低电平点亮
  * @retval None
  */
void stm32f4xx_led_init(uint32_t led_pin, GPIO_TypeDef *p_gpio_port, uint8_t active_low);

/**
  * @brief  light on led
  * @param  led_pin:          LED管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @retval None  
  */     
void stm32f4xx_led_on(uint32_t led_pin, GPIO_TypeDef *p_gpio_por);     

/**
  * @brief  light off led
  * @param  led_pin:          LED管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @retval None  
  */  
void stm32f4xx_led_off(uint32_t led_pin, GPIO_TypeDef *p_gpio_port);       

/**
  * @brief  led state toggle
  * @param  led_pin:          LED管脚
  * @param  p_gpio_port:      GPIO管脚串口 
  * @retval None  
  */  
void stm32f4xx_led_toggle(uint32_t led_pin, GPIO_TypeDef *p_gpio_port);  

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_LED_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
