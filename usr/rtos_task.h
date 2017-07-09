#ifndef __RTOS_TASK_H
#define __RTOS_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "c_lib.h"  
    
/* Cortex-M的堆栈单元类型：堆栈单元的大小为32位，所以使用uint32_t */
typedef uint32_t taskstack_t;
    
typedef struct rtos_task {
   
    /** \brief 堆栈栈顶地址 */    
    uint32_t *task_stack_top;
    
    /** \brief 堆栈的起即地址 */
    uint32_t * stack_base;

    /** \brief 堆栈的总容量 */
    uint32_t stack_size;
    
}rtos_task_t;

/**
 * \brief  任务初始化函数
 *
 * \param[in] task: 任务结构体指针
 * \param[in] task_entry: 任务入口函数
 * \param[in] p_arg: 任务入口函数参数
 * \param[in] task_prio: 任务的优先级
 * \param[in] task_stack: 任务堆栈的指针
 * \param[in] task_stack_size: 任务堆栈的大小,以字节为单位
 *
 * \return    无
 */
void rtos_task_init(rtos_task_t * task, 
                    void (*task_entry) (void *p_arg), 
                    void *p_arg, 
                    uint32_t task_prio, 
                    uint32_t *task_stack,
                    uint32_t task_stack_size);
                    
                    
/**
 * \brief 切换到第一个任务运行，启动操作系统第一个任务
 *                    
 * \param[in] 无
 *
 * \return    无                                      
 */
void rtos_task_run_first (void);                    
                    
                    
/**
 * \brief 任务调度函数 
 *                    
 * \param[in] 无
 *
 * \return    无
 */
void rtos_task_sched(void);                    


#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_H */

/* end of file */

