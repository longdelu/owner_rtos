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

/** \brief ��������ɾ����� */   
#define RTOS_TASK_STATE_RED_DEL    (1UL << 3)   
    
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
    
    /** \brief ����ɾ��ʱ���õ������� */
    void (*pfn_clean) (void * p_par);

    /** \brief ���ݸ��������Ĳ��� */
    void * clean_param;

    /** \brief ����ɾ����־����0��ʾ����ɾ��*/
    uint8_t req_delete_flag;    
           
}rtos_task_t;


typedef struct rtos_task_info {
    /** \brief ����ĵδ����   */     
    uint32_t delay_ticks;
    
    /** \brief ��������ȼ�   */     
    uint32_t task_prio;
    
    /** \brief �����״̬   */   
    uint32_t task_state;
    
    /** \brief �����ʱ��Ƭ����   */ 
    uint32_t task_slice;
    
    /** \brief ���񱻹���Ĵ���   */    
    uint32_t suspend_cnt;
    
   
}rtos_task_info_t;


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
 * \brief ����ʱ���������ʱ�����л���
 * \param[in] p_task: ����ṹ��ָ��   
 * \return ��  
 */
void rtos_task_wake_up_delayed_list (rtos_task_t *p_task);

/**
 * \brief ����ʱ���������ʱ������ɾ��
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


/**
 * \brief ��������ɾ��ʱ���õ�������
 * \param[in] p_task: ����ṹ��ָ�� 
 * \param[in] pfn_clean: ָ�������������ĺ���ָ��   
 * \param[in] p_par: ���ݸ��������Ĳ���   
 * \return �� 
 */
void rtos_task_set_clean_call_fuc (rtos_task_t *p_task, void (*pfn_clean)(void * p_par), void * p_par);


/**
 * \brief ǿ��ɾ��ָ��������
 * \param[in] p_task: ����ṹ��ָ��   
 * \return �� 
 */
void rtos_task_force_del (rtos_task_t *p_task);


/**
 * \brief ����ɾ��ĳ�������������Լ������Ƿ�ɾ���Լ�
 * \param[in] p_task: ����ṹ��ָ��   
 * \return �� 
 */
void rtos_task_req_del (rtos_task_t *p_task);


/**
 * \brief ��ѯ�Ƿ��Ѿ�������ɾ������
 * \param[in] ��   
 * \return �� 
 */
uint32_t rtos_task_req_del_flag_check (void);


/**
 * \brief ����ɾ������
 * \param[in] ��
 * \return �� 
 */
void rtos_task_del_self (void);


/**
 * \brief ��ȡ���������Ϣ
 * \param[in] p_task: ����ṹ��ָ��
 * \param[in] p_task_info: ������Ϣ�ṹ��ָ��    
 * \return �� 
 */
void rtos_task_info_get (rtos_task_t *p_task, rtos_task_info_t *p_task_info);


#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_H */

/* end of file */

