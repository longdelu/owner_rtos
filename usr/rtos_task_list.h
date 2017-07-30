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
 * \brief 操作系统任务链表实现 
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#ifndef __RTOS_TASK_LIST_H
#define __RTOS_TASK_LIST_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_list.h"
    
/** \brief 单向任务链表 */   



/** \brief 双向任务键表实现 */    
typedef struct rtos_task_list {
 
    /** \brief 双向任务链表头结点  */ 
    dlist_node_t head_node ;
    
    /** \brief 任务数量 */
    uint32_t count;
    
    
}rtos_task_list_t;   


/**
 * \brief RTOS任务链表初始化 
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \return 无
 */  
void rtos_task_list_init (rtos_task_list_t *p_rtos_task_list);

/**
 * \brief 统计链表中结点的数量
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \return 链表中所有结点的数量 
 */
uint32_t rtos_task_list_count (rtos_task_list_t *p_rtos_task_list);


/**
 * \brief 返回任务链表的首个结点 
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \return 任务链表的第一个用户结点
 */
dlist_node_t *rtos_task_list_begin_get(rtos_task_list_t *p_rtos_task_list);

/**
 * \brief 近回任务链表的最后一个结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \return 任务链表的最后一个用户结 
 */
dlist_node_t *rtos_task_list_tail_get(rtos_task_list_t *p_rtos_task_list);

/**
 * \brief 近回任务链表指定结点的上一个结点 
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \param[in] p_node: 指向结点的指针
 * \reutn 近回任务链表指定结点的上一个结点的指针 
 */
dlist_node_t *rtos_task_list_prev_get(rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node);

/**
 * \brief 近回任务链表指定结点的下一个结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \param[in] p_node: 指向结点的指针
 * \reutn 近回任务链表指定结点的下一个结点的指针  
 */
dlist_node_t *rtos_task_list_next_get(rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node);

/**
 * \brief 移除任务链表中所有的结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \return 无
 */
void rtos_task_list_remove_all (rtos_task_list_t *p_rtos_task_list);


/**
 * \brief 在任务链表头部插入结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \param[in] p_node: 指向结点的指针
 * \reutn 无  
 */
void rtos_task_list_add_head (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node);

/**
 * \brief 在任务链表尾部插入结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \param[in] p_node: 指向结点的指针
 * \reutn 无 
 */
void rtos_task_list_add_tail (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node);

/**
 * \brief 在任务链表指定位置后面插入结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \param[in] p_node: 指向结点的指针
 * \reutn 无 
 */
void rtos_task_list_add (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_pos, dlist_node_t *p_node);

/**
 * \brief 在任务链表中删除第1个用户结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \reutn 返回被删除的结点 
 */
dlist_node_t *rtos_task_list_remove_first (rtos_task_list_t * p_rtos_task_list);

/**
 * \brief 在任务链表中删除最后1个用户结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指针
 * \reutn 返回被删除的结点 
 */
dlist_node_t *rtos_task_list_remove_last (rtos_task_list_t * p_rtos_task_list);

/**
 * \brief 在任务链表删除指定结点
 * \param[in] p_rtos_task_list: 指向链表结构体的指
 * \param[in] p_node: 指向结点的指针
 * \reutn 无 
 */
void rtos_task_list_remove (rtos_task_list_t * p_rtos_task_list, dlist_node_t *p_node);
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_LIST_H */ 



