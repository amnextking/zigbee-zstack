/**************************************************************************************************
  Filename:       LockApp.c
  Date:           2016-12-13 $
  Revision:       1.0
  Description:    ����Ӧ�ó���
  Author:         zhuxiankang
**************************************************************************************************/


/**************************************************************************************************
  Description:    �˿�����˵��
  1.LED                     ->              P1_5      ��ͨIO�ڣ�����Ϊ���

  2.KEY                     ->   Կ�׿���   P0_1      ��ͨIO�ڣ�����Ϊ���������룬�½��ش����ж� 
                                 ����       P0_4      ��ͨIO�ڣ�����Ϊ���������룬�½��ش����ж�
                                 �ŷ���     P0_5      ��ͨIO�ڣ�����Ϊ����������

  3.BUZZER                  ->   ������     P2_0      ��ͨIO�ڣ�����Ϊ�����������Ĭ�ϸߵ�ƽ   

  4.MOTOR                   ->   H������    P0_6 P0_7 ��ͨIO�ڣ�����Ϊ������͵�ƽ��Ч
                                 H������    P1_0 P1_1 ��ͨIO�ڣ�����Ϊ������ߵ�ƽ��Ч

  5.I2C                     ->   SCL        P1_3      ��ͨIO�ڣ�����Ϊ���
                                 SDA        P1_2      ��ͨIO�ڣ��������Ҳ�������
 
  6.MFRC522                 ->   CS(SPI)    P1_2      ��ͨIO�ڣ�����Ϊ���
                                 SCK(SPI)   P1_3      ��ͨIO�ڣ�����Ϊ���
                                 MOSI(SPI)  P0_2      ��ͨIO�ڣ�����Ϊ���
                                 MISO(SPI)  P0_3      ��ͨIO�ڣ�����Ϊ����
                                 RST        P1_7      ��ͨIO�ڣ�����Ϊ���


**************************************************************************************************/




#include "OSAL.h"
#include "ZGlobals.h"
#include "AF.h"
#include "aps_groups.h"
#include "ZDApp.h"

#include "LockApp.h"
//#include "LockAppHw.h"

#include "OnBoard.h"


/* Ӳ�������HAL */   
#include "hal_led.h"    
#include "hal_key.h"
#include "MT_UART.h"
#include "MT_APP.h"
#include "MT.h"

/* ������ */
#include "dri_key.h"


/* �豸�� */
#include "dev_mfrc522.h"


/* Ӧ�ò� */
#include "appl_data.h"    //EEPROM�洢Ӧ��
#include "apph_card.h"    //RFIDˢ������
#include "apph_door.h"    //����������ʾӦ��


/*********************************************************************
 * ȫ�ֱ���
 */

/*����������б������б�*/
const cId_t LockApp_ClusterList[LOCKAPP_MAX_CLUSTERS] = 
{
  LOCKAPP_PERIODIC_CLUSTERID,    //�㲥�Ĵ�:0x01
  LOCKAPP_FLASH_CLUSTERID        //�鲥�Ĵ�:0x02
};

/*Zigbee�򵥶˵�������*/
const SimpleDescriptionFormat_t LockApp_SimpleDesc = 
{
  LOCKAPP_ENDPOINT,              //�˵��:20
  LOCKAPP_PROFID,                //�˵�Ĵ�ID:0x0F08
  LOCKAPP_DEVICEID,              //�豸ID:0x0001
  LOCKAPP_DEVICE_VERSION,        //�豸�������汾:0
  LOCKAPP_FLAGS,                 //����
  LOCKAPP_MAX_CLUSTERS,          //�˵�֧�ֵ�����ظ���:2
  (cId_t *)LockApp_ClusterList,  //������б�
  LOCKAPP_MAX_CLUSTERS,          //��֧�ֵ�����ظ���:2 
  (cId_t *)LockApp_ClusterList   //������б�����غ������Ҫһһ��Ӧ��
};

/*�˵���Ϣ�ṹ��*/
endPointDesc_t LockApp_epDesc;


/*********************************************************************
 * ���ر���
 */

uint8 LockApp_TaskID;                   // ����ID 
devStates_t LockApp_NwkState;           //����״̬

uint8 LockApp_TransID;                  // ��������ID

afAddrType_t LockApp_Periodic_DstAddr;  //�������ݵĹ㲥Ŀ�ĵ�ַ
afAddrType_t LockApp_Flash_DstAddr;     //�������ݵ��鲥Ŀ�ĵ�ַ

aps_Group_t LockApp_Group;              //����Ϣ����

uint8 LockAppPeriodicCounter = 0;       //���ʹ���ͳ��
uint8 LockAppFlashCounter = 0;

/*********************************************************************
 * ���غ���
 */
void LockApp_HandleKeys( uint8 shift, uint8 keys ); 
void LockApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void LockApp_SendPeriodicMessage( void );
void LockApp_SendFlashMessage( uint16 flashTime );


/*********************************************************************
 * ��������
 */

/*********************************
  Funcname:       LockApp_Init
  Description:    �����ʼ������
  Author:         zhuxiankang
  parm:           task_id -> �������к�
*********************************/

void LockApp_Init( uint8 task_id )
{ 
  /*������ʼ��*/
  LockApp_TaskID = task_id;         //����ID
  LockApp_NwkState = DEV_INIT;      //����״̬��ʼ��ΪDEV_INIT
  LockApp_TransID = 0;              //��������ID  
  
  /*Drivers������ʼ��*/
  Key_Init();                       //���ۣ�������Կ�׿��ų�ʼ��
  Buzzer_Init();                    //P2_0 ������������ʼ��
  Buzzer_Timer4_Init();             //��ʱ��4��ʼ��(������Z-STACK������)
  Motor_Init();                     //P0_6 P0_7 P1_0 P1_1 ���������ʼ��
  I2C_Init();                       //P1_2 P1_3 I2C��ʼ��
  Spi_Init();                       //P1.2��P1.3��P1.7��P0.2��P0.3 �������˿ڳ�ʼ��    
  
  /*Devices�豸��ʼ��*/
  MFRC522_Init();                   //MFRC522����ģ���ʼ��
  //Card_Init();
  
  
  
  /*AppsHӦ�ó�ʼ��*/
  //Data_DoorID_Init();               //����ID��Ϣ��ʼ����������
  Data_CommonCard_Init();           //��ͨ���б��ʼ���������ã�������Զ�̿��ƣ���д��ʱ��ֻ��Ҫһ�� 
 
  
  Buzzer_System_Start();               //ϵͳӦ��������ʾ����
  Door_Close(LedOff);
  
  
  
  /*��������*/  
  LockApp_Periodic_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;      //15:�㲥
  LockApp_Periodic_DstAddr.endPoint = LOCKAPP_ENDPOINT;             //�˵�ţ�20
  LockApp_Periodic_DstAddr.addr.shortAddr = 0x0000;                 //Э������ַ
  
 
  /*���屾�豸����ͨ�ŵ�APS��˵�������*/
  LockApp_epDesc.endPoint = LOCKAPP_ENDPOINT;                     //�˵�ţ�20
  LockApp_epDesc.task_id = &LockApp_TaskID;                       //����ID 
  LockApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&LockApp_SimpleDesc;   //Zigbee�򵥶˵�������
  LockApp_epDesc.latencyReq = noLatencyReqs;                      //����ʱ����
  
  /*��AF��ע��˵�20*/
  afRegister( &LockApp_epDesc );

  /*ע�ᰴ���¼�*/
  RegisterForKeys( LockApp_TaskID );
  
  /*������������ʱ1s*/
  osal_start_timerEx( LockApp_TaskID,LOCKAPP_OFF_LINE_TASK_MSG_EVENT,LOCKAPP_OFF_LINE_TASK_MSG_TIMEOUT );
  
}


//��ʱ�������棬���Է���
uint8 Status = MFRC522_ERR;     //ˢ�����
uint8 BlockData[16] = {0x00};   //M1���������� 
uint8 DoorId[4] = {0x00};       //����ID



/*********************************
  Funcname:       LockApp_ProcessEvent
  Description:    �����¼�������
  Author:         zhuxiankang
  parm:           task_id -> �������к� evnets -> �¼�������־
*********************************/
uint16 LockApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;     //������Ϣ�ṹ��
  (void)task_id;  

  if ( events & SYS_EVENT_MSG )      //���յ�ϵͳ��Ϣ
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( LockApp_TaskID ); //����ϵͳ��Ϣ��������Ϣ��
    
    while ( MSGpkt )
    {
      
      switch ( MSGpkt->hdr.event )  
      {
        /*���������¼�*/
        case KEY_CHANGE:
          LockApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
          break; 

        /*��Ϣ������Ӧ�¼�*/
        case AF_INCOMING_MSG_CMD:
          LockApp_MessageMSGCB( MSGpkt );   //���ûص��������յ������ݽ��д���
          break;

        /*�豸����״̬�仯�¼��������ն��豸����*/
        case ZDO_STATE_CHANGE:
          LockApp_NwkState = (devStates_t)(MSGpkt->hdr.status);//��ȡ�豸��ǰ״̬
          
          if ( (LockApp_NwkState == DEV_ZB_COORD)
              || (LockApp_NwkState == DEV_ROUTER)
              || (LockApp_NwkState == DEV_END_DEVICE) )
          {
            //�豸�����ɹ�
          }
          else
          {
            //�豸�뿪����
          }
          break;

        default:
          break;
      }

      osal_msg_deallocate( (uint8 *)MSGpkt );         //�ͷ���Ϣռ�õ��ڴ�
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( LockApp_TaskID ); //���б��м�����һ����Ҫ�������Ϣ�¼�
    }

    return (events ^ SYS_EVENT_MSG);                  //����δ������¼�
  }

  /*��ʱ�¼�*/
  if ( events & LOCKAPP_OFF_LINE_TASK_MSG_EVENT )
  {
    
    /*1.ˢ����Ȩ�Ϳ���*/
    Status = Card_ReadBlock(CARD_INFORMATION,BlockData); 
    
    /*1.1 ��Ȩ������*/
    if(Status == MFRC522_OK)        
    {
      /*1.1.1 ��δ����*/
      if(LOCK_PORT == LOCK_NO)      
      {
        /*1.1.1.1 ����ID��Ϣ��ȡ*/
        Data_DoorID_Read(DoorId); 
        
        /*1.1.1.2 �˹��ⷿ��ײ¥����Ȩ��*/
        if((BlockData[2] == DoorId[0]) && (BlockData[3] == DoorId[1]))    
        {
          
          /*1.1.1.2.1 ��Ȩ����ɾȨ��*/
          if(((BlockData[0] == Authorization)    && (BlockData[1] == Authorization)) ||
            ((BlockData[0] == UnAuthorizataion) && (BlockData[1] == UnAuthorizataion)))     
          {
             Door_Open(LedOn);
             LED_OFF();
             Delay_Ms(1000);
             Buzzer_One_Led(LedOn);
             Card_Authorization(BlockData[0],DoorId);      //������Ȩ��ɾȨ����
             Door_Close(LedOff);  
          }
          
          /*1.1.1.2.2 �ܿ�*/
          else if((BlockData[0] == TotalCard) && (BlockData[1] == TotalCard))   
          {
            Door_Open_Close();
          }
        }
        
        /*1.1.1.3 ���Զ�ȡ������Ϣ����ͨ������*/
        else 
        {
          if(Data_CommonCard_Confirm(BlockData+12)) 
          {
            Door_Open_Close();   
          }
          
          else
          {
            Buzzer_Card_Fail();   //��Ȩ�޿�����
          }
        }
      }
      
      /*1.1.2 �ŷ���*/
      else 
      {
        Buzzer_Door_Lock();       //�ŷ�������Ҫ��ʾ��   
      }
      
      
      /*1.1.3 ˢ�������*/
      //��ʱ����
      
      
      
    }
    
    /*1.2 ���ܶ�ȡ������Ϣ����ͨ������(��Ҫע���������Ȩģʽ����ͨ������Զ�ȡ�������������������Զ����Ȩ�Ĳ���)*/
    else 
    {
      
      //ʹ��Card_ReadID������ȡ����ID����
      
      
      
    }
    
    
    
   
    osal_start_timerEx( LockApp_TaskID, LOCKAPP_OFF_LINE_TASK_MSG_EVENT,  
        (LOCKAPP_OFF_LINE_TASK_MSG_TIMEOUT + (osal_rand() & 0x00FF)) );


    return (events ^ LOCKAPP_OFF_LINE_TASK_MSG_EVENT);  //����δ������¼�
  }

  return 0;
}

/*********************************************************************
 * ��ͨ�¼�����
 */

/*********************************
  Funcname:       LockApp_HandleKeys
  Description:    ���������¼�
  Author:         zhuxiankang
  parm:           keys -> ����
*********************************/

void LockApp_HandleKeys( uint8 shift, uint8 keys )
{
  (void)shift;  // Intentionally unreferenced parameter
  
  /* Կ�׿��� */
  if ( keys & HAL_KEY_SW_6 ) {
    Buzzer_Key_Open();
  }
  
  /* ���ۿ��� */
  if ( keys & HAL_KEY_SW_7 ) {
    //IsDoorClose =  CLOSE_YES;
  }
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      LockApp_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */

//�������ݺ���
void LockApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  uint16 flashTime;
  byte buf[3];

  //�жϽ��յ��Ĵ�ID
  switch ( pkt->clusterId )
  {
    //�յ��㲥����
    case LOCKAPP_PERIODIC_CLUSTERID:  

      /*�û����**/
      osal_memset(buf,0,3);                   //�ȳ�ʼ��bufȫΪ0,�������
      osal_memcpy(buf,pkt->cmd.Data,2);
      
      if((buf[0] == '1') && (buf[1] == '1'))  //�жϽ��յ��Ƿ�ΪD1
      {
        //HalLedBlink(HAL_LED_2,0,50,500);      //LED2���500ms��˸
        
        #if defined(ZDO_COORDINATOR)
        HalUARTWrite(0,"CoordinatorEB-Pro 57 Receive '11' OK\n", sizeof("CoordinatorEB-Pro 57 Receive 'D1' OK\n"));//���ڷ���
        //HalLedBlink(HAL_LED_1,0,50,500);      //LED1���500ms��˸
        LockApp_SendPeriodicMessage();      //Э�������յ�D1�󷵻�D1���նˣ����ն�LED1Ҳ��˸
        #endif
      }
      else
      {
        //HalLedSet(HAL_LED_2,HAL_LED_MODE_ON);
      }
      break;

    //�յ��鲥����
    case LOCKAPP_FLASH_CLUSTERID:
      flashTime = BUILD_UINT16(pkt->cmd.Data[1], pkt->cmd.Data[2] );
      //HalLedBlink( HAL_LED_4, 4, 50, (flashTime / 4) );
      break;
  }
}

/*********************************************************************
 * @fn      LockApp_SendPeriodicMessage
 *
 * @brief   Send the periodic message.
 *
 * @param   none
 *
 * @return  none
 */

//����������Ϣ
//��LockApp_ProcessEvent�е�osal_start_timerEx��������5s�Ķ�ʱ��
//5s�Ժ�Ķ�ʱ�¼����ǵ�����LockApp_SendPeriodicMessage�����������ݵķ���
//����ÿ5s�ᷢ��һ�����ݣ�������������ǹ㲥����
void LockApp_SendPeriodicMessage( void )
{
  
  byte SendData[3] = "11";
  
  //����AF_DataRequest���������߹㲥��ȥ
  if ( AF_DataRequest( &LockApp_Periodic_DstAddr,   //Ŀ�Ľڵ�ĵ�ַ��Ϣ
                       &LockApp_epDesc,             //�˵���Ϣ
                       LOCKAPP_PERIODIC_CLUSTERID,  //��ID��Ϣ�������Ƿ��͵Ĵ�ID,����յ��Ĵ�IDһһ��Ӧ
                       2,                             //���ݳ���
                       SendData,                      //����
                       &LockApp_TransID,            //������ϢID
                       AF_DISCV_ROUTE,                //����ѡ��
                       AF_DEFAULT_RADIUS )            //��������뾶
      == afStatus_SUCCESS )    
  {
  }
  else
  {
    //HalLedSet(HAL_LED_1,HAL_LED_MODE_ON);
    // Error occurred in request to send.
  }
}

/*********************************************************************
 * @fn      LockApp_SendFlashMessage
 *
 * @brief   Send the flash message to group 1.
 *
 * @param   flashTime - in milliseconds
 *
 * @return  none
 */
void LockApp_SendFlashMessage( uint16 flashTime )
{
  uint8 buffer[3];
  buffer[0] = (uint8)(LockAppFlashCounter++);
  buffer[1] = LO_UINT16( flashTime );
  buffer[2] = HI_UINT16( flashTime );

  if ( AF_DataRequest( &LockApp_Flash_DstAddr, &LockApp_epDesc,
                       LOCKAPP_FLASH_CLUSTERID,
                       3,
                       buffer,
                       &LockApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/*********************************************************************
*********************************************************************/
