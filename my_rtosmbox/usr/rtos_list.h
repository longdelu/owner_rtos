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
 * \brief 操作系统链表实现 
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#ifndef __RTOS_LIST_H
#define __RTOS_LIST_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
    
/**
 * \brief 求结构体成员的偏移
 * \attention 不同平台上，由于成员大小和内存对齐等原因，
 *            同一结构体成员的偏移可能是不一样的
 *
 * \par 示例
 * \code
 *  struct my_struct = {
 *      int  m1;
 *      char m2;
 *  };
 *  int offset_m2;
 *
 *  offset_m2 = RTOS_OFFSET(struct my_struct, m2);
 * \endcode
 */
#define RTOS_OFFSET(structure, member)    ((size_t)(&(((structure *)0)->member)))

/** \brief 求结构体成员的偏移，同 \ref RTOS_OFFSET 一样 */
#ifndef offsetof
#define offsetof(type, member)           RTOS_OFFSET(type, member)
#endif

/**
 * \brief 通过结构体成员指针获取包含该结构体成员的结构体变量的首地址
 *
 * \param ptr    指向结构体成员的指针
 * \param type   结构体类型
 * \param member 结构体中该成员的名称
 *
 * \par 示例
 * \code
 *  struct my_struct = {
 *      int  m1;
 *      char m2;
 *  };
 *  struct my_struct  my_st;
 *  char             *p_m2 = &my_st.m2;
 *  struct my_struct *p_st = RTOS_CONTAINER_OF(p_m2, struct my_struct, m2);
 * \endcode
 */
#define RTOS_CONTAINER_OF(ptr, type, member) \
            ((type *)((char *)(ptr) - offsetof(type,member)))

/**
 * \brief 通过结构体成员指针获取包含该结构体成员的结构体，
 *        同 \ref RTOS_CONTAINER_OF 一样
 */
#ifndef container_of
#define container_of(ptr, type, member)     RTOS_CONTAINER_OF(ptr, type, member)
#endif    
    
    
/** \brief 单向链表实现, 方法，可以定一个指向链表头结点的指针 */   
typedef struct __slist_node {
    
    /** \brief 下一个结点 */
    struct __slist_node *p_next;
    
}slist_node_t;

/** \brief 单项链表头结点类型 */
typedef slist_node_t slist_head_t;

/** \brief 单项链表遍历回调函数  */
typedef int (*slist_node_process_t) (void *p_arg, slist_node_t *p_node);


/**
 * \brief 单项链表初始化函数
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return  RTOS_OK :初始人成功；RTOS_EINVAILD：初始化失败
 */
int slist_init (slist_head_t *p_head);

/**
 * \brief 获取某一结点的前一结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_pos:  指向指定结点的指针
 * \return 返回指定结点的前一结点的指针
 */ 
slist_node_t *slist_prev_get(slist_head_t *p_head, slist_node_t *p_pos);


/**
 * \brief 获取某一结点的后一结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_pos:  指向指定结点的指针
 * \return 返回指定结点的后一结点的指针
 */ 
slist_node_t *slist_next_get(slist_head_t *p_head, slist_node_t *p_pos);


/**
 * \brief 获取开始位置，第一个用户结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return 返回单项链表第一个用户结点的指针
 */ 
slist_node_t *slist_begin_get(slist_head_t *p_head);


/**
 * \brief 获取尾结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return 返回单项链表尾结点的指针
 */ 
slist_node_t *slist_tail_get (slist_head_t *p_head);


/**
 * \brief 获取结束位置，尾结点下一结点的位置
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return 空指针
 */ 
slist_node_t *slist_end_get (slist_head_t *p_head);

/**
 * \brief 在单项链表将结点添加到指定结点的后面
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_pos:  指向指定结点的指针
 * \param[in] p_node: 指向待添加结点的指针
 * \return  RTOS_OK :添加成功；RTOS_EINVAILD：添加失败
 */ 
int slist_add(slist_head_t *p_head, slist_node_t *p_pos, slist_node_t *p_node);

/**
 * \brief 在单项链表尾部添加一个结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_node: 指向待添加结点的指针
 * \return  RTOS_OK :添加成功；RTOS_EINVAILD：添加失败
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node);
;

/**
 * \brief 在单项链表头部添加一个结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_node: 指向待添加结点的指针
 * \return  RTOS_OK :添加成功；RTOS_EINVAILD：添加失败
 */ 
int slist_add_head(slist_head_t *p_head, slist_node_t *p_node);


/**
 * \brief 遍历链表
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] pfn_node_process: 遍历的回调函数
 * \param[in] p_arg: 遍历的回调函数的参数
 * \return  RTOS_OK :遍历取得所需要的信息; -RTOS_ERROR: 没有遍历到所需要信息
 */  
int slist_foreach (slist_head_t *p_head, slist_node_process_t pfn_node_process, void *p_arg);

/**
 * \brief 删除结点 
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_node: 指向待删除结点的指针
 * \return  RTOS_OK :删除成功；RTOS_EINVAILD：删除失败
 */  
int slist_del (slist_head_t *p_head, slist_node_t *p_node);



/*****************************************************************************/
/** \brief 双向链表实现 */    
typedef struct __dlist_node {
 
    /** \brief 下一个结点 */
    struct __dlist_node *p_next;
    
    /** \brief 前一个结点 */
    struct __dlist_node *p_prev; 
 
}dlist_node_t; 


/** \brief 双项链表头结点类型 */
typedef dlist_node_t dlist_head_t;

/** \brief 双项链表遍历回调函数  */
typedef int (*dlist_node_process_t) (void *p_arg, dlist_node_t *p_node);


/**
 * \brief 双项链表初始化函数
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return  RTOS_OK :初始人成功；RTOS_EINVAILD：初始化失败
 */
int dlist_init (dlist_head_t *p_head);

/**
 * \brief 获取某一结点的前一结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_pos:  指向指定结点的指针
 * \return 返回指定结点的前一结点的指针
 */ 
dlist_node_t *dlist_prev_get(dlist_head_t *p_head, dlist_node_t *p_pos);


/**
 * \brief 获取某一结点的后一结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_pos:  指向指定结点的指针
 * \return 返回指定结点的后一结点的指针
 */ 
dlist_node_t *dlist_next_get(dlist_head_t *p_head, dlist_node_t *p_pos);


/**
 * \brief 获取开始位置，第一个用户结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return 返回单项链表第一个用户结点的指针
 */ 
dlist_node_t *dlist_begin_get(dlist_head_t *p_head);


/**
 * \brief 获取尾结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return 返回单项链表尾结点的指针
 */ 
dlist_node_t *dlist_tail_get (dlist_head_t *p_head);


/**
 * \brief 获取结束位置，尾结点下一结点的位置
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return 空指针
 */ 
dlist_node_t *dlist_end_get (dlist_head_t *p_head);

/**
 * \brief 在双项链表将结点添加到指定结点的后面
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_pos:  指向指定结点的指针
 * \param[in] p_node: 指向待添加结点的指针
 * \return  RTOS_OK :添加成功；RTOS_EINVAILD：添加失败
 */ 
int dlist_add(dlist_head_t *p_head, dlist_node_t *p_pos, dlist_node_t *p_node);

/**
 * \brief 在双项链表尾部添加一个结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_node: 指向待添加结点的指针
 * \return  RTOS_OK :添加成功；RTOS_EINVAILD：添加失败
 */ 
int dlist_add_tail(dlist_head_t *p_head, dlist_node_t *p_node);
;

/**
 * \brief 在双项链表头部添加一个结点
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_node: 指向待添加结点的指针
 * \return  RTOS_OK :添加成功；RTOS_EINVAILD：添加失败
 */ 
int dlist_add_head(dlist_head_t *p_head, dlist_node_t *p_node);


/**
 * \brief 遍历链表
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] pfn_node_process: 遍历的回调函数
 * \param[in] p_arg: 遍历的回调函数的参数
 * \return  RTOS_OK :遍历取得所需要的信息; -RTOS_ERROR: 没有遍历到所需要信息
 */  
int dlist_foreach (dlist_head_t *p_head, dlist_node_process_t pfn_node_process, void *p_arg);

/**
 * \brief 删除结点 
 * \param[in] p_head: 指向单项链表头结点的指针
 * \param[in] p_node: 指向待删除结点的指针
 * \return  RTOS_OK :删除成功；RTOS_EINVAILD：删除失败
 */  
int dlist_del (dlist_head_t *p_head, dlist_node_t *p_node);


/**
 * \brief 销毁链表
 * \param[in] p_head: 指向单项链表头结点的指针
 * \return  RTOS_OK :销毁成功；RTOS_EINVAILD：销毁失败
 */  
int dlist_destroy (dlist_head_t *p_head);

    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_LIST_H */





