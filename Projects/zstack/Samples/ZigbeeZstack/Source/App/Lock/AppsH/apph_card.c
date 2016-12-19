#define CARD_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "apph_card.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Local Variable
 * ------------------------------------------------------------------------------------------------
 */
uint8  SectorKeyA[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};                       //A������Կ





/* ------------------------------------------------------------------------------------------------
 *                                          Applications
 * ------------------------------------------------------------------------------------------------
 */


 uint8 ReadData[BlockSize]  = {0x00};                                        //��M1����������     

/**************************************************************************************************
 * @fn          Card_AccountInit
 * @brief       ��Ȩ����ʼ����ֻ��һ��
 * @param       ��       
 * @return      ��
 **************************************************************************************************
 */
void Card_Init(void)
{
  /*
    CARD_INFORMATION M1��62���� 16byte����
    |-------------------------------------------------------|
    |         | Byte0-1  | Byte2-5  |           |  
    | BLOCK62 |---------------------------------------------|
    |         | CardType | DoorId   |           |    
    |-------------------------------------------------------|
  
    CardType: 0x01 0x01 ��Ȩ����ʶ
              0x02 0x02 ɾȨ����ʶ
              0x03 0x03 �տ���ʶ
              0x04 0x04 �ܿ���ʶ(�����ſɿ�)
  
    DoorId  : Byte2 - DOOR_COMPANY   ������ͨ�����տ����ⶼ��Ҫ��Щ������Ϣ
              Byte3 - DOOR_BUILD     
              Byte4 - DOOR_FLOOR
              //Byte5 - DOOR_ID   
  */
  
  
  /*0.��Ȩ��,ɾȨ��,�ܿ���������....*/
  uint8 Status                =  MFRC522_ERR;
  uint8 AuthCard[BlockSize]   = {Authorization,Authorization,DOOR_COMPANY,DOOR_BUILD};         //��Ȩ��
  uint8 UnAuthCard[BlockSize] = {UnAuthorizataion,UnAuthorizataion,DOOR_COMPANY,DOOR_BUILD};   //ɾȨ��
  uint8 TotCard[BlockSize]    = {TotalCard,TotalCard,DOOR_COMPANY,DOOR_BUILD};                                        //�ܿ�
                   

  /*1.��Ȩ��*/  
  Status = Card_ReadBlock(CARD_INFORMATION,ReadData);                           //������д����Ҫ��Card_ReadBlock��Reset����
  if(Status == MFRC522_OK)
  {
    Status = Card_WriteBlock(CARD_INFORMATION,AuthCard);
    if(Status == MFRC522_OK)
    {
      Buzzer_One();
    }
  }
  
  /*2.ɾȨ��*/
  Status = Card_ReadBlock(CARD_INFORMATION,ReadData);
  if(Status == MFRC522_OK)
  {
    Status = Card_WriteBlock(CARD_INFORMATION,UnAuthCard);
    if(Status == MFRC522_OK)
    {
      Buzzer_One();
    }
  }
  
  
  /*3.�ܿ�*/
  Status = Card_ReadBlock(CARD_INFORMATION,ReadData);
  if(Status == MFRC522_OK)
  {
    Status = Card_WriteBlock(CARD_INFORMATION,TotCard);
    if(Status == MFRC522_OK)
    {
      Buzzer_One();
    }
  }
 
}



/**************************************************************************************************
 * @fn          Card_Authorization
 * @brief       ��Ȩ������ˢ��Ȩ��֮��3S��Ѱ����Ȩ����
 * @param       CardType    ->  ����Ȩ���ͣ���Ȩ������ɾȨ��     
 * @return      Status      ->  MFRC522_OK  ��ȷ  
                                MFRC522_ERR ����
 **************************************************************************************************
 */
uint8 Card_Authorization(uint8 CardOperType)
{
  uint8 Status               = MFRC522_ERR;                                                                       
  uint8 ReadData[BlockSize]  = {0x00};       //��M1���������� 
  uint8 CardCount            = 0;            //ˢ������
  uint8 DoorId[4]            = {0x00};       //����ID
  uint8 DataResult           = DATA_ERR;   
  uint16 i = 10;                             //���������ʱ2s
  
  
  /*1. Ĭ����Ȩ��ɾȨ��ʱ3s*/
  while(i--) 
  {
    Delay_Ms(20);
    
//    /*1.1 ��ȡ������������*/
//    Status = Card_ReadBlock(CARD_INFORMATION,ReadData);
//    
//    /*1.2 �ж�����*/
//    if(Status == MFRC522_OK) 
//    {
//      /*1.2.1 ��Ȩ����*/
//      if(CardOperType == Authorization)
//      {
//        
//        /*1.2.1.1 ��Ȩ����ɾȨ�����ܿ����������Ѿ������ٴα���Ȩ*/
//        if(((ReadData[0] == Authorization)    && (ReadData[1] == Authorization))    ||
//           ((ReadData[0] == UnAuthorizataion) && (ReadData[1] == UnAuthorizataion)) ||
//           ((ReadData[0] == TotalCard)        && (ReadData[1] == TotalCard)))
//        {
//          Status = MFRC522_ERR; 
//        }
//        
//        /*1.2.1.2 ��ͨ����Ȩ*/
//        else 
//        {
//          /*1.2.1.2.1 ��ͨ���洢EEPROM*/
//          DataResult = Data_CommonCard_Auth(ReadData+12);   
//          
//          
//          /*1.2.1.2.2 ��ͨ����Ȩ�ɹ�*/
//          if(DataResult == DATA_OK)
//          {
//            Status =  MFRC522_OK; 
//          }
//          
//          /*1.2.1.2.3 ����ͨ���Ѿ���Ȩ���ߴ洢ʧ��*/
//          else if(DataResult == DATA_ERR)                                       
//          {
//            Status = MFRC522_ERR;
//          }
//          
//          /*1.2.1.2.4 ��ͨ���洢�б�����*/
//          else if(DataResult == DATA_FULL)
//          {                                                   
//            Buzzer_Card_Full();
//            break;      //�˳�ѭ��������     
//          }
//          
//        }
//      }
//      
//       
//      /*1.2.2 ɾȨ����*/
//      else if(CardOperType == UnAuthorizataion)
//      {
//        
//      }  
//      
//      /*1.2.3 �����ɹ�*/
//      if((Status == MFRC522_OK) && (DataResult != DATA_FULL) && (CardCount == 0))
//      {    
//        Buzzer_Card_Success();    
//        i = 300;                //�����ظ���Ȩ 
//      }
//      
//      /*1.2.4 ����ʧ��*/
//      if((Status == MFRC522_ERR) && (DataResult != DATA_FULL) && (CardCount == 0))
//      {
//        Buzzer_Card_Fail();
//        LED_ON();
//        i = 300;                //�����ظ���Ȩ 
//      }
//    }  
  }
  
  
  return Status;

}









/* ------------------------------------------------------------------------------------------------
 *                                          Middle
 * ------------------------------------------------------------------------------------------------
 */
/**************************************************************************************************
 * @fn          Card_ReadID
 * @brief       ��ȡ����
 * @param       BlockAddr   ->  �����ַ
                BlockData   ->  ��������           
 * @return      Status      ->  MFRC522_OK  ��ȷ  
                                MFRC522_ERR ����
 **************************************************************************************************
 */
uint8 Card_ReadID(uint8 BlockAddr, uint8 *CardId)
{
  uint8 Status;
  uint8 Block[BlockSize]  = {0x00};
  
  MFRC522_AntennaOn();                                                          //ʵ�������Ķ��������Ͷ�д�������뿪������
  //memcpy(WriteBlock,BlockData,6);                                             //��Ҫ��ֵ�������ͣ���Ȩ��������ͨ����
  Status = Card_Process(CardId,BlockAddr,Block,READ_ID);                        //��дģʽ����ʵ�������ô��俨��ID     
  MFRC522_AntennaOff();
  Delay_Ms(10);

  return Status;
}



/**************************************************************************************************
 * @fn          Card_WriteBlock
 * @brief       д������������
 * @param       BlockAddr   ->  �����ַ
                BlockData   ->  ��������           
 * @return      Status      ->  MFRC522_OK  ��ȷ  
                                MFRC522_ERR ����
 **************************************************************************************************
 */
uint8 Card_WriteBlock(uint8 BlockAddr, uint8 *BlockData)
{
  uint8 CardId[6]       = {0x00};
  uint8 WriteBlock[BlockSize]  = {0x00};
  uint8 ReadBlock[BlockSize] = {0x00};   
  uint8 Status;
  
  /*1.д�뿨������*/
  MFRC522_Reset();                                                              //д�������ã����Ծ�Reset��   
  MFRC522_AntennaOn();                                                          //ʵ�������Ķ��������Ͷ�д�������뿪������
  memcpy(WriteBlock,BlockData,BlockSize);                                       //��ֵ���Ż�����Ϣ                 
  Status = Card_Process(CardId,BlockAddr,WriteBlock,WRITE_BLOCK);               //��дģʽ����ʵ�������ô��俨��ID
  MFRC522_AntennaOff();
  Delay_Ms(10);
  if(Status == MFRC522_ERR)                                                     //�������Ȩ�������Ӧ��ֱ�ӷ�����
  {
    return MFRC522_ERR;
  }
  
  /*2.���д��ɹ������ȡд�������*/
  MFRC522_Reset();                                                              //Ҫʵ�������Ķ�д������������Reset 
  MFRC522_AntennaOn();  
  Status = Card_ReadBlock(CARD_INFORMATION,ReadBlock);
  if(Status == MFRC522_OK)
  {
    if(strncmp((const char*)ReadBlock,(const char*)BlockData,4)!= 0)
    {
      Status = MFRC522_ERR;                                                     //д��ĺͶ�ȡ�Ĳ�һ��
    }
    
    else
    {
      Status = MFRC522_OK;                                                      //д��ĺͶ�ȡ��һ��
      return Status;
    }
  }
  
  /*3.���д���ȡ��һ�£�������д��*/
  if(Status == MFRC522_ERR)
  { 
    MFRC522_Reset();                                                            //д�������ã����Ծ�Reset��   
    MFRC522_AntennaOn();                                                        //ʵ�������Ķ��������Ͷ�д�������뿪������              
    Status = Card_Process(CardId,BlockAddr,WriteBlock,WRITE_BLOCK);             //��дģʽ����ʵ�������ô��俨��ID
    MFRC522_AntennaOff();
    Delay_Ms(10);
    if(Status == MFRC522_ERR)                                                   //�������Ȩ�������Ӧ��ֱ�ӷ�����
    {
      return MFRC522_ERR;
    }
    
    /*���¶�ȡд�������*/
    MFRC522_Reset();                                                            //Ҫʵ�������Ķ�д������������Reset 
    MFRC522_AntennaOn();  
    Status = Card_ReadBlock(CARD_INFORMATION,ReadBlock);
    if(Status == MFRC522_OK)
    {
      if(strncmp((const char*)ReadBlock,(const char*)BlockData,4)!= 0)          //д��ĺͶ�ȡ�Ĳ�һ��
      {
        Status = MFRC522_ERR;
      }
        
      else
      {
        Status = MFRC522_OK;                                                    //д��ĺͶ�ȡ��һ��
      }
    } 
  }
  
  return Status;
}


/**************************************************************************************************
 * @fn          Card_ReadBlock
 * @brief       ��������������
 * @param       BlockAddr   ->  �����ַ
                BlockData   ->  ��������           
 * @return      Status      ->  MFRC522_OK  ��ȷ  
                                MFRC522_ERR ����
 **************************************************************************************************
 */
uint8 Card_ReadBlock(uint8 BlockAddr, uint8 *BlockData)
{
  uint8 CardId[6]       = {0x00};
  uint8 ReadBlock[BlockSize] = {0x00};
  uint8 Status;
  
  //MFRC522_Reset();                                                              //Ҫʵ�������Ķ�д������������Reset�����������Ҫע��
  MFRC522_AntennaOn();  
  
  Status = Card_Process(CardId,BlockAddr,ReadBlock,READ_BLOCK);
  if(Status == MFRC522_ERR)
  {
    return MFRC522_ERR;
  }
  memcpy(BlockData,ReadBlock,BlockSize);                                        //���Ż�����Ϣ
  memcpy(BlockData+12,CardId,BlockSize-12);                                     //����ID
 
  MFRC522_AntennaOff();
  Delay_Ms(10);
  
  return Status;
}


/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */
/**************************************************************************************************
 * @fn          Card_Process
 * @brief       ��������
 * @param       CardId      ->  ��ȡ�Ŀ���ID
                BlockAddr   ->  �����ַ
                BlockData   ->  ��������
                ProcessType ->  ��������  
                                WRITE_BLOCK д��������
                                READ_BLOCK  ����������
 * @return      Status      ->  MFRC522_OK  ��ȷ  
                                MFRC522_ERR ����
 **************************************************************************************************
 */
uint8 Card_Process(uint8 *CardId, uint8 BlockAddr, uint8 *BlockData, uint8 ProcessType)
{
  
  
  uint8 Status;
  uint8 CardData[BlockSize] = {0x00};
  //uint16_t CardType;

  /*1.Ѱ��,���ؿ�������*/
  Status = MFRC522_RequestCard(PICC_REQIDL,CardData);
  if(Status == MFRC522_OK)
  {
    //CardType = (CardData[0]<<8)| CardData[1];                                 //Mifare_One(S50) У԰��
  }
  
  /*2.����ײ�����ؿ�������*/
  Status = MFRC522_Anticoll(CardData);
  if(Status == MFRC522_OK)
  {
    memcpy(CardId,CardData,5);   
  }
  
  if(ProcessType == WRITE_BLOCK)
  {
    /*3.ѡ��,���ؿ�����*/ 
    MFRC522_SelectTag(CardId);                                                  //Size = 0x08
    
    /*4.д����*/
    Status = MFRC522_Auth(PICC_AUTHENT1A,BlockAddr,SectorKeyA,CardId);
    if(Status == MFRC522_OK)
    {
      Status = MFRC522_WriteToBlock(BlockAddr,BlockData);
    }
  }
 
  else if(ProcessType == READ_BLOCK)
  {
    /*3.ѡ��,���ؿ�����*/ 
    MFRC522_SelectTag(CardId);                
    
    /*4.������*/
    Status = MFRC522_Auth(PICC_AUTHENT1A,BlockAddr,SectorKeyA,CardId);
    if(Status == MFRC522_OK)
    {
      Status = MFRC522_ReadFromBlock(BlockAddr,CardData);
      if(Status == MFRC522_OK)
      {
        memcpy(BlockData,CardData,BlockSize);                                   //�������ȷ
      }
    }  
  }
  
  return Status;
}




