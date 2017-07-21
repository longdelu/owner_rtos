#ifndef __RTOS_TASK_H
#define __RTOS_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "c_lib.h"
#include "rtos_config.h"    
#include "rtos_task_bitmap.h"
#include "rtos_task_list.h"
#include "rtos_list.h"
    
/**
 * \brief 任务数量
 */    
#define TASK_COUNT    RTOS_PRIO_COUNT  
    
/** \brief 任务处于就绪状态 */   
#define RTOS_TASK_STATE_REDDY      (1UL << 0)  
    
/** \brief 任务处于延时状态 */   
#define RTOS_TASK_STATE_DELAYED    (1UL << 1)     
    
/** \brief 任务处于挂起状态 */   
#define RTOS_TASK_STATE_SUSPEND    (1UL << 2)   
    
/* Cortex-M的堆栈单元类型：堆栈单元的大小为32位，所以使用uint32_t */
typedef uint32_t taskstack_t;
    
typedef struct rtos_task {
   
    /** \brief 堆栈栈顶地址   */    
    uint32_t *task_stack_top;
    
    /** \brief 堆栈的起即地址 */
    uint32_t * stack_base;

    /** \brief 堆栈的总容量   */
    uint32_t stack_size;
    
    /** \brief 任务延时计数器 */
    uint32_t delay_ticks;

    /** \brief 任务的优先级   */    
    uint32_t prio;
    
    /** \brief 任务的延时结点，通过该结点将任务放置到延时队列中   */     
    dlist_node_t delay_node;

    /** \brief 同一优先级任务链表，通过该结点将任务放置到同一优先级任务队列中   */     
    dlist_node_t prio_node;
       
    /** \brief 任务的状态   */      
    uint32_t task_state;
    
    /** \brief 任务的时间片计数   */      
    uint32_t slice;   

    /** \brief 任务被挂起的次数   */      
    uint32_t suspend_cnt;    
        
}rtos_task_t;


/** \brief 当前任务：记录当前是哪个任务正在运行 */
extern rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
extern rtos_task_t * p_next_task;


/** \brief 所有任务的指针数组：简单起见，只使用两个任务 */
extern rtos_task_t * p_task_table[TASK_COUNT];

/** \brief  空闲任务结构体指针 */
extern rtos_task_t * p_idle_task;

/** \brief 任务优先级的标记位置结构全变量 */
extern rtos_task_bitmap_t task_priobitmap;


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
 * \brief 初始化优先级任务调度链表  
 * 
 * \param[in] p_task_table: 化先级任务调度表
 *                    
 * \return 无                  
 */
void rtos_task_sched_init(rtos_task_list_t *p_task_table);                    
                    
                    
/**
 * \brief 将任务设置为就绪状态 
 * 
 * \param[in] task: 任务结构体指针   
 * \return 无                    
 */
void rtos_task_sched_ready(rtos_task_t *task);


/**
 * \brief 将任务从绪列表中移除
 * \param[in] task: 任务结构体指针   
 * \return 无                       
 */
void rtos_task_sched_unready(rtos_task_t *task);                   
                    
                                       
/**
 * \brief 获取当前最高优先级且可运行的任务
 *                    
 * \param[in] 无
 *
 * \return    优先级最高的且可运行的任务结构体指针                                      
 */
rtos_task_t *rtos_task_highest_ready(void);                    
                    
                    
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


/**
 * \brief 初始化任务延时队列
 * \param[in] p_rtos_task_delayed_list : 指向任务延时队列的指针
 * \return 无
 */
void rtos_task_delayed_init (rtos_task_list_t *p_rtos_task_delayed_list);


/**
 * \brief 将任务加入到延时队列中
 * \param[in] p_task: 任务结构体指针   
 * \param[in] delay_ticks: 延时的ticks
 * \return 无  
 */
void rtos_task_add_delayed_list (rtos_task_t * p_task, uint32_t delay_ticks);


/**
 * \brief 将任务从延时队列中唤醒
 * \param[in] p_task: 任务结构体指针   
 * \return 无  
 */
void rtos_task_del_delayed_list (rtos_task_t *p_task);


/**
 * \brief 挂起任务
 * \param[in] p_task: 任务结构体指针   
 * \return 无 
 */
void rtos_task_suspend (rtos_task_t *p_task);


/**
 * \brief 挂起任务
 * \param[in] p_task: 任务结构体指针   
 * \return 无 
 */
void rtos_task_wakeup (rtos_task_t *p_task);


#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_H */

/* end of file */

