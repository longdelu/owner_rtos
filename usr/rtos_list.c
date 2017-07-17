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
 
#include "rtos_list.h"
#include "rtos_config.h"

/*****************************************************************************/
 
/** \brief ��������ʵ��,����ͷ���ʵ�� */


/**
 * \brief ���������ʼ������
 */
int slist_init (slist_head_t *p_head)
{
    if (p_head == NULL) {
        
        return -RTOS_EINVAILD;

    }
    
    p_head->p_next = NULL;

    return RTOS_OK;      
}

/**
 * \brief ��ȡĳһ����ǰһ���
 */ 
slist_node_t *slist_prev_get(slist_head_t *p_head, slist_node_t *p_pos)
{
    slist_node_t *p_tmp = p_head; 
    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_pos == NULL) {
        
        return NULL;
        
    } 

    while ((p_tmp != NULL) && (p_tmp->p_next != p_pos)) {
        p_tmp = p_tmp->p_next;

    }

    return p_tmp;    
}


/**
 * \brief ��ȡĳһ���ĺ�һ���
 */ 
slist_node_t *slist_next_get(slist_head_t *p_head, slist_node_t *p_pos)
{
    if (p_pos) {
        return p_pos->p_next; 
    }
    
    return NULL;  
}


/**
 * \brief ��ȡ��ʼλ�ã���һ���û����
 */ 
slist_node_t *slist_begin_get(slist_head_t *p_head)
{
    
    return  slist_next_get(p_head, p_head); 
}


/**
 * \brief ��ȡβ���
 */ 
slist_node_t *slist_tail_get (slist_head_t *p_head)
{
    
    /* ����β��� */
    return  slist_prev_get(p_head, NULL);  
}


/**
 * \brief ��ȡ����λ�ã�β�����һ����λ��
 */ 
slist_node_t *slist_end_get (slist_head_t *p_head)
{
    
    return  NULL;  
}

/**
 * \brief �ڵ������������ӵ�ָ�����ĺ���
 */ 
int slist_add(slist_head_t *p_head, slist_node_t *p_pos, slist_node_t *p_node)
{
    /* ���û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL || p_pos == NULL) {
        
        return -RTOS_EINVAILD;
    } 

    /* �½���е�p_nextָ��p_posָ�������һ����� */
    p_node->p_next = p_pos->p_next;

    /* �޸�p_pos->p_next��ָ��ʹ��ָ���½�� */
    p_pos->p_next = p_node; 
    
    
    return RTOS_OK;    
}

#if 0

/**
 * \brief �ڵ�������β�����һ�����
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tmp = p_head;
    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* Ѱ�ҵ����һ�������� */
    while (p_tmp->p_next != NULL) {
        
        p_tmp = p_tmp->p_next;
    }
   
     /* �������ӵ�����β�� */ 
    return slist_add(p_head, p_tmp, p_node);            
}

/**
 * \brief �ڵ�������β�����һ�����
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tmp = NULL
    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* Ѱ�ҵ����һ�������� */   
    p_tmp = slist_tail_get(p_head);
  
     /* �������ӵ�����β�� */ 
    return slist_add(p_head, p_tmp, p_node);            
}

/**
 * \brief �ڵ�������ͷ�����һ�����
 */ 
int slist_add_head(slist_head_t *p_head, slist_node_t *p_node)
{
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
    }
    
    
      /* �������ӵ�����ͷ�� */    
    return slist_add(p_head, p_head, p_node);  
              
}

#endif /* 0 */ 





#if 1

/**
 * \brief �ڵ�������β�����һ�����
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tmp = p_head;
    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    while (p_tmp->p_next != NULL) {
        
        p_tmp = p_tmp->p_next;
    }
    
    /* β����p_nextָ���½�� */
    p_tmp->p_next = p_node;
    
    /* �½���p_nextָ��NULL */
    p_node->p_next = NULL;
    
    return RTOS_OK;
            
}

/**
 * \brief �ڵ�������ͷ�����һ�����
 */ 
int slist_add_head(slist_head_t *p_head, slist_node_t *p_node)
{
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
    }
    
    /* �ӽ����Ľ��ָ���һ���û���� */
    p_node->p_next = p_head->p_next;
    
    
    /* ͷ����ָ��ĵ�һ���û�������µ�����ʹ������ָ���½�� */
    p_head->p_next = p_node; 
    
    
    return RTOS_OK;
               
}

#endif /* 1 */ 


/**
 * \brief ��������
 */  
int slist_foreach (slist_head_t *p_head, slist_node_process_t pfn_node_process, void *p_arg)
{
    slist_node_t *p_tmp = NULL; 
    
    slist_node_t *p_end = NULL; 

    int ret = 0;

    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || pfn_node_process == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    p_tmp  = slist_begin_get(p_head);
    
    p_end  = slist_end_get(p_head);
    
    
    while(p_tmp != p_end) {
        
        ret = pfn_node_process(p_arg, p_tmp);
        
        if (ret == 0)  {
            return ret; /* ���ٱ��� */
        }
        
        p_tmp = slist_next_get(p_head, p_tmp); 
        
    }
    
    
    return -RTOS_ERROR;
       
}

/**
 * \brief ɾ����� 
 */  
int slist_del (slist_head_t *p_head, slist_node_t *p_node) 
{
    slist_node_t *p_prev = NULL; 
    
    /* ͷ���ʵ��û�г�ʼ��,���Է�ֹ��ɾͷ��� */
    if (p_head == NULL || p_node == NULL || (p_node == p_head)) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* ��ô�ɾ������ǰһ��� */
    p_prev =  slist_prev_get(p_head, p_node);
    
    if (p_prev) {
        
        /* ʹ��ɾ��������һ������p_nextָ���ɾ��������һ�����*/
        p_prev->p_next = p_node->p_next;

        /* ��ɾ������p_next��Ϊ�� */
        p_node->p_next = NULL;
        
        return RTOS_OK;
        
    } else {
        return -RTOS_EINVAILD; 
    }    
}



                   


/*****************************************************************************/ 
/** \brief ˫�����ʵ�֣�����ͷ���ʵ�� */
 
/**
 * \brief ˫�������ʼ������
 */
int dlist_init (dlist_head_t *p_head)
{
    if (p_head == NULL) {
        
        return -RTOS_EINVAILD;

    }
    
    p_head->p_next = p_head;
    p_head->p_prev = p_head;

    return RTOS_OK;      
}

/**
 * \brief ��ȡĳһ����ǰһ���
 */ 
dlist_node_t *dlist_prev_get(dlist_head_t *p_head, dlist_node_t *p_pos)
{
    dlist_node_t *p_tmp = p_head; 
    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_pos == NULL) {
        
        return NULL;
        
    } 

    if (p_pos != NULL) {
        
        return p_pos->p_prev;
    }

    return p_tmp;    
}


/**
 * \brief ��ȡĳһ���ĺ�һ���
 */ 
dlist_node_t *dlist_next_get(dlist_head_t *p_head, dlist_node_t *p_pos)
{
    if (p_pos) {
        return p_pos->p_next; 
    }
    
    return NULL;  
}


/**
 * \brief ��ȡ��ʼλ�ã���һ���û����
 */ 
dlist_node_t *dlist_begin_get(dlist_head_t *p_head)
{
    
    if (p_head != NULL) {
        
        /* ���ص�һ���û���� */
        return p_head->p_next;
    }
    
    return NULL;
}


/**
 * \brief ��ȡβ���
 */ 
dlist_node_t *dlist_tail_get (dlist_head_t *p_head)
{
    
    if (p_head != NULL) {
        
        /* ����β��� */
        return p_head->p_prev;
    }
    
    return NULL;
        
    
}


/**
 * \brief ��ȡ����λ�ã�β�����һ����λ��
 */ 
dlist_node_t *dlist_end_get (dlist_head_t *p_head)
{
    
    if (p_head != NULL) {
        
        /* ����β�����һ����λ��,��ͷ��㱾�� */
        return p_head->p_prev;
    }
    
    return NULL; 
}

/**
 * \brief ��˫�����������ӵ�ָ�����ĺ���
 */ 
int dlist_add(dlist_head_t *p_head, dlist_node_t *p_pos, dlist_node_t *p_node)
{
    /* ���û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL || p_pos == NULL) {
        
        return -RTOS_EINVAILD;
    } 
    
    /* �½���е�p_prevָ��p_pos��һ��ǰ��� */
    p_node->p_prev = p_pos;

    /* �½���е�p_nextָ��p_pos��һ��������һ����� */
    p_node->p_next = p_pos->p_next;
    
    /* ����p_pos->p_next��p_prevָ���½�� */
    p_pos->p_next->p_prev = p_node; 

    /* p_pos��һ��ǰ����p_nextָ���½�� */
    p_pos->p_next = p_node; 
    
    
    return RTOS_OK;    
}



/**
 * \brief ��˫������β�����һ�����
 */ 
int dlist_add_tail(dlist_head_t *p_head, dlist_node_t *p_node)
{
    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
     /* �������ӵ�����β�� */ 
    return dlist_add(p_head, p_head->p_prev, p_node);            
}


/**
 * \brief ��˫������ͷ�����һ�����
 */ 
int dlist_add_head(dlist_head_t *p_head, dlist_node_t *p_node)
{
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
    }
    
    
      /* �������ӵ�����ͷ�� */    
    return dlist_add(p_head, p_head, p_node);  
              
}


/**
 * \brief ��������
 */  
int dlist_foreach (dlist_head_t *p_head, dlist_node_process_t pfn_node_process, void *p_arg)
{
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 

    int ret = 0;

    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL || pfn_node_process == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    /* ��õ�һ���û���� */
    p_tmp  = dlist_begin_get(p_head);
    
    /* ����λ��Ϊͷ��㱾�� */
    p_end  = dlist_end_get(p_head);
    
    
    while(p_tmp != p_end) {
        
        ret = pfn_node_process(p_arg, p_tmp);
        
        if (ret == 0)  {
            return ret; /* ���ٱ��� */
        }
        
        p_tmp = dlist_next_get(p_head, p_tmp); 
        
    }
    
    
    return -RTOS_ERROR;
       
}

/**
 * \brief ɾ����� 
 */  
int dlist_del (dlist_head_t *p_head, dlist_node_t *p_node) 
{
    
    /* ͷ���ʵ��û�г�ʼ������ֹ��ɾ��ͷ��� */
    if (p_head == NULL || p_node == NULL || (p_head == p_node)) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* ǰ���p_node->p_prev��p_next�޸�ָ����� */
    p_node->p_prev->p_next =  p_node->p_next;
    
    /* ����p_node->p_next��p_prevt�޸�ָ��ǰ��� */
    p_node->p_next->p_prev =  p_node->p_prev;   

    /* ʹ���ָ������ */
    p_node->p_next = p_node;
    p_node->p_prev = p_node;

    return RTOS_OK;    
}

/**
 * \brief ��������
 */  
int dlist_destroy (dlist_head_t *p_head)
{
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 

    
    /* ͷ���ʵ��û�г�ʼ�� */
    if (p_head == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    /* ��õ�һ���û���� */
    p_next  = dlist_begin_get(p_head);
    
    /* ����λ��Ϊͷ��㱾�� */
    p_end  = dlist_end_get(p_head);
    
  
    while(p_next != p_end) {   

        /* ����ǰ�ȼ�¼�µ�ǰ��� */
        p_tmp =  p_next;
    
        
        /* ����ǰ�ȼ�¼��һ��������Ϣ */
        p_next = dlist_next_get(p_head, p_next);

        /* ���ý���Լ�����Ϣ,ָ�������� */
        p_tmp->p_next = p_tmp;            
        p_tmp->p_prev = p_tmp;   
        
              
    }
    
    /*  ����ͷ����Լ�����Ϣ,ָ��ͷ������� */
    p_next->p_next =  p_next;
    p_next->p_next =  p_next;
    
    return RTOS_OK;
       
}

 
/* end of file */


