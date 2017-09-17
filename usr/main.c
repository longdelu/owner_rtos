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
 * \brief main�������
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "rtos_init.h"
#include "microlib_adapter.h"
#include "sdram.h"
#include "lan8720.h"
#include "pcf8574.h"
#include "usmart.h"
#include "malloc.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"

#define   TASK_STACK_SIZE  1024  

/** \brief ����1��� */
int g_task_flag1 = 0;

/** \brief ����2��� */
int g_task_flag2 = 0;

/** \brief �����ջ */

 /* ��8�ֽڶ��� */
__align(8) taskstack_t first_task_stack_buf[TASK_STACK_SIZE];
__align(8) taskstack_t second_task_stack_buf[TASK_STACK_SIZE];


/** \brief ��ǰ����ṹ�� */
rtos_task_t first_task;

/** \brief ��һ������ṹ�� */
rtos_task_t second_task;

/**
 * \brief ����״̬��Ϣ
 */

/**
 * \brief ��ǰ������ں���,�൱��starts����
 * \note  ϵͳ���ĳ�ʼ������Ҫ�������ʼ��������ɺ���ܵ���
 */
void first_task_entry (void *p_arg)
{  
/* CPU������ռ���ʵ�ʱ�� */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0    
    /* ȷ�����񱻵����������ٳ�ʼ��ϵͳ��������Ϊ10ms�������������� */
    rtos_systick_init(); 
#endif
    
    for (; ;) {         
         
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0;
        rtos_sched_mdelay(10);      
             
    }
}

/**
 * \brief �ڶ���������ں���
 */
void second_task_entry (void *p_arg)
{          
    for (; ;) {
        
        /*
         * ���������˴�ʱ�����ڻ����ź��������ȼ��̳л���
         * task2�����ȼ���1���0
         */        
       
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0;
        rtos_sched_mdelay(10);       
 
    }
}


/**
 * \brief RTOS Ӧ����������ʼ��
 */
void  rtos_task_app_init (void) 
{
    rtos_task_init(&first_task,   first_task_entry,  &g_task_flag1, 0,  first_task_stack_buf,   sizeof(first_task_stack_buf),RTOS_TASK_OPT_SAVE_FP); 
    rtos_task_init(&second_task,  second_task_entry, &g_task_flag2, 1,  second_task_stack_buf,  sizeof(second_task_stack_buf),RTOS_TASK_OPT_SAVE_FP); 
    
}


/**
 * \brief ��ں���
 */
int main (void)
{  
    uint32_t i = 0;
    
    /* �����ȼ���4λ�������ȼ�Ҳ��4λ */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));

    /* ʱ�ӳ�ʼ�� */
    stm32f4xx_hal_clk_init(&clk_dev, &clk_info);
    
    /* �δ��ʼ�� */
    rtos_systick_init();    
    
    /* ���ڴ�ӡ��ʼ�� */
    stm32f4xx_uart_init(&UART1_Handler, USART1, 115200);        
    microlib_adapter_init(NULL, &UART1_Handler);

    /* led��ʼ�� */
    stm32f4xx_led_init(GPIO_PIN_0, GPIOB, 1); 
    stm32f4xx_led_init(GPIO_PIN_1, GPIOB, 1); 

    SDRAM_Init();                   /* ��ʼ��SDRAM */
    
    PCF8574_Init();                 //��ʼ��PCF8574
    my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
    my_mem_init(SRAMEX);            //��ʼ���ⲿ�ڴ��
    my_mem_init(SRAMCCM);            //��ʼ��CCM�ڴ��
    
    /* ��ʱ����ʼ�� */
    stm32f4xx_hal_tim_init(&TIM3_Handler, TIM3, 1000-1, 900-1); 
     
    while(lwip_comm_init())         //lwip��ʼ��
    {
        printf("LWIP Init Falied! ");
        rtos_mdelay(500);
        printf("Retrying...       ");
        rtos_mdelay(500);
    }
    
    printf("LWIP Init Success!");
    printf("DHCP IP configing...");  //�ȴ�DHCP��ȡ 
#if LWIP_DHCP   //ʹ��DHCP
    while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//�ȴ�DHCP��ȡ�ɹ�/��ʱ���
    {  
        lwip_periodic_handle();    //LWIP�ں���Ҫ��ʱ����ĺ���
    }
#endif
    show_address(lwipdev.dhcpstatus);    //��ʾ��ַ��Ϣ
    while(1)
    {
        lwip_periodic_handle();    //LWIP�ں���Ҫ��ʱ����ĺ���
        rtos_udelay(200);
        i++;
        if(i==2000)
        {
            i=0;
            LED0=!LED0;
        }
    }   
    
    
    /* RTOS��ʼ�� */
    rtos_init();

/* CPU������ռ���ʵ�ʱ�� */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0
  
    /* �����ʼ������ */
    rtos_task_app_init();
  
#else
    
    /* �������ڲ���CPUռ���� */
    rtos_cpu_use_check_test(rtos_task_app_init);
    
    /* CPUռ���ʲ��Գ�ʼ�� */
    rtos_cpu_use_init();
    
#endif

    printf("rtos init complete\r\n");
    

#ifdef LWIP_SUPPORT_OS
    /* ��������ϵͳ, �Զ�����������ȼ�����������,���������Զ���᷵�� */
    rtos_start();
#endif

    
    return RTOS_OK;    
}




/* end of file */

