#define APP_DELAY_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "app_tcp.h"
#include "hal_mcu.h"      //����Ҫ�����жϵĺ궨��



/*ȫ�ֱ���*/
typedef struct{
  uint8 RxStatus;         //����λ�����������͵�����֡���������
  uint8 RxProgress;       //��ǰһ֡���ݵĽ��ս���
  uint8 RxCount;          //����ֵ
  uint8 CMD;              //������
  uint8 LENG;             //֡����
  uint8 SEQ;              //֡����
  uint8 BASE_ID[2];       //��վID
  uint8 DOOR_ID[2];       //����ID
  uint8 RESP;             //����״̬
  uint8 DATA[Rx_BUFF];    //����
  uint8 LRC;              //У��
  
}Typedef_Frame;

Typedef_Frame Frame; 






/* ------------------------------------------------------------------------------------------------
 *                                          �жϺ���
 * ------------------------------------------------------------------------------------------------
 */
uint8 Uart0_Rx;


HAL_ISR_FUNCTION( UART0_ISR, URX0_VECTOR )
{
  URX0IF    = 0;  
  Uart0_Rx  = U0DBUF;    
  Tcp_Receive(Uart0_Rx);
}


/* ------------------------------------------------------------------------------------------------
 *                                          Ӧ�ú���
 * ------------------------------------------------------------------------------------------------
 */



/**************************************************************************************************
 * @fn          Tcp_Init
 * @brief       ����֡������ʼ��
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */
void Tcp_Init(void)
{
  Frame.RxStatus   =  FRAME_RX_START;  
  Frame.RxProgress =  FRAME_RX_SOP_START;
  Frame.RxCount   =  0x00;
  
  Frame.CMD        =  0x00;
  Frame.LENG       =  0x00;
  Frame.SEQ        =  0x00;
  memset(Frame.BASE_ID,0x00,2);
  memset(Frame.DOOR_ID,0x00,2);
  Frame.RESP       =  0x00;
  memset(Frame.DATA,0x00,Rx_BUFF);
  Frame.LRC        =  0x00;
 
}


/**************************************************************************************************
 * @fn          Tcp_Receive
 * @brief       ���ܷ��������͵�TCP�����ֽ�
 * @param       Rx_Data -> ���ڽ��յ�����
 * @return      ��
 **************************************************************************************************
 */
void Tcp_Receive(uint8 Rx_Data)
{
  uint8 data[2] = {0x44,0x45};
  
  
  if(Frame.RxStatus == FRAME_RX_START)     //���������ʱ��ȥ�����������ݵ�ʵʱ�Կ��ܻ���Ӱ��
  {
    //TcpClient_Process(Rx_Data);
  }
}






/**************************************************************************************************
 * @fn          Tcp_SendtoPC
 * @brief       SendType -> ���͵����ͣ�Ѱ��������Ƿ������
                Id       -> BASE_ID + DOOR_ID
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */

void Tcp_SendtoPC(uint8 CMD, uint8* Id, uint8 RESP, uint8 *data, uint8 DataLength)
{
    /*------------------------------------------------------------------------------------------------------------*/
    /* Command    0       1        2       3       4         5       6           7            8     9             */
    /*------------------------------------------------------------------------------------------------------------*/
    /* Name      SOP       CMD   LENG    SEQ    BASE_ID   DOOR_ID  RESP  Data[0]_Data[n-1]  LRC    EOP           */
    /*------------------------------------------------------------------------------------------------------------*/
    /* Values    0xAA                                                                               0x0E          */
    /*------------------------------------------------------------------------------------------------------------*/
    /* No.Byte   2byte   1byte    1byte   1byte   2byte    2byte    1byte     n-byte        1-byte  1-byte        */
    /*------------------------------------------------------------------------------------------------------------*/
    /*1.Ѱ������ FPB_CMD:0x00 RESP:0xEE*/
    /*2.�������� FPB_CMD:������  RESP:����״̬*/
  
    uint8 i;
    uint8 LRC;
    uint8 TxCount = 0;
    uint8 frame[Tx_BUFF] = {0x00};
    
    /*SOP*/
    frame[TxCount++] = SOP;
    frame[TxCount++] = SOP;
    
    /*CMD*/
    frame[TxCount++] = CMD;
    
    /*LENG*/
    frame[TxCount++] = DataLength + 6;
    
    /*SEQ*/
    frame[TxCount++] = SEq;   //Ŀǰ�̶�0x01
    
    /*ID*/
    for(i=0; i<4; i++)        //BASE_ID + DOOR_ID
    {
      frame[TxCount++] = Id[i];
    }
    
    /*RESP*/
    frame[TxCount++] = RESP;
    
    /*Data[0]_Data[n-1]*/
    if(DataLength != 0x00)
    {
      for(i=0; i<DataLength;i++)
      {
        frame[TxCount++] = data[i];
      }  
    }
    
    LRC = frame[2];
    
    /*LRC*/
    for(i=3;i<TxCount; i++)
    {
      LRC = LRC ^ frame[i];
    }
    frame[TxCount++] = LRC;
    
    /*EOP*/
    frame[TxCount++] = EOP;
    
    Uart0_SendString(frame,TxCount);
}
