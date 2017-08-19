#ifndef __RTOS_TASK_SWITCH_H
#define __RTOS_TASK_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtos_task.h"
    
/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#if  (__FPU_PRESENT == 1)
#define  RTOS_CPU_ARM_FP_EN                              1
#else
#define  RTOS_CPU_ARM_FP_EN                              0
#endif

#define  RTOS_CPU_ARM_FP_REG_NBR                         32u    
    


/**
 * \brief 触发PendSVC
 * \param[in] 无
 * \return    无
 */
void PendSVC_Trigger(void);
    
    
/**
 * \brief 任务切换
 * \param[in] 无
 * \return    无
 */
void rtos_task_switch (void);

#if (RTOS_CPU_ARM_FP_EN == 1)
/**
 * \brief 硬件浮点进栈
 * \param[in] p_fp_stack 硬件浮点的堆栈指针
 * \return    无
 */
void  rtos_cpu_fp_reg_push (void);

/**
 * \brief 硬件浮点出栈
 * \param[in] p_fp_stack 硬件浮点的堆栈指针
 * \return    无
 */
void  rtos_cpu_fp_reg_pop (void);

#endif




#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_SWITCH_H */



