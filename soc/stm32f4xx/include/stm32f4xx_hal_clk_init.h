/**
  ******************************************************************************
  * @file    stm32f4xx_hal_clk_init.h
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   Header file of CLK_INIT HAL module.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_CLK_INIT_H
#define __STM32F4xx_HAL_CLK_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
     
/**
 * \brief clk device information.
 */
typedef struct stm32f4xx_clk_devinfo {

    /** \brief apb1时钟分频数，可填 1,2,4,8,16 之间数值 */    
    uint32_t apb1_div;
    
    /** \brief apb2时钟分频数，可填 1,2,4,8,16 之间数值 */    
    uint32_t apb2_div;
    
    /** \brief ahb时钟分频数，可填 1- 512 之间数值 */
    uint32_t ahb_div
    ;
    /** \brief 主pll倍频系数 */
    uint32_t plln;
    
    /** \brief 输入主pll和音频pll之前分频系数 */
    uint32_t pllm;    

    /** \brief 输入主pll之后的分频系数 */
    uint32_t pllp;    
    
    /** \brief usb/sdio/随机数产生器等的主分频系数 */
    uint32_t pllq;   

} stm32f4xx_clk_devinfo_t;


/**
 * \brief CLK 设备结构体
 */
typedef struct stm32f4xx_clk_dev {

    /** \brief 指向CLK 设备信息的指针   */
    const stm32f4xx_clk_devinfo_t *p_devinfo;

    /** \brief system clock frequency,divIde from main clock. */
    uint32_t sysclk;   
    
    /** \brief  the ahb clock frequency */
    uint32_t ahb_clk;

    /** \brief  the apb clock frequency */
    uint32_t apb1_clk;

    /** \brief  the apb clock frequency */
    uint32_t apb2_clk;

} stm32f4xx_clk_dev_t;


extern  stm32f4xx_clk_devinfo_t clk_info;

extern stm32f4xx_clk_dev_t clk_dev;

     

/**
  * @brief  This function Initializes  clk
  * @param  p_dev:    时钟设备结构体的指针
  * @param  p_deinfo: 时钟设备信息结构体的指针
  * @retval HAL status
  */
int stm32f4xx_hal_clk_init(stm32f4xx_clk_dev_t *p_dev, stm32f4xx_clk_devinfo_t *p_deinfo);


/**
  * @brief 获取时钟频率
  *             
  * @param  clk_id: 0为获取系统时钟频率，1为AHB时钟频率; 2为APB1时钟频率; 3为APB2时钟频率
  *
  * @retval the clk frq;
  */
uint32_t stm32f4xx_clk_frq_get (uint32_t clk_id);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_CLK_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
