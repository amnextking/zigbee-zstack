#define DEV_DRI_BUZZER_C_


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
 * @fn          Dev_Buzzer_System_Start
 * @brief       ϵͳ������ʾ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_System_Start(void)
{
  LED_ON();
  DRI_BUZZER_ON();
  T4CC0   = GamutF;     
  Dri_DelayMs(100);
  LED_OFF();
  DRI_BUZZER_OFF();
}

/* ------------------------------------------------------------------------------------------------
 *                                          ��ͨ��ʾ��
 * ------------------------------------------------------------------------------------------------
 */
/**************************************
 * @fn          Dev_Buzzer_One_Buzzer
 * @brief       ��ͨ��ʾ��һ��
 * @param       ��
 * @return      ��
 **************************************
 */
void Dev_Buzzer_One_Buzzer(void)
{
  LED_ON();
  DRI_BUZZER_ON();
  T4CC0   = GamutB;     
  Dri_DelayMs(100);
  LED_OFF();
  DRI_BUZZER_OFF();
  Dri_DelayMs(100);
}

/***************************************
 * @fn          Dev_Buzzer_OneBuzzer_Led
 * @brief       ��ͨ��ʾ��һ��
 * @param       ��
 * @return      ��
 ***************************************
 */
void Dev_Buzzer_OneBuzzer_Led(uint8 Led)
{
  if(Led == LedOn)
  {
    LED_ON();
  }
  
  else
  {
    LED_OFF();
  }
  
  DRI_BUZZER_ON();
  T4CC0   = GamutB;     
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  Dri_DelayMs(100);
  
}

/***************************************
 * @fn          Dev_Buzzer_Two_Buzzer
 * @brief       ��ͨ��ʾ������
 * @param       ��
 * @return      ��
 ***************************************
 */
void Dev_Buzzer_Two_Buzzer(void)
{
  Dev_Buzzer_One_Buzzer();
  Dev_Buzzer_One_Buzzer();
}

/***************************************
 * @fn          Dev_Buzzer_Three_Buzzer
 * @brief       ��ͨ��ʾ������
 * @param       ��
 * @return      ��
 ***************************************
 */
void Dev_Buzzer_Three_Buzzer(void)
{
  Dev_Buzzer_One_Buzzer();
  Dev_Buzzer_One_Buzzer();
  Dev_Buzzer_One_Buzzer();
}

/* ------------------------------------------------------------------------------------------------
 *                                          ��������ʾ��
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Dev_Buzzer_Door_Open
 * @brief       ������ʾ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_Door_Open(void)
{
  
  T4CC0   = GamutC; 
  DRI_BUZZER_ON();       
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
       
  T4CC0   = GamutDp;    
  DRI_BUZZER_ON();   
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
      
  T4CC0   = GamutG;    
  DRI_BUZZER_ON();   
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
}


/****************************************
 * @fn          Dev_Buzzer_Door_Close
 * @brief       ������ʾ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_Door_Close(void)
{       
  T4CC0   = GamutG;   
  DRI_BUZZER_ON();
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
      
  T4CC0   = GamutDp;        
  DRI_BUZZER_ON();  
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
  
  T4CC0   = GamutC; 
  DRI_BUZZER_ON();
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
}


/****************************************
 * @fn          Dev_Buzzer_Door_Lock
 * @brief       �ŷ���LED��ʾ
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_Door_Lock(void)
{
  LED_ON();
  Dri_DelayMs(100);
  LED_OFF();
  Dri_DelayMs(100);
  LED_ON();
  Dri_DelayMs(100);
  LED_OFF();
  Dri_DelayMs(100);
  LED_ON();
  Dri_DelayMs(100);
  LED_OFF();
  Dri_DelayMs(100);
}

/****************************************
 * @fn          Dev_Buzzer_Key_Open
 * @brief       Կ�׿�����ʾ
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_Key_Open(void)
{
  LED_ON();
  T4CC0   = GamutB; 
  DRI_BUZZER_ON();       
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
       
  T4CC0   = GamutC;    
  DRI_BUZZER_ON();   
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  LED_OFF();
  
  
}


/* ------------------------------------------------------------------------------------------------
 *                                          ��Ȩ״̬��ʾ��
 * ------------------------------------------------------------------------------------------------
 */
/****************************************
 * @fn          Dev_Buzzer_Card_Full
 * @brief       ��ͨ���б�����
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_Card_Full(void)
{
  LED_OFF();
  T4CC0   = GamutB;   
  DRI_BUZZER_ON();
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
      
  T4CC0   = GamutC;        
  DRI_BUZZER_ON();  
  Dri_DelayMs(100);
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  LED_ON();
}

/****************************************
 * @fn          Dev_Buzzer_Card_Success
 * @brief       ��ͨ���б���ȨɾȨ�ɹ�
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_Card_Success(void)
{
  LED_OFF();
  DRI_BUZZER_ON();
  T4CC0   = GamutC;     
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  Dri_DelayMs(100);
  DRI_BUZZER_ON();
  T4CC0   = GamutB;     
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  LED_ON();
}

/****************************************
 * @fn          Dev_Buzzer_Card_Fail
 * @brief       ��ͨ���б���ȨɾȨʧ��
 * @param       ��
 * @return      ��
 ****************************************
 */
void Dev_Buzzer_Card_Fail(void)
{
  Dev_Buzzer_One_Buzzer();
  Dev_Buzzer_One_Buzzer();
}



/**************************************************************************************************
 * @fn          Dev_Buzzer_Door_Init
 * @brief       ������ʼ����ʾ
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */
void Dev_Buzzer_Door_Init(void)
{
  T4CC0   = GamutB;   
  DRI_BUZZER_ON();
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
      
  T4CC0   = GamutG;        
  DRI_BUZZER_ON();  
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
  
  T4CC0   = GamutE; 
  DRI_BUZZER_ON();
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
  
  T4CC0   = GamutA;   
  DRI_BUZZER_ON();
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
      
  T4CC0   = GamutF;        
  DRI_BUZZER_ON();  
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
  
  
  T4CC0   = GamutDp; 
  DRI_BUZZER_ON();
  Dri_DelayMs(100);
  DRI_BUZZER_OFF();
}