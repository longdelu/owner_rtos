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
 * \brief ��������
 */    
#define TASK_COUNT    RTOS_PRIO_COUNT  
    
/** \brief �����ھ���״̬ */   
#define RTOS_TASK_STATE_REDDY      (1UL << 0)  
    
/** \brief ��������ʱ״̬ */   
#define RTOS_TASK_STATE_DELAYED    (1UL << 1)     
    
/** \brief �����ڹ���״̬ */   
#define RTOS_TASK_STATE_SUSPEND    (1UL << 2)   
    
/* Cortex-M�Ķ�ջ��Ԫ���ͣ���ջ��Ԫ�Ĵ�СΪ32λ������ʹ��uint32_t */
typedef uint32_t taskstack_t;
    
typedef struct rtos_task {
   
    /** \brief ��ջջ����ַ   */    
    uint32_t *task_stack_top;
    
    /** \brief ��ջ���𼴵�ַ */
    uint32_t * stack_base;

    /** \brief ��ջ��������   */
    uint32_t stack_size;
    
    /** \brief ������ʱ������ */
    uint32_t delay_ticks;

    /** \brief ��������ȼ�   */    
    uint32_t prio;
    
    /** \brief �������ʱ��㣬ͨ���ý�㽫������õ���ʱ������   */     
    dlist_node_t delay_node;

    /** \brief ͬһ���ȼ���������ͨ���ý�㽫������õ�ͬһ���ȼ����������   */     
    dlist_node_t prio_node;
       
    /** \brief �����״̬   */      
    uint32_t task_state;
    
    /** \brief �����ʱ��Ƭ����   */      
    uint32_t slice;   

    /** \brief ���񱻹���Ĵ���   */      
    uint32_t suspend_cnt;    
        
}rtos_task_t;


/** \brief ��ǰ���񣺼�¼��ǰ���ĸ������������� */
extern rtos_task_t * p_current_task;

/** \brief ��һ���������е������ڽ��������л�ǰ�������úø�ֵ��Ȼ�������л������л���ж�ȡ��һ������Ϣ */
extern rtos_task_t * p_next_task;


/** \brief ���������ָ�����飺�������ֻʹ���������� */
extern rtos_task_t * p_task_table[TASK_COUNT];

/** \brief  ��������ṹ��ָ�� */
extern rtos_task_t * p_idle_task;

/** \brief �������ȼ��ı��λ�ýṹȫ���� */
extern rtos_task_bitmap_t task_priobitmap;


/**
 * \brief  �����ʼ������
 *
 * \param[in] task: ����ṹ��ָ��
 * \param[in] task_entry: ������ں���
 * \param[in] p_arg: ������ں�������
 * \param[in] task_prio: ��������ȼ�
 * \param[in] task_stack: �����ջ��ָ��
 * \param[in] task_stack_size: �����ջ�Ĵ�С,���ֽ�Ϊ��λ
 *
 * \return    ��
 */
void rtos_task_init(rtos_task_t * task, 
                    void (*task_entry) (void *p_arg), 
                    void *p_arg, 
                    uint32_t task_prio, 
                    uint32_t *task_stack,
                    uint32_t task_stack_size);
                    
                    
/**
 * \brief ��ʼ�����ȼ������������  
 * 
 * \param[in] p_task_table: ���ȼ�������ȱ�
 *                    
 * \return ��                  
 */
void rtos_task_sched_init(rtos_task_list_t *p_task_table);                    
                    
                    
/**
 * \brief ����������Ϊ����״̬ 
 * 
 * \param[in] task: ����ṹ��ָ��   
 * \return ��                    
 */
void rtos_task_sched_ready(rtos_task_t *task);


/**
 * \brief ����������б����Ƴ�
 * \param[in] task: ����ṹ��ָ��   
 * \return ��                       
 */
void rtos_task_sched_unready(rtos_task_t *task);                   
                    
                                       
/**
 * \brief ��ȡ��ǰ������ȼ��ҿ����е�����
 *                    
 * \param[in] ��
 *
 * \return    ���ȼ���ߵ��ҿ����е�����ṹ��ָ��                                      
 */
rtos_task_t *rtos_task_highest_ready(void);                    
                    
                    
/**
 * \brief �л�����һ���������У���������ϵͳ��һ������
 *                    
 * \param[in] ��
 *
 * \return    ��                                      
 */
void rtos_task_run_first (void);                    
                    
                    
/**
 * \brief ������Ⱥ��� 
 *                    
 * \param[in] ��
 *
 * \return    ��
 */
void rtos_task_sched(void);           


/**
 * \brief ��ʼ��������ʱ����
 * \param[in] p_rtos_task_delayed_list : ָ��������ʱ���е�ָ��
 * \return ��
 */
void rtos_task_delayed_init (rtos_task_list_t *p_rtos_task_delayed_list);


/**
 * \brief ��������뵽��ʱ������
 * \param[in] p_task: ����ṹ��ָ��   
 * \param[in] delay_ticks: ��ʱ��ticks
 * \return ��  
 */
void rtos_task_add_delayed_list (rtos_task_t * p_task, uint32_t delay_ticks);


/**
 * \brief ���������ʱ�����л���
 * \param[in] p_task: ����ṹ��ָ��   
 * \return ��  
 */
void rtos_task_del_delayed_list (rtos_task_t *p_task);


/**
 * \brief ��������
 * \param[in] p_task: ����ṹ��ָ��   
 * \return �� 
 */
void rtos_task_suspend (rtos_task_t *p_task);


/**
 * \brief ��������
 * \param[in] p_task: ����ṹ��ָ��   
 * \return �� 
 */
void rtos_task_wakeup (rtos_task_t *p_task);


#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_H */

/* end of file */

