#define DEV_AT24C256_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "dev_at24c256.h"


/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */


/**************************************************************************************************
 * @fn          AT24C256_DelayMs
 * @brief       AT24C256��ʱ����
 * @param       n -> ��ʱ������
 * @return      ��
 **************************************************************************************************
 */
void AT24C256_DelayMs(uint16 n)     //��Ҫע�������ʱ������֮ǰ����ʱ������ͬ
{
  uint16 i,j;
  for(i=0;i<n;i++)
  for(j=0;j<200;j++);
  for(j=0;j<200;j++);
  for(j=0;j<187;j++);
}

//֮ǰ����ʱ����
//void AT24C256_DelayMs(uint16_t n)
//{
//  uint16_t i,j;
//  for(i=0;i<n;i++)
//  for(j=0;j<587;j++);
//}


/**************************************************************************************************
 * @fn          AT24C256_WriteByte
 * @brief       EEPROMдһ�ֽ�����
 * @param       SlaveAddr -> �ӻ���ַ
                ByteAddr  -> ���ݵ�ַ(16bit��Ҫ���)��MAX 0X4000��
                Data      -> ��д�������
 * @return      AckFlag   -> 0 ��Ӧ�� 1 ��Ӧ�� 
 **************************************************************************************************
 */
uint8 AT24C256_WriteByte(uint8 SlaveAddr,uint16 ByteAddr,uint8 Data)
{
  uint8 AckFlag;
  
  I2C_Start();                                  //��������
  AckFlag = I2C_SendByte(SlaveAddr);            //���ʹӻ���ַ��������ַ��
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte((ByteAddr>>8)&0xFF);   //���͵�ַ�߰�λ      
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(ByteAddr&0xFF);        //���͵�ַ�Ͱ�λ
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(Data);                 //����Ҫ�洢������
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  I2C_Stop();                                   //��������
  AT24C256_DelayMs(12);                         //��Ҫ��С��ʱ
  return 1;
}

/**************************************************************************************************
 * @fn          AT24C256_WriteBuff
 * @brief       EEPROMд���ֽ�����
 * @param       SlaveAddr -> �ӻ���ַ
                ByteAddr  -> ���ݵ�ַ(16bit��Ҫ���)��MAX 0X4000��
                Data      -> ��д�������
                Len       -> д���������            (MAX 64)
 * @return      AckFlag   -> 0 ��Ӧ�� 1 ��Ӧ�� 
 **************************************************************************************************
 */
uint8 AT24C256_WriteBuff(uint8 SlaveAddr, uint16 ByteAddr, uint8 *Data, uint8 Len)
{
  uint8 AckFlag;
  uint8 i;
  
  I2C_Start();                                  //��������
  AckFlag = I2C_SendByte(SlaveAddr);            //���ʹӻ���ַ��������ַ��
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte((ByteAddr>>8)&0xFF);   //���͵�ַ�߰�λ      
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(ByteAddr&0xFF);        //���͵�ַ�Ͱ�λ
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  for(i=0; i<Len; i++)
  {
    AckFlag = I2C_SendByte(Data[i]);            //����Ҫ�洢������
    if(AckFlag == 1)                            //��û��Ӧ��
    {
      return 0;
    }
  }
  
  I2C_Stop();                                   //��������
  AT24C256_DelayMs(12);                         //��Ҫ��С��ʱ
  return 1;
}

/**************************************************************************************************
 * @fn          AT24C256_ReadByte
 * @brief       EEPROM��һ�ֽ�����
 * @param       SlaveAddr -> �ӻ���ַ
                ByteAddr  -> ���ݵ�ַ(16bit��Ҫ���)��MAX 0X4000��
                Data      -> ����ȡ������
 * @return      AckFlag   -> 0 ��Ӧ�� 1 ��Ӧ�� 
 **************************************************************************************************
 */
uint8 AT24C256_ReadByte(uint8 SlaveAddr,uint16 ByteAddr,uint8 *Data)
{
  uint8 AckFlag;
  
  I2C_Start();                                  //��������
  AckFlag = I2C_SendByte(SlaveAddr);            //���ʹӻ���ַ��������ַ��
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte((ByteAddr>>8)&0xFF);   //���͵�ַ�߰�λ      
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(ByteAddr&0xFF);        //���͵�ַ�Ͱ�λ
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  I2C_Start();     
  AckFlag = I2C_SendByte(SlaveAddr + 1);        //�����Ƕ����ݣ�ע��Ĭ�����λ��0������д���ݣ�+1���1
  if(AckFlag == 1)                        
  {
    return 0;
  }
  
  *Data = I2C_ReceiveByte();                    //��ȡ����
  I2C_SendAck(1);                               //���һ���ֽ� No Acknowledge ������ҪʹSDA = 1 
                           
  I2C_Stop();                                   //��������
  return 1;
}



/**************************************************************************************************
 * @fn          AT24C256_ReadBuff
 * @brief       EEPROM�����ֽ�����
 * @param       SlaveAddr -> �ӻ���ַ
                ByteAddr  -> ���ݵ�ַ(16bit��Ҫ���)��MAX 0X4000��
                Data      -> ��ȡ������
                Len       -> ��ȡ��������           
 * @return      AckFlag   -> 0 ��Ӧ�� 1 ��Ӧ�� 
 **************************************************************************************************
 */
uint8 AT24C256_ReadBuff(uint8 SlaveAddr, uint16 ByteAddr, uint8 *Data, uint8 Len)
{
  uint8 AckFlag;
  uint8 i;
  
  I2C_Start();                                  //��������
  AckFlag = I2C_SendByte(SlaveAddr);            //���ʹӻ���ַ��������ַ��
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte((ByteAddr>>8)&0xFF);   //���͵�ַ�߰�λ      
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  AckFlag = I2C_SendByte(ByteAddr&0xFF);        //���͵�ַ�Ͱ�λ
  if(AckFlag == 1)                              //��û��Ӧ��
  {
    return 0;
  }
  
  I2C_Start();     
  AckFlag = I2C_SendByte(SlaveAddr + 1);        //�����Ƕ����ݣ�ע��Ĭ�����λ��0������д���ݣ�+1���1
  if(AckFlag == 1)                        
  {
    return 0;
  }
  
  for(i=0; i<Len-1; i++)
  {
    Data[i] = I2C_ReceiveByte();                //��ȡ�洢������
    I2C_SendAck(0);  
  }
  
  Data[Len-1] = I2C_ReceiveByte();
  I2C_SendAck(1);
 
  I2C_Stop();                                   //��������
  return 1;
}