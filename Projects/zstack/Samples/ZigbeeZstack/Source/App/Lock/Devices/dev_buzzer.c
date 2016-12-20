#define DEV_BUZZER_C_


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "dev_buzzer.h"



/* ------------------------------------------------------------------------------------------------
 *                                          ϵͳ��ʾ��
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Buzzer_System_Start
 * @brief       ϵͳ������ʾ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_System_Start(void)
{
  LED_ON();
  BUZZER_ON();
  T4CC0   = GamutF;     
  Delay_Ms(100);
  LED_OFF();
  BUZZER_OFF();
}

/* ------------------------------------------------------------------------------------------------
 *                                          ��ͨ��ʾ��
 * ------------------------------------------------------------------------------------------------
 */
/**************************************
 * @fn          Buzzer_One
 * @brief       ��ͨ��ʾ��һ��
 * @param       ��
 * @return      ��
 **************************************
 */
void Buzzer_One(void)
{
  LED_ON();
  BUZZER_ON();
  T4CC0   = GamutB;     
  Delay_Ms(100);
  LED_OFF();
  BUZZER_OFF();
  Delay_Ms(100);
}

/***************************************
 * @fn          Buzzer_One_Led
 * @brief       ��ͨ��ʾ��һ��
 * @param       ��
 * @return      ��
 ***************************************
 */
void Buzzer_One_Led(uint8 Led)
{
  if(Led == LedOn)
  {
    LED_ON();
  }
  
  else
  {
    LED_OFF();
  }
  
  BUZZER_ON();
  T4CC0   = GamutB;     
  Delay_Ms(100);
  BUZZER_OFF();
  Delay_Ms(100);
  
}

/***************************************
 * @fn          Buzzer_Two
 * @brief       ��ͨ��ʾ������
 * @param       ��
 * @return      ��
 ***************************************
 */
void Buzzer_Two(void)
{
  Buzzer_One();
  Buzzer_One();
}

/***************************************
 * @fn          Buzzer_Three
 * @brief       ��ͨ��ʾ������
 * @param       ��
 * @return      ��
 ***************************************
 */
void Buzzer_Three(void)
{
  Buzzer_One();
  Buzzer_One();
  Buzzer_One();
}

/* ------------------------------------------------------------------------------------------------
 *                                          ��������ʾ��
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Buzzer_Door_Open
 * @brief       ������ʾ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_Door_Open(void)
{
  
  T4CC0   = GamutC; 
  BUZZER_ON();       
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
       
  T4CC0   = GamutDp;    
  BUZZER_ON();   
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
      
  T4CC0   = GamutG;    
  BUZZER_ON();   
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
}


/****************************************
 * @fn          Buzzer_Door_Close
 * @brief       ������ʾ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_Door_Close(void)
{       
  T4CC0   = GamutG;   
  BUZZER_ON();
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutDp;        
  BUZZER_ON();  
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutC; 
  BUZZER_ON();
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
}


/****************************************
 * @fn          Buzzer_Door_Lock
 * @brief       �ŷ���LED��ʾ
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_Door_Lock(void)
{
  LED_ON();
  Delay_Ms(100);
  LED_OFF();
  Delay_Ms(100);
  LED_ON();
  Delay_Ms(100);
  LED_OFF();
  Delay_Ms(100);
  LED_ON();
  Delay_Ms(100);
  LED_OFF();
  Delay_Ms(100);
}

/****************************************
 * @fn          Buzzer_Key_Open
 * @brief       Կ�׿�����ʾ
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_Key_Open(void)
{
  LED_ON();
  T4CC0   = GamutC; 
  BUZZER_ON();       
  Delay_Ms(100);
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
       
  T4CC0   = GamutB;    
  BUZZER_ON();   
  Delay_Ms(100);
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  LED_OFF();
  
  
}


/* ------------------------------------------------------------------------------------------------
 *                                          ��Ȩ״̬��ʾ��
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Buzzer_Card_Full
 * @brief       ��ͨ���б�����
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_Card_Full(void)
{
  LED_OFF();
  T4CC0   = GamutB;   
  BUZZER_ON();
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutC;        
  BUZZER_ON();  
  Delay_Ms(100);
  Delay_Ms(100);
  BUZZER_OFF();
  LED_ON();
}

/****************************************
 * @fn          Buzzer_Card_Success
 * @brief       ��ͨ���б���ȨɾȨ�ɹ�
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_Card_Success(void)
{
  LED_OFF();
  BUZZER_ON();
  T4CC0   = GamutC;     
  Delay_Ms(100);
  BUZZER_OFF();
  Delay_Ms(100);
  BUZZER_ON();
  T4CC0   = GamutB;     
  Delay_Ms(100);
  BUZZER_OFF();
  LED_ON();
}

/****************************************
 * @fn          Buzzer_Card_Fail
 * @brief       ��ͨ���б���ȨɾȨʧ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Buzzer_Card_Fail(void)
{
  Buzzer_One();
  Buzzer_One();
}



/**************************************************************************************************
 * @fn          Buzzer_Door_Init
 * @brief       ������ʼ����ʾ
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */
void Buzzer_Door_Init(void)
{
  T4CC0   = GamutB;   
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutG;        
  BUZZER_ON();  
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutE; 
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutA;   
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
  
      
  T4CC0   = GamutF;        
  BUZZER_ON();  
  Delay_Ms(100);
  BUZZER_OFF();
  
  
  T4CC0   = GamutDp; 
  BUZZER_ON();
  Delay_Ms(100);
  BUZZER_OFF();
}