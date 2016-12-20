#define APPL_DATA_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "appl_data.h"

/***************************������Ϣ��ʼ��*************************************/

/*******************************************
 * @fn          Data_Door_Init
 * @brief       ������ʼ��
 * @param       ��
 * @return      DataResult -> DATA_ERR  ��ʼ��ʧ��
                DataResult -> DATA_OK   ��ʼ���ɹ�
 *******************************************
 */
uint8 Data_Door_Init(void)
{
  uint8 DataResult = DATA_OK;                                                 //���ݲ�����ʶ
  
  DataResult = Data_CommonCard_Init();                                          //��ͨ���б��ʼ��
  
  
  return DataResult;
}





/*******************************************
 * @fn          Data_CommonCard_Init
 * @brief       ��ͨ���б��ʼ��
 * @param       ��
 * @return      DataResult -> DATA_ERR  ��ʼ��ʧ��
                           -> DATA_OK   ��ʼ���ɹ�
 *******************************************
 */
uint8 Data_CommonCard_Init(void)
{
  uint8 NullCardRom[4] = {'=','=','=','='};                                   //��ͨ�����ڴ��ʶ
  uint8 ReadData[4]    = {0x00};                                            
  uint8 DataResult = DATA_OK;                                                 //���ݲ�����ʶ
  uint16 i;
  
  /*1.��ͨ���ڴ����*/
  for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                                        
  {
    AT24C256_WriteBuff(AT24C256_WRITE_ADDR,i,NullCardRom,4);
    Delay_Ms(5);                                                             //һ��Ҫ��һ��С��ʱ��д�Ͷ�Ҫ��ʱ�����
  }
  

  for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                     //��ͨ���ڴ������֤
  {
    AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
    Delay_Ms(5); 
    if(strncmp((const char*)ReadData,(const char*)NullCardRom,4) != 0)          //������ݲ����
    {
      DataResult = DATA_ERR;
      break;
    }
  }
  
  
  /*2.���д���ȡ��һ�£��ٴ����*/
  if(DataResult == DATA_ERR)
  {
    for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                                     
    {
      AT24C256_WriteBuff(AT24C256_WRITE_ADDR,i,NullCardRom,4);
      Delay_Ms(5); 
    }
    
    for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                   //��ͨ���ڴ������֤
    {
      AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
      Delay_Ms(5); 
      if(strncmp((const char*)ReadData,(const char*)NullCardRom,4) != 0)        //������ݲ����
      {
        DataResult = DATA_ERR;
        break;
      }
    }
  }
  
  return DataResult;
  
}

/*******************************************
 * @fn          Data_DoorID_Init
 * @brief       ����ID��Ϣ��ʼ��,������
 * @param       ��
 * @return      ��
 *******************************************
 */


void Data_DoorID_Init(void)
{
  uint8 data[4] = {0x00};
  
  uint8 Data[4] = {DOOR_COMPANY,DOOR_BUILD,DOOR_FLOOR,DOOR_ID};
  AT24C256_WriteBuff(AT24C256_WRITE_ADDR,DOORID_BASE_ADDR,Data,4);
  Delay_Ms(100);
  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,DOORID_BASE_ADDR,data,4);
  Delay_Ms(100);
}




/***************************����ID���ݲ���*************************************/

/*******************************************
 * @fn          Data_DoorID_Read
 * @brief       ����ID��Ϣ��ȡ
 * @param       DoorId -> ����ID
 * @return      ��
 *******************************************
 */
void Data_DoorID_Read(uint8 *DoorId)
{
  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,DOORID_BASE_ADDR,DoorId,4);
  Delay_Ms(5);
}


/***************************��ͨ�����ݲ���*************************************/

/*******************************************
 * @fn          Data_CommonCard_Auth
 * @brief       ��ͨ����Ȩ�洢
 * @param       CardId -> ��Ҫ��Ȩ�Ŀ���
 * @return      Result
                 -> DATA_OK      ��Ȩ�ɹ�
                 -> DATA_FULL    �������� 100��
                 -> DATA_ERR     ��������Ȩ���ߴ洢ʧ��
 *******************************************
 */
uint8 Data_CommonCard_Auth(uint8 *CardId)
{
  uint8 DataResult    = DATA_OK;
  uint8 ReadData[4]   = {0x00};
  uint8 NullRom[4]    = {'=','=','=','='};                                    //���ڴ��ʶ
  uint8 UsedRom[4]    = {'-','-','-','-'};                                    //ʹ�ù����ڴ��ʶ
  uint16 NullPosition = COMMONCARD_MAX1_ADDR;                                 //���ڴ�λ��
  uint16 UsedPosition = COMMONCARD_MAX1_ADDR;                                 //ʹ�ù��ڴ�
  uint16 i;
  
  
  /*1. ��ѯ���е���ͨ���б�*/
  for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                                                
  {
    /*1.1 ��ȡ4�ֽڵ���ͨ����ַ��Ϣ*/
    AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
    
    //�������Ҫ����ѯ��ȡ��ʱ��
    Delay_Ms(5);                                                              //һ��Ҫ����С��ʱ
    
    /*1.2 �����ͨ���Ѿ���Ȩ�����˳�*/
    if(strncmp((const char*)CardId,(const char*)ReadData,4)== 0)                
    {
      DataResult = DATA_ERR;
      break;
    }
    
    /*1.3 �������λ�ö�����¼���򲻻��ȥ�ж��� */
    else if((NullPosition == COMMONCARD_MAX1_ADDR) || (UsedPosition == COMMONCARD_MAX1_ADDR))
    {
      
      /*1.3.1 ��һ��ѭ��������������������¼�µ�λ���򲻻����*/
      if(NullPosition == COMMONCARD_MAX1_ADDR)
      {
        /*1.3.1.1 �����δʹ�ù����ڴ棬��¼���ڴ�λ�� */
        if(strncmp((const char*)ReadData,(const char*)NullRom,4)== 0)           
        {
          NullPosition = i;                                                     //(RomPosition + 1) % 4  == 0
        }
      }
      
      /*1.3.2 ��һ��ѭ��������������������¼�µ�λ���򲻻����*/
      if(UsedPosition == COMMONCARD_MAX1_ADDR)
      {
         /*1.3.2.1 �����ʹ�ù����Ǳ��������ڴ棬��¼��ʹ�ù������������ڴ� */
        if(strncmp((const char*)ReadData,(const char*)UsedRom,4)== 0)           
        {
          UsedPosition = i;                                                     //(UsedPosition + 1) % 4  == 0
        }
      }
    }
  }
  
  /*2. �������ͨ��û�б��ڹ�Ȩ*/
  if(DataResult == DATA_OK)
  {
    /*2.1 ���ڴ���λ�ã������ȴ洢���ڴ棬��Ҫע����ڴ������λ�ò�������COMMONCARD_MAX1_ADDR���������Ҫע����ڴ������λ�ò�������COMMONCARD_MAX1_ADDR-4*/
    if((NullPosition != COMMONCARD_MAX1_ADDR) && ((NullPosition % 4) == 0))
    {
      DataResult = Data_Storage(CardId,NullPosition);                           
    }
    
    /*2.2 ���ڴ�û��λ�ã����Ǵ洢��ʹ�ù����Ǳ��������ڴ棬��������Ϊ���ô洢���Ӿ��ȣ��򵥿���Ҳ����û�б��������ڴ棬��ȫ�����óɿ��ڴ�*/
    else if((UsedPosition != COMMONCARD_MAX1_ADDR) && ((UsedPosition % 4) == 0))
    {
      DataResult = Data_Storage(CardId,UsedPosition);                           
    }
    
    /*2.3 ����洢�������� */
    else if((NullPosition == COMMONCARD_MAX1_ADDR) && (UsedPosition == COMMONCARD_MAX1_ADDR))
    {
      DataResult = DATA_FULL;                                                 
    }
  }
  
  return DataResult;
}

/*******************************************
 * @fn          Data_CommonCard_UnAuth
 * @brief       ��ͨ��ɾȨ�洢
 * @param       CardId -> ��ҪɾȨ�Ŀ���
 * @return      Result
                 -> DATA_OK     ɾ��ͨ���ɹ�
                 -> DATA_ERR    ɾ��ͨ��ʧ�ܣ�û�����ſ�,���ߴ洢����ʧ��
 *******************************************
 */
uint8 Data_CommonCard_UnAuth(uint8 *CardId)
{
  uint8 DataResult  = DATA_ERR;
  uint8 ReadData[4] = {0x00};
  uint8 UsedRom[4]  = {'-','-','-','-'};                                      //ʹ�ù����ڴ��ʶ
  uint16 i;
  
  for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4) 
  {
    AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
    
    Delay_Ms(5);    //����5ms��С��ʱ
    
    if(strncmp((const char*)CardId,(const char*)ReadData,4) == 0)               //�ж��Ƿ�����Ȩ���ÿ�
    {
      DataResult = Data_Storage(UsedRom,i);
      break;
    }
  }
  
  return DataResult;
}


/*******************************************
 * @fn          Data_CommonCard_Confirm
 * @brief       ��ͨ��ȷ��
 * @param       CardId -> ��Ҫȷ�ϵĿ���
 * @return      Result
                 -> DATA_OK     �иÿ�
                 -> DATA_ERR    �޸ÿ�
 *******************************************
 */
uint8 Data_CommonCard_Confirm(uint8 *CardId)
{
  uint8 DataResult = DATA_ERR;
  uint8 ReadData[4] = {0x00};
  uint16 i;
  
  for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4) 
  {
    AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
    Delay_Ms(5);    
    if(strncmp((const char*)CardId,(const char*)ReadData,4)== 0)                //�ж��Ƿ�����Ȩ���ÿ�
    {
      DataResult = DATA_OK;
      break;
    }
  }
  
  return DataResult;
}
















/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/*******************************************
 * @fn          Data_Storage
 * @brief       �洢�ж�
 * @param       WriteId   -> ��Ҫ�洢�Ŀ���
                WriteAddr -> �洢��λ��

 * @return      Result
                 -> AT24C256_OK     �洢�ɹ�
                 -> AT24C256_ERR    �洢ʧ��
 *******************************************
 */
uint8 Data_Storage(uint8 *WriteId, uint16 WriteAddr)
{
  uint8 ReadId[4] = {0x00};
  uint8 DataResult = DATA_OK;
  
  AT24C256_WriteBuff(AT24C256_WRITE_ADDR,WriteAddr,WriteId,4);
  
  
  Delay_Ms(5);    //д��֮����Ҫ����5ms��С��ʱ
    
  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,WriteAddr,ReadId,4);
  
  Delay_Ms(5);    
  
  if(strncmp((const char*)ReadId,(const char*)WriteId,4) != 0)                  //�жϴ洢���Ƿ�ɹ�
  {
    AT24C256_WriteBuff(AT24C256_WRITE_ADDR,WriteAddr,WriteId,4);
    if(strncmp((const char*)ReadId,(const char*)WriteId,4) != 0)                //�ٴ��жϴ洢���Ƿ�ɹ�
    {
      DataResult = DATA_ERR;                                                    //�洢ʧ��
    }
  } 

  return DataResult;
}