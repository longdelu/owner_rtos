#include "c_lib.h"
#include "arm.h"

#pragma import(__use_no_semihosting)       
      
/** \brief ��׼����Ҫ��֧�ֺ��� */
                 
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout; 
      
/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */    
void _sys_exit(int x) 
{ 
    x = x; 
} 
/* �ض���fputc���� */
int fputc(int ch, FILE *f)
{   
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
    return ch;
}


 




