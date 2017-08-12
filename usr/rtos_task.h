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
    
/** \brief 任务数量 */    
#define TASK_COUNT    RTOS_PRIO_COUNT  
    
/** \brief 任务处于就绪状态 */   
#define RTOS_TASK_STATE_REDDY      (1UL << 0)  
    
/** \brief 任务处于延时状态 */   
#define RTOS_TASK_STATE_DELAYED    (1UL << 1)     
    
/** \brief 任务处于挂起状态 */   
#define RTOS_TASK_STATE_SUSPEND    (1UL << 2)   

/** \brief 任务被请求删除标记 */   
#define RTOS_TASK_STATE_RED_DEL    (1UL << 3)  



/** \brief 任务处于等待事件状态 */
#define RTOS_TASK_EVENT_WAIT_MASK  (0xFF << 16)

/** \brief 任务处于等待事件状态 */
#define RTOS_TASK_TEST_EVENT_WAIT  (1UL << 16)
    
/** \brief Cortex-M的堆栈单元类型：堆栈单元的大小为32位，所以使用uint32_t */
typedef uint32_t taskstack_t;


/** \brief 事件控制块前置声明 */
struct rtos_task_event;
    
typedef struct rtos_task {
   
    /** \brief 堆栈栈顶地址   */    
    uint32_t *task_stack_top;
    
    /** \brief 堆栈的起始地址 */
    uint32_t *stack_base;

    /** \brief 堆栈的总容量   */
    uint32_t stack_size;
    
    /** \brief 任务延时计数器 */
    uint32_t delay_ticks;

    /** \brief 任务的优先级   */    
    uint32_t prio;
    
    /** \brief 任务的延时结点，通过该结点将任务放置到延时队列中   */     
    dlist_node_t delay_node;

    /** \brief 同一优先级任务链表，通过该结点将任务放置到同一优先级任务队列中，用于就绪表   */     
    dlist_node_t prio_node;
       
    /** \brief 任务等待事件的结点，通过该结点将任务放置等待事件链表中  */     
    dlist_node_t event_node;
       
    /** \brief 任务的状态   */      
    uint32_t task_state;
    
    /** \brief 任务的时间片计数   */      
    uint32_t slice;   

    /** \brief 任务被挂起的次数   */      
    uint32_t suspend_cnt;    
    
    /** \brief 任务被删除时调用的清理函数 */
    void (*pfn_clean) (void * p_par);

    /** \brief 传递给清理函数的参数 */
    void *clean_param;

    /** \brief 请求删除标志，非0表示请求删除*/
    uint8_t req_delete_flag; 

    /** \brief 任务正在等待的事件类型,加入事件等待链表或移除会被设置与清除 */
    struct rtos_task_event *p_event;
    
    /** \brief 等待事件的消息存储位置，加入事件等待链表或移除会被设置与清除 */
    void *p_event_msg;
    
    /** \brief 等待事件的结果，加入事件等待链表或移除会被设置与清除 */
    int32_t event_wait_result;
    
    /** \brief 等待的事件标记组方式 */
    uint32_t wait_flags_grp_type;

    /** \brief 等待的事件标记组的标志 */
    uint32_t event_flags_grp;    

            
}rtos_task_t;


typedef struct rtos_task_info {
    /** \brief 任务的滴答计数   */     
    uint32_t delay_ticks;
    
    /** \brief 任务的优先级   */     
    uint32_t task_prio;
    
    /** \brief 任务的状态   */   
    uint32_t task_state;
    
    /** \brief 任务的时间片计数   */ 
    uint32_t task_slice;
    
    /** \brief 任务被挂起的次数   */    
    uint32_t suspend_cnt;
        
    /** \brief 堆栈的总容量   */
    uint32_t stack_size;
    
    /** \brief 堆栈空余量   */    
    uint32_t stack_free;
   
}rtos_task_info_t;


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
 * \brief 将延时的任务从延时队列中唤醒
 * \param[in] p_task: 任务结构体指针   
 * \return 无  
 */
void rtos_task_wake_up_delayed_list (rtos_task_t *p_task);

/**
 * \brief 将延时的任务从延时队列中删除
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


/**
 * \brief 设置任务被删除时调用的清理函数
 * \param[in] p_task: 任务结构体指针 
 * \param[in] pfn_clean: 指向任务清理函数的函数指针   
 * \param[in] p_par: 传递给清理函数的参数   
 * \return 无 
 */
void rtos_task_set_clean_call_fuc (rtos_task_t *p_task, void (*pfn_clean)(void * p_par), void * p_par);


/**
 * \brief 强制删除指定的任务
 * \param[in] p_task: 任务结构体指针   
 * \return 无 
 */
void rtos_task_force_del (rtos_task_t *p_task);


/**
 * \brief 请求删除某个任务，由任务自己决定是否删除自己
 * \param[in] p_task: 任务结构体指针   
 * \return 无 
 */
void rtos_task_req_del (rtos_task_t *p_task);


/**
 * \brief 查询是否已经被请求删除自已
 * \param[in] 无   
 * \return 无 
 */
uint32_t rtos_task_req_del_flag_check (void);


/**
 * \brief 任务删除自身
 * \param[in] 无
 * \return 无 
 */
void rtos_task_del_self (void);


/**
 * \brief 获取任务相关信息
 * \param[in] p_task: 任务结构体指针
 * \param[in] p_task_info: 任务信息结构体指针    
 * \return 无 
 */
void rtos_task_info_get (rtos_task_t *p_task, rtos_task_info_t *p_task_info);


#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_H */

/* end of file */

