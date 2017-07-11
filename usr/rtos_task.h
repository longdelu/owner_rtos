#ifndef __RTOS_TASK_H
#define __RTOS_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "c_lib.h"  
    
/**
 * \brief 任务数量
 */    
#define TASK_COUNT    2    
    
/* Cortex-M的堆栈单元类型：堆栈单元的大小为32位，所以使用uint32_t */
typedef uint32_t taskstack_t;
    
typedef struct rtos_task {
   
    /** \brief 堆栈栈顶地址 */    
    uint32_t *task_stack_top;
    
    /** \brief 堆栈的起即地址 */
    uint32_t * stack_base;

    /** \brief 堆栈的总容量 */
    uint32_t stack_size;
    
    /** \brief 任务延时计数器 */
    uint32_t delay_ticks;
    
}rtos_task_t;


/** \brief 当前任务：记录当前是哪个任务正在运行 */
extern rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
extern rtos_task_t * p_next_task;


/** \brief 所有任务的指针数组：简单起见，只使用两个任务 */
extern rtos_task_t * p_task_table[TASK_COUNT];

/** \brief  空闲任务结构体指针 */
extern rtos_task_t * p_idle_task;


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

