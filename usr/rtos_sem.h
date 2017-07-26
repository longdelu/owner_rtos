/**
 * \file
 * \brief RTOS配置文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_SEM_H
#define __RTOS_SEM_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
typedef struct rtos_sem {
    
    /** \brief 事件控制块 */
    rtos_task_event_t sem_event;

    /** \brief 信号量计数 */
    uint32_t sem_count;

    /** \brief 信号量最大计数 */
    uint32_t max_count;    
       
}rtos_sem_t;


/**
 * \brief 初始化计数信号量
 * \param[in] p_sem: 指向计数信号量结构体的指针
 * \param[in] start_count:  信号量的初始计数
 * \param[in] max_count: 信号量的最大计数，如果为0，则信号量计数理论上不限数量
 * \return  无
 */  
void rtos_sem_init(rtos_sem_t *p_sem, uint32_t start_count, uint32_t max_count);


/**
 * \brief 等待计数信号量
 * \param[in] p_sem: 指向计数信号量结构体的指针
 * \param[in] wait_ticks:  当信号量计数为0时，等待的ticks数，为0时表示永远等待
 * \return  等待结果,RTOS_UNUSED.RTOS_OK,RTOS_TIMEOUT
 */  
int32_t rtos_sem_wait (rtos_sem_t *p_sem, uint32_t wait_ticks);

/**
 * \brief 获取信号量，如果信号量计数不可用，则立即退回
 * \param[in] p_sem: 指向计数信号量结构体的指针
 * \return  等待结果,RTOS_UNUSED.RTOS_OK,RTOS_TIMEOUT
 */  
int32_t rtos_sem_get (rtos_sem_t *p_sem);


/**
 * \brief 通知信号量可用，唤醒等待队列中的一个任务，或者将计数+1
 * \param[in] p_sem: 指向计数信号量结构体的指针
 * \return  无
 */  
void rtos_sem_notify (rtos_sem_t *p_sem);



    
    
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_SEM_H */

