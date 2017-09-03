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
 * \brief led初始化函数实现
 * \internal
 * \par Modification history
 * - 1.00 17-08-10  nwt, first implementation.
 * \endinternal
 */
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h" 

static uint8_t __low_active_state = 0;

static void __gpio_port_enable (GPIO_TypeDef *p_gpio_port) 
{
    uint32_t port_base = (uint32_t)p_gpio_port;
    switch (port_base) {
    case (uint32_t) GPIOA :
        __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟        
        break;
    
    case (uint32_t) GPIOB :
        __HAL_RCC_GPIOB_CLK_ENABLE();          //开启GPIOB时钟           
        break;  
    
    case (uint32_t) GPIOC :
        __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟           
        break;
    
    case (uint32_t) GPIOD :
        __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOD时钟          
        break;
    
    case (uint32_t) GPIOE :
        __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟          
        break;
    
    case (uint32_t) GPIOF :
        __HAL_RCC_GPIOF_CLK_ENABLE();           //开启GPIOF时钟        
        break;
    
    case (uint32_t) GPIOG :
        __HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOG时钟        
        break;
    
    case (uint32_t) GPIOH :
        __HAL_RCC_GPIOH_CLK_ENABLE();           //开启GPIOH时钟        
        break;
    
    case (uint32_t) GPIOI :
        __HAL_RCC_GPIOI_CLK_ENABLE();           //开启GPIOI时钟        
        break; 
    
    case (uint32_t) GPIOJ :
        __HAL_RCC_GPIOJ_CLK_ENABLE();           //开启GPIOJ时钟        
        break;
    
    case (uint32_t) GPIOK :
         __HAL_RCC_GPIOK_CLK_ENABLE();           //开启GPIOK时钟       
        break; 

    default:
        break;            
        

    }        
    
}


/**
  * @brief  This function Initializes led
  * @retval None
  */
void stm32f4xx_led_init(uint32_t led_pin, GPIO_TypeDef *p_gpio_port, uint8_t active_low)
{ 
    GPIO_InitTypeDef GPIO_Initure;
    
    __gpio_port_enable(p_gpio_port);        //开启对应端口时钟
    
    __low_active_state = active_low;
    
    
    /* 默认初始化后灯灭 */
    if (active_low == 1) {
        HAL_GPIO_WritePin(GPIOB,led_pin,GPIO_PIN_SET);  
    } else {
        HAL_GPIO_WritePin(GPIOB,led_pin,GPIO_PIN_RESET);  
         
    }
    
    GPIO_Initure.Pin=led_pin;               //LED管脚
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(p_gpio_port,&GPIO_Initure);   
}


/**
  * @brief  light on led
  */     
void stm32f4xx_led_on(uint32_t led_pin, GPIO_TypeDef *p_gpio_por)
{
     if (__low_active_state == 1) {
         HAL_GPIO_WritePin(GPIOB,led_pin,GPIO_PIN_RESET);           
                 
     } else {
         HAL_GPIO_WritePin(GPIOB,led_pin,GPIO_PIN_SET); 
     }
}    

/**
  * @brief  light off led
  */  
void stm32f4xx_led_off(uint32_t led_pin, GPIO_TypeDef *p_gpio_port)
{
     if (__low_active_state == 1) {
         HAL_GPIO_WritePin(GPIOB,led_pin,GPIO_PIN_SET);           
                 
     } else {
         HAL_GPIO_WritePin(GPIOB,led_pin,GPIO_PIN_RESET); 
     }
}    

/**
  * @brief  led state toggle
  */  
void stm32f4xx_led_toggle(uint32_t led_pin, GPIO_TypeDef *p_gpio_port)
{
     HAL_GPIO_TogglePin(p_gpio_port,led_pin);      
}    
 
 
 
 
 /* end of file */


  




