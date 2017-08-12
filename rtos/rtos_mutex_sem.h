/**
 * \file
 * \brief RTOS 存储块头文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_MUTEX_SEM_H
#define __RTOS_MUTEX_SEM_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
    
    
    
/** \brief 互斥信号量结构体声明   */    
typedef struct rtos_mutex_sem {
    /** \brief 事件控制块   */
    rtos_task_event_t mutex_sem_event;
    
    /** \brief 互斥信号量嵌套计数，可以解决嵌套调用 */
    uint32_t loced_count;
    
    /** \brief 互斥信号量拥有者，防止其它任务执行释放信号量时出错，即其它任务执行释放操作无效 */ 
    rtos_task_t *p_task_ower;
    
    /** \brief 互斥信号量拥有者的原先优先级 */  
    uint32_t task_ower_prior;
    
}rtos_mutex_sem_t;


/** \brief 互斥信号量查询结构 */ 
typedef struct  rtos_mutex_sem_info
{
    /** \brief 等待的任务数量 */
    uint32_t task_count;

    /** \brief 拥有者任务的优先级 */
    uint32_t owner_task_prio;

    /** \brief 继承优先级 */
    uint32_t inherited_prio;

    /** \brief  当前信号量的拥有者 */
    rtos_task_t *p_task_ower;

    /** \brief 锁定次数 */
    uint32_t locked_count;
    
}rtos_mutex_sem_info_t;



/**
 * \brief 初始化互斥信号量
 * \param[in] p_mutex_sem: 指向互斥信号量结构体的指针
 * \return 无
 */  
void rtos_mutex_sem_init (rtos_mutex_sem_t *p_mutex_sem);


/**
 * \brief 等待互斥信号量
 * \param[in] p_mutex_sem: 指向互斥信号量结构体的指针
 * \param[in] wait_ticks:  当信号量计数为0时，等待的ticks数，为0时表示永远等待
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */  
int32_t rtos_mutex_sem_wait (rtos_mutex_sem_t *p_mutex_sem, uint32_t wait_ticks);

/**
 * \brief 获取互斥信号量，如果互斥信号量不可用，则立即退回
 * \param[in] p_mutex_sem: 指向互斥信号量结构体的指针
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK
 */  
int32_t rtos_mutex_sem_get (rtos_mutex_sem_t *p_mutex_sem);


/**
 * \brief 通知互斥信号量可用，唤醒等待队列中的一个任务，或者将计数+1
 * \param[in] p_mutex_sem: 指向互斥信号量结构体的指针
 * \return  无
 */  
int32_t rtos_mutex_sem_notify (rtos_mutex_sem_t *p_mutex_sem);


/**
 * \brief查询互斥信号量的状态信息
 * \param[in] p_mutex_sem:  指向互斥信号量结构体的指针
 * \param[in] p_info: 指向互斥信号量结构体的指针
 * \return  无
 */  
void rtos_mutex_sem_info_get (rtos_mutex_sem_t * p_mutex_sem, rtos_mutex_sem_info_t *p_info);

/**       
 * \brief 销毁互斥信号量
 * \param[in] p_mutex_sem:  指向互斥信号量结构体的指针
 * \return  因销毁该互斥信号量而唤醒的任务数量
 */  
uint32_t rtos_mutex_sem_destroy (rtos_mutex_sem_t *p_mutex_sem);

 
    

    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */


