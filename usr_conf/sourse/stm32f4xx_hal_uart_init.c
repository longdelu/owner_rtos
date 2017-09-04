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

#if SUPPORT_OS   /* 使用OS */    

#include "rtos_task_critical.h"

#endif

/* UART句柄 */
UART_HandleTypeDef UART1_Handler;


uint8_t USART_RX_BUF[USART_REC_LEN];     /* 接收缓冲,最大USART_REC_LEN个字节. */

//接收状态
//bit15，    接收完成标志
//bit14，    接收到0x0d
//bit13~0，    接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记    

uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲


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
                                                                  
#ifdef ENABLE_UART_REC_INT        
        HAL_NVIC_EnableIRQ(USART1_IRQn);          //使能USART1中断通道  
#endif
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
    
#ifdef ENABLE_UART_REC_INT
    __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);                    //开启接收中断

#endif    
    
}

#ifdef ENABLE_UART_REC_INT

/** \brief 串口1中断服务程序 */
void USART1_IRQHandler(void)                    
{ 
    uint8_t Res = 0;
    
#if SUPPORT_OS   /* 使用OS */    
    rtos_interupt_enter();    
#endif
    
    /* 注意,读取USARTx->SR能避免莫名其妙的错误 */
    if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
        if((USART_RX_STA&0x8000)==0)//接收未完成
        {
            if(USART_RX_STA&0x4000)//接收到了0x0d
            {
                if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
                else USART_RX_STA|=0x8000;    //接收完成了 
            }
            else //还没收到0X0D
            {    
                if(Res==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收      
                }         
            }
        }            
    }
    
    HAL_UART_IRQHandler(&UART1_Handler); 
    
#if SUPPORT_OS   /* 使用OS */    
    rtos_interupt_exit();    
#endif    
}

#endif
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 /* end of file */ 

  

