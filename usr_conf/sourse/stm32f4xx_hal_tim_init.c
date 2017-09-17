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
#include "stm32f4xx_hal_led_init.h"

#if SUPPORT_OS   /* 使用OS */    

#include "rtos_task_critical.h"

#endif

//TIM_HandleTypeDef TIM2_Handler;      /** < \brief 定时器2句柄  */
TIM_HandleTypeDef TIM3_Handler;      /** < \brief 定时器3句柄  */
//TIM_HandleTypeDef TIM4_Handler;      /** < \brief 定时器4句柄  */
//TIM_HandleTypeDef TIM5_Handler;      /** < \brief 定时器5句柄  */
//TIM_HandleTypeDef TIM9_Handler;      /** < \brief 定时器9句柄  */
//TIM_HandleTypeDef TIM10_Handler;     /** < \brief 定时器10句柄 */
//TIM_HandleTypeDef TIM11_Handler;     /** < \brief 定时器11句柄 */
//TIM_HandleTypeDef TIM12_Handler;     /** < \brief 定时器12句柄 */
//TIM_HandleTypeDef TIM13_Handler;     /** < \brief 定时器13句柄 */
//TIM_HandleTypeDef TIM14_Handler;     /** < \brief 定时器14句柄 */

static void __tim_init_enable (TIM_TypeDef *p_tim_base) 
{
    uint32_t tim_base = (uint32_t)p_tim_base;
    switch (tim_base) {
    case (uint32_t) TIM2 :
        __HAL_RCC_TIM2_CLK_ENABLE();           //开启TIM2时钟 
        HAL_NVIC_EnableIRQ(TIM2_IRQn);         //开启ITM2中断       
        break;
    
    case (uint32_t) TIM3 :
        __HAL_RCC_TIM3_CLK_ENABLE();          //开启TIM3时钟 
        HAL_NVIC_EnableIRQ(TIM3_IRQn);        //开启ITM3中断     
        break;  
    
    case (uint32_t) TIM4 :
        __HAL_RCC_TIM4_CLK_ENABLE();           //开启TIM4时钟  
        HAL_NVIC_EnableIRQ(TIM4_IRQn);         //开启ITM4中断        
        break;
    
    case (uint32_t) TIM5 :
        __HAL_RCC_TIM5_CLK_ENABLE();           //开启TIM5时钟 
        HAL_NVIC_EnableIRQ(TIM5_IRQn);         //开启ITM5中断     
        break;
    
    case (uint32_t) TIM9 :
        __HAL_RCC_TIM9_CLK_ENABLE();           //开启TIM9时钟  
        HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);//开启ITM9中断     
        break;
    
    case (uint32_t) TIM10 :
        __HAL_RCC_TIM10_CLK_ENABLE();           //开启TIM10时钟        
        break;
    
    case (uint32_t) TIM11 :
        __HAL_RCC_TIM11_CLK_ENABLE();           //开启TIM11时钟        
        break;
    
    case (uint32_t) TIM12 :
        __HAL_RCC_TIM12_CLK_ENABLE();           //开启TIM12H时钟        
        break;
    
    case (uint32_t) TIM13 :
        __HAL_RCC_TIM13_CLK_ENABLE();           //开启TIM13时钟        
        break; 
    
    case (uint32_t) TIM14 :
        __HAL_RCC_TIM14_CLK_ENABLE();           //开启TIM14时钟        
        break;

    default:
        break;            
        
    }          
}



/**
  * @brief  This function call by HAL_GPIO_Init, set the uart pin and clk 
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *p_tim_handler)
{
     __tim_init_enable(p_tim_handler->Instance);             //使能TIM时钟
}

 
/**
  * @brief  This function Initializes tim
  * @retval None
  */
void stm32f4xx_hal_tim_init(TIM_HandleTypeDef *p_tim_handler, TIM_TypeDef *p_tim_base, uint32_t arr, uint32_t psc)
{    
    p_tim_handler->Instance=p_tim_base;                       //定时器基址
    p_tim_handler->Init.Prescaler=psc;                        //分频系数
    p_tim_handler->Init.CounterMode=TIM_COUNTERMODE_UP;       //向上计数器
    p_tim_handler->Init.Period=arr;                           //自动装载值
    p_tim_handler->Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //时钟分频因子
    HAL_TIM_Base_Init(p_tim_handler);
    
    HAL_TIM_Base_Start_IT(p_tim_handler);                     //使能定时器和定时器更新中断：TIM_IT_UPDATE                          
    
}


/**
 * \brief 定时器3中断服务函数
 */
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
    

}


/**
 * \brief 回调函数，在定时器中断服务函数调用
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
#if SUPPORT_OS   /* 使用OS */    
    rtos_interupt_enter();    
#endif
  
    if(htim==(&TIM3_Handler))
    {
        LED1=!LED1;        /* LED1反转 */
#ifdef LWIP_NO_SUPPORT_OS
        extern uint32_t lwip_localtime;           //lwip本地时间计数器,单位:ms
        lwip_localtime += 10;
#endif
        
    }
    
#if SUPPORT_OS   /* 使用OS */    
    rtos_interupt_exit();    
#endif  
    
}


 
/* end of file */ 







             




