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
 * \brief main函数入口
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

/** \brief 任务1标记 */
int g_task_flag1 = 0;

/** \brief 任务2标记 */
int g_task_flag2 = 0;

/** \brief 任务堆栈 */

 /* 按8字节对齐 */
__align(8) taskstack_t first_task_stack_buf[TASK_STACK_SIZE];
__align(8) taskstack_t second_task_stack_buf[TASK_STACK_SIZE];


/** \brief 当前任务结构体 */
rtos_task_t first_task;

/** \brief 下一个任务结构体 */
rtos_task_t second_task;

/**
 * \brief 任务状态信息
 */

/**
 * \brief 当前任务入口函数,相当于starts任务
 * \note  系统节拍初始化函数要等任务初始化函数完成后才能调用
 */
void first_task_entry (void *p_arg)
{  
/* CPU不测量占有率的时候 */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0    
    /* 确保任务被调度起来后，再初始化系统节拍周期为10ms，否则会出现问题 */
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
 * \brief 第二个任务入口函数
 */
void second_task_entry (void *p_arg)
{          
    for (; ;) {
        
        /*
         * 当运行至此处时，由于互斥信号量的优先级继承机制
         * task2的优先级由1变成0
         */        
       
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0;
        rtos_sched_mdelay(10);       
 
    }
}


/**
 * \brief RTOS 应用相关任务初始化
 */
void  rtos_task_app_init (void) 
{
    rtos_task_init(&first_task,   first_task_entry,  &g_task_flag1, 0,  first_task_stack_buf,   sizeof(first_task_stack_buf),RTOS_TASK_OPT_SAVE_FP); 
    rtos_task_init(&second_task,  second_task_entry, &g_task_flag2, 1,  second_task_stack_buf,  sizeof(second_task_stack_buf),RTOS_TASK_OPT_SAVE_FP); 
    
}


/**
 * \brief 入口函数
 */
int main (void)
{  
    uint32_t i = 0;
    
    /* 组优先级有4位，次优先级也有4位 */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));

    /* 时钟初始化 */
    stm32f4xx_hal_clk_init(&clk_dev, &clk_info);
    
    /* 滴答初始化 */
    rtos_systick_init();    
    
    /* 串口打印初始化 */
    stm32f4xx_uart_init(&UART1_Handler, USART1, 115200);        
    microlib_adapter_init(NULL, &UART1_Handler);

    /* led初始化 */
    stm32f4xx_led_init(GPIO_PIN_0, GPIOB, 1); 
    stm32f4xx_led_init(GPIO_PIN_1, GPIOB, 1); 

    SDRAM_Init();                   /* 初始化SDRAM */
    
    PCF8574_Init();                 //初始化PCF8574
    my_mem_init(SRAMIN);            //初始化内部内存池
    my_mem_init(SRAMEX);            //初始化外部内存池
    my_mem_init(SRAMCCM);            //初始化CCM内存池
    
    /* 定时器初始化 */
    stm32f4xx_hal_tim_init(&TIM3_Handler, TIM3, 1000-1, 900-1); 
     
    while(lwip_comm_init())         //lwip初始化
    {
        printf("LWIP Init Falied! ");
        rtos_mdelay(500);
        printf("Retrying...       ");
        rtos_mdelay(500);
    }
    
    printf("LWIP Init Success!");
    printf("DHCP IP configing...");  //等待DHCP获取 
#if LWIP_DHCP   //使用DHCP
    while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//等待DHCP获取成功/超时溢出
    {  
        lwip_periodic_handle();    //LWIP内核需要定时处理的函数
    }
#endif
    show_address(lwipdev.dhcpstatus);    //显示地址信息
    while(1)
    {
        lwip_periodic_handle();    //LWIP内核需要定时处理的函数
        rtos_udelay(200);
        i++;
        if(i==2000)
        {
            i=0;
            LED0=!LED0;
        }
    }   
    
    
    /* RTOS初始化 */
    rtos_init();

/* CPU不测量占有率的时候 */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0
  
    /* 任务初始化函数 */
    rtos_task_app_init();
  
#else
    
    /* 表明是在测试CPU占有率 */
    rtos_cpu_use_check_test(rtos_task_app_init);
    
    /* CPU占有率测试初始化 */
    rtos_cpu_use_init();
    
#endif

    printf("rtos init complete\r\n");
    

#ifdef LWIP_SUPPORT_OS
    /* 启动操作系统, 自动查找最高优先级的任务运行,这个函数永远不会返回 */
    rtos_start();
#endif

    
    return RTOS_OK;    
}




/* end of file */

