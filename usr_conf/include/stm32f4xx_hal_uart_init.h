/**
  ******************************************************************************
  * @file    stm32f4xx_hal_uart_init.h
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   Header file of UART_INIT HAL module.
  ***********************************************************************  *******
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_UART_INIT_H
#define __STM32F4xx_HAL_UART_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
     
/**
  * @brief  This function Initializes  uart
  * @param  p_uart_handler:    ´®¿Ú¾ä±ú
  * @param  p_uart_base:       ´®¿Ú  
  * @param  bound_rate:        ´®¿Ú²¨ÌØÂÊ          
  * @retval None
  */
void stm32f4xx_uart_init(UART_HandleTypeDef *p_uart_handler, USART_TypeDef *p_uart_base, uint32_t bound_rate);
     
     
/* UART¾ä±ú */
extern UART_HandleTypeDef UART1_Handler;


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_UART_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
