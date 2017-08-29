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
 * \brief 串口初始化函数实现
 * \internal
 * \par Modification history
 * - 1.00 17-08-10  nwt, first implementation.
 * \endinternal
 */
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"



/**
  * @brief  This function call by HAL_GPIO_Init, set the uart pin and clk 
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *p_huart_base)
{
    /* GPIO端口设置 */
    GPIO_InitTypeDef GPIO_Initure;
    
    if(p_huart_base->Instance==USART1) {          //如果是串口1，进行串口1 MSP初始化
        __HAL_RCC_GPIOA_CLK_ENABLE();             //使能GPIOA时钟
        __HAL_RCC_USART1_CLK_ENABLE();            //使能USART1时钟
    
        GPIO_Initure.Pin=GPIO_PIN_9;              //PA9
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;        //复用推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;            //上拉
        GPIO_Initure.Speed=GPIO_SPEED_FAST;       //高速
        GPIO_Initure.Alternate=GPIO_AF7_USART1;   //复用为USART1
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);       //初始化PA9

        GPIO_Initure.Pin=GPIO_PIN_10;             //PA10
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);       //初始化PA10
          
    }

}
 
/**
  * @brief  This function Initializes uart
  * @retval HAL status
  */
void stm32f4xx_uart_init(UART_HandleTypeDef *p_uart_handler, USART_TypeDef *p_uart_base, uint32_t bound_rate)
{    
    /* UART 初始化设置 */
    p_uart_handler->Instance=p_uart_base;                        //USART1
    p_uart_handler->Init.BaudRate=bound_rate;                    //波特率
    p_uart_handler->Init.WordLength=UART_WORDLENGTH_8B;          //字长为8位数据格式
    p_uart_handler->Init.StopBits=UART_STOPBITS_1;               //一个停止位
    p_uart_handler->Init.Parity=UART_PARITY_NONE;                //无奇偶校验位
    p_uart_handler->Init.HwFlowCtl=UART_HWCONTROL_NONE;          //无硬件流控
    p_uart_handler->Init.Mode=UART_MODE_TX_RX;                   //收发模式
    HAL_UART_Init(p_uart_handler);                               //HAL_UART_Init()会使能UART
    
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 /* end of file */ 

  

