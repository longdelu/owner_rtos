#ifndef __RTOS_TASK_DELAY_H
#define __RTOS_TASK_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"    


/**
 * \brief  返回当前操作系统滴答时钟计数
 *
 * \param[in] 无
 *
 * \return    当前操作系统滴答时钟计数
 */    
uint32_t rtos_get_systick(void); 
    

/**
 * \brief  返回当前操作系统滴答时钟计数
 *
 * \param[in] 延时周期，单位周期，最小延时为1ms
 *
 * \return    无
 */      
void rtos_mdelay (int ms);   


/**
 * \brief  操作系统滴答时钟初始化函数
 *
 * \param[in] ms: 滴答定时回调周期，单位ms
 *
 * \return    无
 */
void rtos_systick_init(uint32_t ms);



#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_DELAY_H */
