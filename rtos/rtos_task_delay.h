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
uint32_t rtos_get_systick (void); 
    
    
/**
 * \brief  设置当前系统滴答计数值
 *
 * \param[in] rtos_systick_value_reload : 系统滴答计数值
 *
 * \return    无
 */ 
void rtos_set_systick (uint32_t rtos_systick_value_reload);    
    

/**
 * \brief  无任务调度的延时
 *
 * \param[in] 延时周期，单位周期，最小延时为10ms
 *   
 * \note      要换算成相对应的tick计数
 *
 * \return    无
 */      
void rtos_mdelay (int32_t ms);   


/**
 * \brief  操作系统滴答时钟初始化函数
 *
 * \param[in] ms: 滴答定时回调周期，单位ms
 *
 * \return    无
 */
void rtos_systick_init (uint32_t ms);


/**
 * \brief  滴答计数初始化
 * \param[in] 无
 * \return    无
 */
void rtos_tick_count_init (void);


/**
 * \brief  有任务调度的延时
 *
 * \param[in] 延时周期，单位周期，最小延时为10ms
 *   
 * \note      要换算成相对应的tick计数
 *
 * \return    无
 */      
void rtos_sched_mdelay (uint32_t ms);   



#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_DELAY_H */
