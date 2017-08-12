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
 * \brief ����ϵͳ����ʵ�� 
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
 * \brief ��ṹ���Ա��ƫ��
 * \attention ��ͬƽ̨�ϣ����ڳ�Ա��С���ڴ�����ԭ��
 *            ͬһ�ṹ���Ա��ƫ�ƿ����ǲ�һ����
 *
 * \par ʾ��
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

/** \brief ��ṹ���Ա��ƫ�ƣ�ͬ \ref RTOS_OFFSET һ�� */
#ifndef offsetof
#define offsetof(type, member)           RTOS_OFFSET(type, member)
#endif

/**
 * \brief ͨ���ṹ���Աָ���ȡ�����ýṹ���Ա�Ľṹ��������׵�ַ
 *
 * \param ptr    ָ��ṹ���Ա��ָ��
 * \param type   �ṹ������
 * \param member �ṹ���иó�Ա������
 *
 * \par ʾ��
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
 * \brief ͨ���ṹ���Աָ���ȡ�����ýṹ���Ա�Ľṹ�壬
 *        ͬ \ref RTOS_CONTAINER_OF һ��
 */
#ifndef container_of
#define container_of(ptr, type, member)     RTOS_CONTAINER_OF(ptr, type, member)
#endif    
    
    
/** \brief ��������ʵ��, ���������Զ�һ��ָ������ͷ����ָ�� */   
typedef struct __slist_node {
    
    /** \brief ��һ����� */
    struct __slist_node *p_next;
    
}slist_node_t;

/** \brief ��������ͷ������� */
typedef slist_node_t slist_head_t;

/** \brief ������������ص�����  */
typedef int (*slist_node_process_t) (void *p_arg, slist_node_t *p_node);


/**
 * \brief ���������ʼ������
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return  RTOS_OK :��ʼ�˳ɹ���RTOS_EINVAILD����ʼ��ʧ��
 */
int slist_init (slist_head_t *p_head);

/**
 * \brief ��ȡĳһ����ǰһ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_pos:  ָ��ָ������ָ��
 * \return ����ָ������ǰһ����ָ��
 */ 
slist_node_t *slist_prev_get(slist_head_t *p_head, slist_node_t *p_pos);


/**
 * \brief ��ȡĳһ���ĺ�һ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_pos:  ָ��ָ������ָ��
 * \return ����ָ�����ĺ�һ����ָ��
 */ 
slist_node_t *slist_next_get(slist_head_t *p_head, slist_node_t *p_pos);


/**
 * \brief ��ȡ��ʼλ�ã���һ���û����
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return ���ص��������һ���û�����ָ��
 */ 
slist_node_t *slist_begin_get(slist_head_t *p_head);


/**
 * \brief ��ȡβ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return ���ص�������β����ָ��
 */ 
slist_node_t *slist_tail_get (slist_head_t *p_head);


/**
 * \brief ��ȡ����λ�ã�β�����һ����λ��
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return ��ָ��
 */ 
slist_node_t *slist_end_get (slist_head_t *p_head);

/**
 * \brief �ڵ������������ӵ�ָ�����ĺ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_pos:  ָ��ָ������ָ��
 * \param[in] p_node: ָ�����ӽ���ָ��
 * \return  RTOS_OK :��ӳɹ���RTOS_EINVAILD�����ʧ��
 */ 
int slist_add(slist_head_t *p_head, slist_node_t *p_pos, slist_node_t *p_node);

/**
 * \brief �ڵ�������β�����һ�����
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_node: ָ�����ӽ���ָ��
 * \return  RTOS_OK :��ӳɹ���RTOS_EINVAILD�����ʧ��
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node);
;

/**
 * \brief �ڵ�������ͷ�����һ�����
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_node: ָ�����ӽ���ָ��
 * \return  RTOS_OK :��ӳɹ���RTOS_EINVAILD�����ʧ��
 */ 
int slist_add_head(slist_head_t *p_head, slist_node_t *p_node);


/**
 * \brief ��������
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] pfn_node_process: �����Ļص�����
 * \param[in] p_arg: �����Ļص������Ĳ���
 * \return  RTOS_OK :����ȡ������Ҫ����Ϣ; -RTOS_ERROR: û�б���������Ҫ��Ϣ
 */  
int slist_foreach (slist_head_t *p_head, slist_node_process_t pfn_node_process, void *p_arg);

/**
 * \brief ɾ����� 
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_node: ָ���ɾ������ָ��
 * \return  RTOS_OK :ɾ���ɹ���RTOS_EINVAILD��ɾ��ʧ��
 */  
int slist_del (slist_head_t *p_head, slist_node_t *p_node);



/*****************************************************************************/
/** \brief ˫������ʵ�� */    
typedef struct __dlist_node {
 
    /** \brief ��һ����� */
    struct __dlist_node *p_next;
    
    /** \brief ǰһ����� */
    struct __dlist_node *p_prev; 
 
}dlist_node_t; 


/** \brief ˫������ͷ������� */
typedef dlist_node_t dlist_head_t;

/** \brief ˫����������ص�����  */
typedef int (*dlist_node_process_t) (void *p_arg, dlist_node_t *p_node);


/**
 * \brief ˫�������ʼ������
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return  RTOS_OK :��ʼ�˳ɹ���RTOS_EINVAILD����ʼ��ʧ��
 */
int dlist_init (dlist_head_t *p_head);

/**
 * \brief ��ȡĳһ����ǰһ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_pos:  ָ��ָ������ָ��
 * \return ����ָ������ǰһ����ָ��
 */ 
dlist_node_t *dlist_prev_get(dlist_head_t *p_head, dlist_node_t *p_pos);


/**
 * \brief ��ȡĳһ���ĺ�һ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_pos:  ָ��ָ������ָ��
 * \return ����ָ�����ĺ�һ����ָ��
 */ 
dlist_node_t *dlist_next_get(dlist_head_t *p_head, dlist_node_t *p_pos);


/**
 * \brief ��ȡ��ʼλ�ã���һ���û����
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return ���ص��������һ���û�����ָ��
 */ 
dlist_node_t *dlist_begin_get(dlist_head_t *p_head);


/**
 * \brief ��ȡβ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return ���ص�������β����ָ��
 */ 
dlist_node_t *dlist_tail_get (dlist_head_t *p_head);


/**
 * \brief ��ȡ����λ�ã�β�����һ����λ��
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return ��ָ��
 */ 
dlist_node_t *dlist_end_get (dlist_head_t *p_head);

/**
 * \brief ��˫�����������ӵ�ָ�����ĺ���
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_pos:  ָ��ָ������ָ��
 * \param[in] p_node: ָ�����ӽ���ָ��
 * \return  RTOS_OK :��ӳɹ���RTOS_EINVAILD�����ʧ��
 */ 
int dlist_add(dlist_head_t *p_head, dlist_node_t *p_pos, dlist_node_t *p_node);

/**
 * \brief ��˫������β�����һ�����
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_node: ָ�����ӽ���ָ��
 * \return  RTOS_OK :��ӳɹ���RTOS_EINVAILD�����ʧ��
 */ 
int dlist_add_tail(dlist_head_t *p_head, dlist_node_t *p_node);
;

/**
 * \brief ��˫������ͷ�����һ�����
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_node: ָ�����ӽ���ָ��
 * \return  RTOS_OK :��ӳɹ���RTOS_EINVAILD�����ʧ��
 */ 
int dlist_add_head(dlist_head_t *p_head, dlist_node_t *p_node);


/**
 * \brief ��������
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] pfn_node_process: �����Ļص�����
 * \param[in] p_arg: �����Ļص������Ĳ���
 * \return  RTOS_OK :����ȡ������Ҫ����Ϣ; -RTOS_ERROR: û�б���������Ҫ��Ϣ
 */  
int dlist_foreach (dlist_head_t *p_head, dlist_node_process_t pfn_node_process, void *p_arg);

/**
 * \brief ɾ����� 
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \param[in] p_node: ָ���ɾ������ָ��
 * \return  RTOS_OK :ɾ���ɹ���RTOS_EINVAILD��ɾ��ʧ��
 */  
int dlist_del (dlist_head_t *p_head, dlist_node_t *p_node);


/**
 * \brief ��������
 * \param[in] p_head: ָ��������ͷ����ָ��
 * \return  RTOS_OK :���ٳɹ���RTOS_EINVAILD������ʧ��
 */  
int dlist_destroy (dlist_head_t *p_head);

    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_LIST_H */





