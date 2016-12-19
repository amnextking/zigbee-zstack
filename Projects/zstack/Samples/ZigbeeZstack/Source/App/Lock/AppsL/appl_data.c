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
  }
  
  for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                     //��ͨ���ڴ������֤
  {
    AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
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
    }
    
    for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                   //��ͨ���ڴ������֤
    {
      AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
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
 * @brief       ����ID��Ϣ��ʼ��
 * @param       ��
 * @return      ��
 *******************************************
 */
//void Data_DoorID_Init(void)
//{
//  uint8 DoorId[4] = {0x00};
//  uint8 Data[4] = {DOOR_COMPANY,DOOR_BUILD,DOOR_FLOOR,DOOR_ID};
//  AT24C256_WriteBuff(AT24C256_WRITE_ADDR,DOORID_BASE_ADDR,Data,4);
//  
////  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,DOORID_BASE_ADDR,DoorId,4);
//
//}




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
  
  for(i=COMMONCARD_BASE_ADDR; i<COMMONCARD_MAX1_ADDR; i+=4)                                                
  {
    AT24C256_ReadBuff(AT24C256_WRITE_ADDR,i,ReadData,4);
    if(strncmp((const char*)CardId,(const char*)ReadData,4)== 0)                //�ж��Ƿ��Ѿ���Ȩ���ÿ�
    {
      DataResult = DATA_ERR;
      break;
    }
    
    else if((NullPosition == COMMONCARD_MAX1_ADDR) || (UsedPosition == COMMONCARD_MAX1_ADDR))
    {
      if(NullPosition == COMMONCARD_MAX1_ADDR)
      {
        if(strncmp((const char*)ReadData,(const char*)NullRom,4)== 0)           //Ѱ�ҿ��ڴ�
        {
          NullPosition = i;                                                     //(RomPosition + 1) % 4  == 0
        }
      }
      
      if(UsedPosition == COMMONCARD_MAX1_ADDR)
      {
        if(strncmp((const char*)ReadData,(const char*)UsedRom,4)== 0)           //Ѱ��ʹ�ù����ڴ�
        {
          UsedPosition = i;                                                     //(UsedPosition + 1) % 4  == 0
        }
      }
    }
  }
  
  if(DataResult == DATA_OK)
  {
    if((NullPosition != COMMONCARD_MAX1_ADDR) && ((NullPosition % 4) == 0))
    {
      DataResult = Data_Storage(CardId,NullPosition);                           //��ͨ���洢 
    }
    
    else if((UsedPosition != COMMONCARD_MAX1_ADDR) && ((UsedPosition % 4) == 0))
    {
      DataResult = Data_Storage(CardId,UsedPosition);                           //��ͨ���洢 
    }
    
    else if((NullPosition == COMMONCARD_MAX1_ADDR) && (UsedPosition == COMMONCARD_MAX1_ADDR))
    {
      DataResult = DATA_FULL;                                                   //�洢��������
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
    
  AT24C256_ReadBuff(AT24C256_WRITE_ADDR,WriteAddr,ReadId,4);
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