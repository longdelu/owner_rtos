/*******************************************************************************
*                                 Embedded_rtos
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Free soft.
* All rights reserved.
*
* Contact information:
* web site:    
* e-mail:      
*******************************************************************************/

/**
 * \file
 * \brief RTOS 事件控制块指针
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#ifndef __RTOS_FLAGGROUP_H
#define __RTOS_FLAGGROUP_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
    
#define RTOS_FALG_GRP_CLR        (0UL << 0)         /** \brief 事件标记组位清0 */
#define RTOS_FALG_GRP_SET        (1UL << 0)         /** \brief 事件标记组位置1 */
#define RTOS_FALG_GRP_ANY        (0UL << 1)         /** \brief 事件标记组任意位变化 */
#define RTOS_FALG_GRP_ALL        (1UL << 1)         /** \brief 事件标记组所有位变化 */

/** \brief 事件标记组所有位置1 */    
#define RTOS_FLAG_GRP_ALL_SET    (RTOS_FALG_GRP_SET | RTOS_FALG_GRP_ALL) 
    
/** \brief事件标记组任何一位置1*/    
#define RTOS_FLAG_GRP_ANY_SET    (RTOS_FALG_GRP_SET | RTOS_FALG_GRP_ANY) 
    
/** \brief 事件标记组所有位清0 */
#define RTOS_FLAG_GRP_ALL_CLR    (RTOS_FALG_GRP_CLR | RTOS_FALG_GRP_ALL) 
    
/** \brief 事件标记组任何一位清0 */
#define RTOS_FLAG_GRP_ANY_CLR    (RTOS_FALG_GRP_CLR | RTOS_FALG_GRP_ANY)   

/** \brief 事件标记组被消耗     */
#define RTOS_FLAG_GRP_CONSUME    (1UL << 7)

/** \brief 事件标记组结构体声明 */
typedef struct rtos_flag_grp {
    
    /** \brief 事件控制块   */
    rtos_task_event_t flag_grp_event;
    
    /** \brief 当前事件标记 */
    uint32_t flag;
    
}rtos_flag_grp_t;

/** \brief 事件标记组结构体声明 */
typedef struct rtos_flag_grp_info_get {
    
    /** \brief 当前等待任务的计数   */
    uint32_t task_count;
    
    /** \brief 当前事件标记 */
    uint32_t flag;
    
    
}rtos_flag_grp_info_get_t;


/**
 * \brief 初始化事件标记组
 * \param[in] p_flag_grp: 指向事件标记组结构体的指针
 * \param[in] flag:  初始的事件标志
 * \return 无
 */  
void rtos_flag_grp_init(rtos_flag_grp_t *p_flag_grp, uint32_t flag);



/**
 * \brief 等待事件标记组
 * \param[in] p_flag_grp: 指向事件标记组结构体的指针
 * \param[in] wait_type:  等待的事件类型，值为 RTOS_FLAG_GRP_ANY_* 这一类宏 (# RTOS_FLAG_GRP_ANY_CLR)
 * \param[in] req_flag:   等待的事件标志组的标记
 * \param[out] p_req_result:  等待标记的结果,任务等待到哪些事件标记位
 * \param[in] wait_ticks:  当事件标记组计数为0时，等待的ticks数，为0时表示永远等待
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */  
int32_t rtos_flag_grp_wait (rtos_flag_grp_t *p_flag_grp, 
                            uint32_t wait_type, 
                            uint32_t req_flag, 
                            uint32_t *p_req_result, 
                            uint32_t wait_ticks);

/**
 * \brief 获取事件标记组，如果事件标记组计数不可用，则立即退回
 * \param[in] p_flag_grp: 指向事件标记组结构体的指针
 * \param[in] wait_type:  等待的事件类型，值为 RTOS_FLAG_GRP_ANY_* 这一类宏 (# RTOS_FLAG_GRP_ANY_CLR)
 * \param[in] req_flag:   等待的事件标志组的标记
 * \param[in] p_req_result:  等待标记的结果，任务等待到哪些事件标记位
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK
 */  
int32_t rtos_flag_grp_get (rtos_flag_grp_t *p_flag_grp, uint32_t wait_type, uint32_t req_flag, uint32_t *p_req_result);


/**
 * \brief 设置事件标记组中的某一位，唤醒等待队列中的一个任务
 * \param[in] p_flag_grp: 指向事件标记组结构体的指针
 * \param[in] set_bit:  等待的事件类型，值为 RTOS_FLAG_GRP_ANY_* 这一类宏 (# RTOS_FLAG_GRP_ANY_CLR)
 * \param[in] flag:     设置事件标志组的标记
 * \return  无
 */  
void rtos_flag_grp_notify (rtos_flag_grp_t *p_flag_grp, uint32_t set_bit, uint32_t flag);



/**
 * \brief  查询事件标志组的状态信息
 * \param[in] p_flag_grp: 指向事件标记组结构体的指针
 * \param[in] p_info:  指向事件标记组状态结构体的指针
 * \param[in] flag:     
 * \return  无
 */  
void rtos_flag_grp_info_get (rtos_flag_grp_t *p_flag_grp, rtos_flag_grp_info_get_t *p_info);
 
 
/**
 * \brief  销毁事件标志组
 * \param[in] p_flag_grp: 指向事件标记组结构体的指针
 * \return  因销毁该事件标记组控制块而唤醒的任务数量
 */  
uint32_t rtos_flag_grp_destroy (rtos_flag_grp_t *p_flag_grp);

    
    
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_FLAGGROUP_H */

