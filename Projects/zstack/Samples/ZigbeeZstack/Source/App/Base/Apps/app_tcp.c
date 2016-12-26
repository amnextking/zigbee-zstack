#define APP_TCP_C_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "app_tcp.h"
#include "hal_mcu.h"      //����Ҫ�����жϵĺ궨��

/* ------------------------------------------------------------------------------------------------
 *                                          ��ʼ������
 * ------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 * @fn          Tcp_Init
 * @brief       ����֡�����ĳ�ʼ��
 * @param       ��
 * @return      ��
 **************************************************************************************************
 */
void Tcp_Init(void)
{
  Frame.RxData     =  0x00;
  Frame.RxStatus   =  FRAME_RX_START;  
  Frame.RxProgress =  FRAME_RX_SOP_START;
  Frame.RxCount   =  0x00;
  
  Frame.CMD        =  BASE_CMD_INIT;  //������
  Frame.LENG       =  0x00;
  memset(Frame.DOOR_ID,0x00,2);
  Frame.RESP       =  0x00;
  memset(Frame.DATA,0x00,Rx_BUFF);
  Frame.LRC        =  0x00;
 
}








/* ------------------------------------------------------------------------------------------------
 *                                          �жϺ���
 * ------------------------------------------------------------------------------------------------
 */


HAL_ISR_FUNCTION( UART0_ISR, URX0_VECTOR )
{
  URX0IF    = 0;  
  Frame.RxData  = U0DBUF;    
  Tcp_Receive(Frame.RxData);
}


/* ------------------------------------------------------------------------------------------------
 *                                          Ӧ�ú���
 * ------------------------------------------------------------------------------------------------
 */




/**************************************************************************************************
 * @fn          Tcp_Receive
 * @brief       ���ܷ��������͵�TCP�����ֽ�
 * @param       Rx_Data -> ���ڽ��յ�����
 * @return      ��
 **************************************************************************************************
 */
void Tcp_Receive(uint8 Rx_Data)
{
  if(Frame.RxStatus == FRAME_RX_START)     //���������ʱ��ȥ�����������ݵ�ʵʱ�Կ��ܻ���Ӱ��
  {
    Tcp_Process(Rx_Data);
  }
}






/**************************************************************************************************
 * @fn          Tcp_Process
 * @brief       ��������֡����
 * @param       Rx_Data -> ���ڽ��յ�����
 * @return      ��
 **************************************************************************************************
 */
void Tcp_Process(uint8 Rx_Data)
{
  /*-----------------------------------------------------------------------------*/
  /* Command    0      2     1        3             4            5      6        */
  /*-----------------------------------------------------------------------------*/
  /* Name      SOP    CMD   LENG   DOOR_ID   Data[0]_Data[n-1]  LRC    EOP       */
  /*-----------------------------------------------------------------------------*/
  /* Values    0xAA                                                    0x0E      */
  /*-----------------------------------------------------------------------------*/
  /* No.Byte   2byte  1byte 1byte   2byte         nbyte        1byte   1byte     */
  /*-----------------------------------------------------------------------------*/
  switch(Frame.RxProgress)
  {
    /*1.���յ�֡ͷSOP -> 0xAA(1)*/
    case FRAME_RX_SOP_START:          if(Rx_Data != SOP)
                                      {
                                        break;
                                      }
                                      
                                      Frame.RxProgress = FRAME_RX_SOP_CHECK;
                                      break;
                                
                                
    /*2.ȷ�Ͻ��յ���֡ͷSOP -> 0xAA(2)*/ 
    case FRAME_RX_SOP_CHECK:          if(Rx_Data == SOP)
                                      {
                                        Frame.LRC        = 0x00;
                                        Frame.RxProgress = FRAME_RX_LENG;
                                      }
                                      else
                                      {
                                        Frame.RxProgress = FRAME_RX_SOP_START;    //����������¿�ʼ����          
                                      }
                                      break;   
                                      
    /*3.���յ����ݳ��� -> LENG*/                               
    case  FRAME_RX_LENG:              if(Rx_Data >= MIN_LENG)                     //������ڵ�����С����֡����
                                      {
                                        Frame.LENG = Rx_Data;                     //LRCУ�鿪ʼ
                                        Frame.LRC = Frame.LRC ^ Rx_Data;
                                        Frame.RxProgress = FRAME_RX_CMD;
                                        break;                                    //���������ȷ���������ȴ���һ������
                                      } 
                                      else 
                                      {
                                        Frame.RESP = BASE_RESP_LENG_ERR;          //����ȱʧ
                                        Frame.RxStatus = FRAME_RX_ERR;            //һ֡���ݴ���,�������ʼ���������ٶԽ������ݽ��д���  
                                        Frame.RxProgress = FRAME_RX_END;          //������֡���ݵĽ���
                                      }
      
   /*4.���յ������� -> CMD*/   
   case  FRAME_RX_CMD:                if(Frame.RxProgress != FRAME_RX_END)        //�����һ�����մ���,���������
                                      {
                                        if(Rx_Data <= MAX_CMD)                    //�����������
                                        {
                                          Frame.CMD = Rx_Data;
                                          Frame.LRC = Frame.LRC ^ Rx_Data;
                                          Frame.RxProgress = FRAME_RX_DOOR_ID;   
                                          Frame.RxCount = 0;
                                          break;
                                        }
                                        else
                                        {
                                          Frame.CMD =  BASE_CMD_INIT;;
                                          Frame.RESP = BASE_RESP_CMD_ERR;           //����ȱʧ
                                          Frame.RxStatus = FRAME_RX_ERR;            //һ֡���ݴ���,�������ʼ���������ٶԽ������ݽ��д���  
                                          Frame.RxProgress = FRAME_RX_END;          //������֡���ݵĽ���
                                        }
                                      }
                                  
                                      
   /*5.���յ�����ID -> DOOR_ID*/   
   case FRAME_RX_DOOR_ID:             if(Frame.RxProgress != FRAME_RX_END)          //�����һ�����մ���,���������
                                      {
                                        Frame.DOOR_ID[Frame.RxCount++] = Rx_Data;
                                        Frame.LRC = Frame.LRC ^ Rx_Data;
                                        if(Frame.RxCount == 2)
                                        {
                                          Frame.RxCount = 0;
                                          Frame.RxProgress = FRAME_RX_DATA;
                                        } 
                                        
                                        break;
                                      }
                                      
   /*6.�������� -> Data[0]_Data[n-1]*/
   case FRAME_RX_DATA:                if(Frame.RxProgress != FRAME_RX_END)
                                      {
                                        if(Frame.LENG == MIN_LENG)          //û�����ݣ���ֱ�ӽ���LRC������������
                                        {
                                          Frame.RxProgress = FRAME_RX_LRC;
                                          //break;
                                        }
                                        
                                        else                             //�����ݣ���ʱ������
                                        {
                                          //Frame.LENG - MIN_LENG ����й�ϵ
                                          
                                          break;
                                        }
                                        
                                      }
                                      
   /*7.����У�� -> LRC*/              
   case  FRAME_RX_LRC:                if(Frame.RxProgress != FRAME_RX_END)
                                      {
                                        if(Frame.LRC == Rx_Data)
                                        {
                                          Frame.RxProgress = FRAME_RX_EOP;
                                          break;
                                        }       
                                        else
                                        {
                                          Frame.RESP = BASE_RESP_LRC_ERR;
                                          Frame.RxStatus = FRAME_RX_ERR;
                                          Frame.RxProgress = FRAME_RX_END;        //ǿ�н�����������
                                        }
                                       
                                      }
                            
   /*7.����֡β -> EOP*/              
   case  FRAME_RX_EOP:                if(Frame.RxProgress != FRAME_RX_END)
                                      {
                                        if(Rx_Data == EOP)
                                        {
                                          Frame.RxStatus = FRAME_RX_OK;           //һ֡������ȷ 
                                          osal_set_event(0x07,0x04);              //(BaseApp_TaskID,BASEAPP_TCP_RECEIVE_TRUE_MSG_EVENT)
                                          break;
                                        }
                                        else
                                        {
                                          Frame.RESP = BASE_RESP_EOP_ERR;
                                          Frame.RxStatus = FRAME_RX_ERR;
                                          Frame.RxProgress = FRAME_RX_END;        //ǿ�н�����������
                                        }     
                                       
                                      }                                   
                                      
                                      

   default:                           /* ���յ����������֡ */
                                      if((Frame.RxStatus == FRAME_RX_ERR) && (Frame.RxProgress == FRAME_RX_END))
                                      {
                                        Tcp_Send(Frame.CMD,Frame.DOOR_ID,Frame.RESP,Frame.DATA,0); //������������֡���
                                        Tcp_Init();
                                        //osal_set_event(0x07,0x02);  //(BaseApp_TaskID,BASEAPP_TCP_RECEIVE_ERR_MSG_EVENT)
                                      }
                                      break;  
    
  }
}  
  
  
  


/**************************************************************************************************
 * @fn          Tcp_Send
 * @brief       ���ͷ������ݸ���λ��
 * @param       FPB_CMD -> ��������
                DoorId  -> ���������Ŀ������
                RESP    -> ��������
                data    -> ����
                DataLength -> ���ݳ���
 * @return      ��
 **************************************************************************************************
 */
void Tcp_Send(uint8 FPB_CMD, uint8* DoorId, uint8 RESP, uint8 *data, uint8 DataLength)
{
    /*---------------------------------------------------------------------------------------------*/
    /* Command    0       1        2         3       4         5               6      7            */
    /*---------------------------------------------------------------------------------------------*/
    /* Name      SOP     LENG    FPB_CMD    DOOR_ID  RESP  Data[0]_Data[n-1]  LRC    EOP           */
    /*---------------------------------------------------------------------------------------------*/
    /* Values    0xAA                                                                0x0E          */
    /*---------------------------------------------------------------------------------------------*/
    /* No.Byte   2byte   1byte   1byte       2byte   1byte    nbyte    byte   1byte  1-byte        */
    /*---------------------------------------------------------------------------------------------*/
  
    uint8 i;
    uint8 LRC;
    uint8 TxCount = 0;
    uint8 frame[Tx_BUFF] = {0x00};
    
    /*SOP*/
    frame[TxCount++] = SOP;
    frame[TxCount++] = SOP;
    
    /*LENG*/
    frame[TxCount++] = DataLength + FPB_MIN_LENG;   //FPB_CMD ~ Data[n-1]
    
    
    /*FPB_CMD*/
    frame[TxCount++] = FPB_CMD;
   
    
    /*DOOR_ID*/
    for(i=0; i<2; i++)        //DOOR_ID
    {
      frame[TxCount++] = DoorId[i];
    }
    
    /*RESP*/
    frame[TxCount++] = RESP;
    
    /*Data[0]_Data[n-1]*/ 
    for(i=0; i<DataLength;i++)
    {
      frame[TxCount++] = data[i];
    }  
    
    
    /*LRC*/
    LRC = frame[2];     //LENG ~ Data[n-1]
    
    
    for(i=3;i<TxCount; i++)
    {
      LRC = LRC ^ frame[i];
    }
    
    frame[TxCount++] = LRC;
    
    /*EOP*/
    frame[TxCount++] = EOP;
    
    Uart0_SendString(frame,TxCount);
    
}



