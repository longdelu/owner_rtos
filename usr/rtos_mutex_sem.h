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
    
    /** \brief 当前事件标记 */
    uint32_t loced_count;
    
    /** \brief 互斥信号量拥有者 */ 
    rtos_task_t *p_task_ower;
    
    /** \brief 互斥信号量拥有者的原先优先级 */  
    uint32_t task_ower_prior;
    
}rtos_mutex_sem_t;


/**
 * \brief 初始化互斥信号量
 * \param[in] p_mutex_sem: 指向互斥信号量结构体的指针
 * \return 无
 */  
void rtos_mutex_sem_init (rtos_mutex_sem_t *p_mutex_sem);
    

    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */


