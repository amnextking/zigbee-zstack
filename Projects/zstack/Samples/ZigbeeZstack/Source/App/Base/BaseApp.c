/**************************************************************************************************
  Filename:       BaseApp.c
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

#include "BaseApp.h"
//#include "BaseAppHw.h"

#include "OnBoard.h"


/* Ӳ�������HAL */   
#include "hal_led.h"    
#include "hal_key.h"
#include "MT_UART.h"
#include "MT_APP.h"
#include "MT.h"


/*********************************************************************
 * ȫ�ֱ���
 */

/*����������б������б�*/
const cId_t BaseApp_ClusterList[BASEAPP_MAX_CLUSTERS] = 
{
  BASEAPP_PERIODIC_CLUSTERID,    //�㲥�Ĵ�:0x01
  BASEAPP_FLASH_CLUSTERID        //�鲥�Ĵ�:0x02
};

/*Zigbee�򵥶˵�������*/
const SimpleDescriptionFormat_t BaseApp_SimpleDesc = 
{
  BASEAPP_ENDPOINT,              //�˵��:20
  BASEAPP_PROFID,                //�˵�Ĵ�ID:0x0F08
  BASEAPP_DEVICEID,              //�豸ID:0x0001
  BASEAPP_DEVICE_VERSION,        //�豸�������汾:0
  BASEAPP_FLAGS,                 //����
  BASEAPP_MAX_CLUSTERS,          //�˵�֧�ֵ�����ظ���:2
  (cId_t *)BaseApp_ClusterList,  //������б�
  BASEAPP_MAX_CLUSTERS,          //��֧�ֵ�����ظ���:2 
  (cId_t *)BaseApp_ClusterList   //������б�����غ������Ҫһһ��Ӧ��
};

/*�˵���Ϣ�ṹ��*/
endPointDesc_t BaseApp_epDesc;


/*********************************************************************
 * ���ر���
 */

uint8 BaseApp_TaskID;                   // ����ID 
devStates_t BaseApp_NwkState;           //����״̬

uint8 BaseApp_TransID;                  // ��������ID

afAddrType_t BaseApp_Periodic_DstAddr;  //�������ݵĹ㲥Ŀ�ĵ�ַ
afAddrType_t BaseApp_Flash_DstAddr;     //�������ݵ��鲥Ŀ�ĵ�ַ

aps_Group_t BaseApp_Group;              //����Ϣ����

uint8 BaseAppPeriodicCounter = 0;       //���ʹ���ͳ��
uint8 BaseAppFlashCounter = 0;

/*********************************************************************
 * ���غ���
 */
void BaseApp_HandleKeys( uint8 shift, uint8 keys ); 
void BaseApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void BaseApp_SendPeriodicMessage( void );
void BaseApp_SendFlashMessage( uint16 flashTime );


/*********************************************************************
 * ��������
 */

/*********************************
  Funcname:       BaseApp_Init
  Description:    �����ʼ������
  Author:         zhuxiankang
  parm:           task_id -> �������к�
*********************************/

void BaseApp_Init( uint8 task_id )
{ 
  /*������ʼ��*/
  BaseApp_TaskID = task_id;         //����ID
  BaseApp_NwkState = DEV_INIT;      //����״̬��ʼ��ΪDEV_INIT
  BaseApp_TransID = 0;              //��������ID  
  

  /*��������*/  
  BaseApp_Periodic_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;      //15:�㲥
  BaseApp_Periodic_DstAddr.endPoint = BASEAPP_ENDPOINT;             //�˵�ţ�20
  BaseApp_Periodic_DstAddr.addr.shortAddr = 0x0000;                 //Э������ַ
  
 
  /*���屾�豸����ͨ�ŵ�APS��˵�������*/
  BaseApp_epDesc.endPoint = BASEAPP_ENDPOINT;                     //�˵�ţ�20
  BaseApp_epDesc.task_id = &BaseApp_TaskID;                       //����ID 
  BaseApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&BaseApp_SimpleDesc;   //Zigbee�򵥶˵�������
  BaseApp_epDesc.latencyReq = noLatencyReqs;                      //����ʱ����
  
  /*��AF��ע��˵�20*/
  afRegister( &BaseApp_epDesc );

  /*ע�ᰴ���¼�*/
  //RegisterForKeys( BaseApp_TaskID );
  
  /*��վ��ʱ����*/
  osal_start_timerEx( BaseApp_TaskID,BASEAPP_OFF_LINE_TASK_MSG_EVENT,BASEAPP_OFF_LINE_TASK_MSG_TIMEOUT );
  
}






/*********************************
  Funcname:       BaseApp_ProcessEvent
  Description:    �����¼�������
  Author:         zhuxiankang
  parm:           task_id -> �������к� evnets -> �¼�������־
*********************************/
uint16 BaseApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;     //������Ϣ�ṹ��
  (void)task_id;  

  if ( events & SYS_EVENT_MSG )      //���յ�ϵͳ��Ϣ
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( BaseApp_TaskID ); //����ϵͳ��Ϣ��������Ϣ��
    
    while ( MSGpkt )
    {
      
      switch ( MSGpkt->hdr.event )  
      {
        /*��Ϣ������Ӧ�¼�*/
        case AF_INCOMING_MSG_CMD:
          BaseApp_MessageMSGCB( MSGpkt );   //���ûص��������յ������ݽ��д���
          break;

        /*�豸����״̬�仯�¼��������ն��豸����*/
        case ZDO_STATE_CHANGE:
          BaseApp_NwkState = (devStates_t)(MSGpkt->hdr.status);//��ȡ�豸��ǰ״̬
          
          if ( (BaseApp_NwkState == DEV_ZB_COORD)
              || (BaseApp_NwkState == DEV_ROUTER)
              || (BaseApp_NwkState == DEV_END_DEVICE) )
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
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( BaseApp_TaskID ); //���б��м�����һ����Ҫ�������Ϣ�¼�
    }

    return (events ^ SYS_EVENT_MSG);                  //����δ������¼�
  }

  /*��ʱ�¼�*/
  if ( events & BASEAPP_OFF_LINE_TASK_MSG_EVENT )
  {

//    HAL_TOGGLE_LED1();
//    HAL_TOGGLE_LED2();
//    HAL_TOGGLE_LED3();
    
    
    osal_start_timerEx( BaseApp_TaskID, BASEAPP_OFF_LINE_TASK_MSG_EVENT,  
        (BASEAPP_OFF_LINE_TASK_MSG_TIMEOUT + (osal_rand() & 0x00FF)) );


    return (events ^ BASEAPP_OFF_LINE_TASK_MSG_EVENT);  //����δ������¼�
  }

  return 0;
}

/*********************************************************************
 * ��ͨ�¼�����
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      BaseApp_MessageMSGCB
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
void BaseApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  uint16 flashTime;
  byte buf[3];

  //�жϽ��յ��Ĵ�ID
  switch ( pkt->clusterId )
  {
    //�յ��㲥����
    case BASEAPP_PERIODIC_CLUSTERID:  

      /*�û����**/
      osal_memset(buf,0,3);                   //�ȳ�ʼ��bufȫΪ0,�������
      osal_memcpy(buf,pkt->cmd.Data,2);
      
      if((buf[0] == '1') && (buf[1] == '1'))  //�жϽ��յ��Ƿ�ΪD1
      {
        HalLedBlink(HAL_LED_2,0,50,500);      //LED2���500ms��˸
        
        #if defined(ZDO_COORDINATOR)
        HalUARTWrite(0,"CoordinatorEB-Pro 57 Receive '11' OK\n", sizeof("CoordinatorEB-Pro 57 Receive 'D1' OK\n"));//���ڷ���
        //HalLedBlink(HAL_LED_1,0,50,500);      //LED1���500ms��˸
        BaseApp_SendPeriodicMessage();      //Э�������յ�D1�󷵻�D1���նˣ����ն�LED1Ҳ��˸
        #endif
      }
      else
      {
        HalLedSet(HAL_LED_2,HAL_LED_MODE_ON);
      }
      break;

    //�յ��鲥����
    case BASEAPP_FLASH_CLUSTERID:
      flashTime = BUILD_UINT16(pkt->cmd.Data[1], pkt->cmd.Data[2] );
      //HalLedBlink( HAL_LED_4, 4, 50, (flashTime / 4) );
      break;
  }
}

/*********************************************************************
 * @fn      BaseApp_SendPeriodicMessage
 *
 * @brief   Send the periodic message.
 *
 * @param   none
 *
 * @return  none
 */

//����������Ϣ
//��BaseApp_ProcessEvent�е�osal_start_timerEx��������5s�Ķ�ʱ��
//5s�Ժ�Ķ�ʱ�¼����ǵ�����BaseApp_SendPeriodicMessage�����������ݵķ���
//����ÿ5s�ᷢ��һ�����ݣ�������������ǹ㲥����
void BaseApp_SendPeriodicMessage( void )
{
  
  byte SendData[3] = "11";
  
  //����AF_DataRequest���������߹㲥��ȥ
  if ( AF_DataRequest( &BaseApp_Periodic_DstAddr,   //Ŀ�Ľڵ�ĵ�ַ��Ϣ
                       &BaseApp_epDesc,             //�˵���Ϣ
                       BASEAPP_PERIODIC_CLUSTERID,  //��ID��Ϣ�������Ƿ��͵Ĵ�ID,����յ��Ĵ�IDһһ��Ӧ
                       2,                             //���ݳ���
                       SendData,                      //����
                       &BaseApp_TransID,            //������ϢID
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
 * @fn      BaseApp_SendFlashMessage
 *
 * @brief   Send the flash message to group 1.
 *
 * @param   flashTime - in milliseconds
 *
 * @return  none
 */
void BaseApp_SendFlashMessage( uint16 flashTime )
{
  uint8 buffer[3];
  buffer[0] = (uint8)(BaseAppFlashCounter++);
  buffer[1] = LO_UINT16( flashTime );
  buffer[2] = HI_UINT16( flashTime );

  if ( AF_DataRequest( &BaseApp_Flash_DstAddr, &BaseApp_epDesc,
                       BASEAPP_FLASH_CLUSTERID,
                       3,
                       buffer,
                       &BaseApp_TransID,
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
