#define APPH_DOOR_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "apph_door.h"

/**************************************************************************************************
 * @fn          Door_Open
 * @brief       ����(����)
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */
void Door_Open(uint8 Led)
{
  Buzzer_One_Led(Led);                             
  Motor_H1ON();                                                                 //��ֹ���ѿ����ɶ�һ��
  Motor_H1ON();
  Delay_Ms(10);       
  Motor_H2ON();
  Motor_H2ON();
  Delay_Ms(10);
  Motor_H1ON();
  Motor_H1ON();
  Delay_Ms(10);       
  Delay_Ms(10);
  Motor_H2ON();
  Motor_H2ON();
  Delay_Ms(10);
  Delay_Ms(10);
  Motor_H1ON();
  Motor_H1ON();
  Delay_Ms(10);       
  Delay_Ms(10);
  Delay_Ms(10);       
  Delay_Ms(10);
  Motor_H2ON();
  Motor_H2ON();
  Delay_Ms(10);
  Delay_Ms(10);
  Delay_Ms(10);       
  Delay_Ms(10);
  Motor_H1H2OFF();
  Delay_Ms(100);      
  Motor_H1ON();
  Motor_H1ON();
  Delay_Ms(100);      
  Delay_Ms(100);
  Motor_H1H2OFF();
  Motor_H1H2OFF();
  Buzzer_Door_Open();   
}

/**************************************************************************************************
 * @fn          Door_Close
 * @brief       ����
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */
void Door_Close(uint8 Led)
{
  Motor_H2ON();
  Motor_H2ON();
  Delay_Ms(10);   
  Motor_H1ON();       
  Motor_H1ON();
  Delay_Ms(10);
  Motor_H2ON();
  Motor_H2ON();
  Delay_Ms(10);  
  Delay_Ms(10);
  Motor_H1ON();       
  Motor_H1ON();
  Delay_Ms(10);
  Delay_Ms(10);
  Motor_H1ON();
  Motor_H1ON();
  Delay_Ms(10);
  Delay_Ms(10);
  Delay_Ms(10);       
  Delay_Ms(10);
  Motor_H2ON();
  Motor_H2ON();
  Delay_Ms(10);
  Delay_Ms(10);
  Delay_Ms(10);       
  Delay_Ms(10);
  Motor_H1H2OFF();
  Delay_Ms(100);
  Motor_H2ON();
  Motor_H2ON();
  Delay_Ms(100);  
  Delay_Ms(100);
  Motor_H1H2OFF();
  Motor_H1H2OFF();
  Buzzer_Door_Close(); 
  
  if(Led == LedOff)
  {
    LED_OFF();
  }
  
  else
  {
    LED_ON();
  }
  
}




/**************************************************************************************************
 * @fn          Door_OpenClose
 * @brief       ���Ų�����
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */

void Door_Open_Close(void)
{ 
  uint16 i = 60;                 //���������ʱ3s
  DoorStatus = PUSHOUT;           //����Ĭ�������棬û��ѹ��
  
  Door_Open(LedOn);
  
  //Ĭ����ʱ3s����
  while(i--) 
  {
    Delay_Ms(50);
    
    if((DOOR == PUSHOUT) && (DoorStatus == PUSHIN))   //�����Ѿ�����ѹ���ˣ���ʱ��������ֵ�����
                                                      //ģ����һ�ο��ŵĶ������ſ���ʱ��������־���һ�ο���
    {
      break;
    }
  }
  
  DoorStatus = PUSHOUT;
  Door_Close(LedOff);
}