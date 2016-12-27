/**************************************************************************************************
  Filename:       BaseApp.c
  Date:           2016-12-13 $
  Revision:       1.0
  Description:    ����Ӧ�ó���
  Author:         zhuxiankang
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

#include <stdio.h>

#include "dri_delay.h"
#include "dri_uart.h"
#include "dri_iport.h"
#include "app_tcp.h"


/*********************************************************************
 * ȫ�ֱ���
 */

/*����������б������б�*/
const cId_t BaseApp_ClusterList[BASEAPP_MAX_CLUSTERS] = 
{
  BROADCAST_TEST_ID,      //Э�����㲥��������
  SINGLE_TEST_ID,         //�ն˵������ݲ���,����1
  SINGLE_TEST_ID_2,       //�ն˵������ݲ���,����2    
  SINGLE_TEST_ID_3        //Э�����������ݸ���������
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

afAddrType_t BaseApp_BroadCast_DstAddr; //�������ݵĹ㲥Ŀ�ĵ�ַ
afAddrType_t BaseApp_Single_DstAddr;    //�������ݵĵ���Ŀ�ĵ�ַ

aps_Group_t BaseApp_Group;              //����Ϣ����

uint8 BaseAppPeriodicCounter = 0;       //���ʹ���ͳ��
uint8 BaseAppFlashCounter = 0;



/*********************************************************************
 * ������
 */

typedef struct
{
  uint8 extAddr[8];   //MAC��ַ
  uint16 doorId;      //����ID
  
  
  
  
} associate_t;

associate_t associateList;      



/*********************************************************************
 * ���غ���
 */
void BaseApp_HandleKeys( uint8 shift, uint8 keys ); 
void BaseApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void BaseApp_SendMessage(afAddrType_t *DstAddr, uint8 *SendData, uint8 len, uint8 Cmd);  //��վ��������������ĺ���



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
  
  /*Drivers������ʼ��*/
  Uart0_Init();                     //uart0��ʼ��
  Iport_Init();                     //iport��ʼ��
  Tcp_Init();                       //uart���ܱ�����ʼ��
  
  /*��������*/  
  BaseApp_Single_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;        //3:����
  BaseApp_Single_DstAddr.endPoint = BASEAPP_ENDPOINT;               //�˵�ţ�20
  //BaseApp_Periodic_DstAddr.addr.shortAddr = 0x0000;               //Э������ַ
  
  /*�㲥����*/
  BaseApp_BroadCast_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;  //15:�㲥
  BaseApp_BroadCast_DstAddr.endPoint = BASEAPP_ENDPOINT;             //�˵�ţ�20
  BaseApp_BroadCast_DstAddr.addr.shortAddr = BROADCAST_ADDR;         //�㲥��ַ 
  
 
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


//uint8 sendNum = 0;

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

  /*1. ϵͳ��Ϣ�¼�*/
  if ( events & SYS_EVENT_MSG )     
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
          
          if (BaseApp_NwkState == DEV_ZB_COORD)
              //|| (BaseApp_NwkState == DEV_ROUTER)
              //|| (BaseApp_NwkState == DEV_END_DEVICE) )
          {           
            //HAL_TOGGLE_LED1();   
            //HalUARTWrite(0,"State Change OK\n", sizeof("State Change OK\n"));
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

  /*2. ��ʱ�¼�*/
  if ( events & BASEAPP_OFF_LINE_TASK_MSG_EVENT )
  {
//    uint8 data[2] = {0x44,0x45};
//    uint8 data2[2] = {0x44,0x46};
//    
//    
//    //�豸1������
//    if(AssociatedDevList[0].shortAddr != 0xFFFF) 
//    {
//      BaseApp_Single_DstAddr.addr.shortAddr = AssociatedDevList[0].shortAddr;
//      BaseApp_SendMessage(&BaseApp_Single_DstAddr,data,2,SINGLE_TEST_ID_3);
//    } 
//   
//    if(AssociatedDevList[1].shortAddr != 0xFFFF)
//    {
//      BaseApp_Single_DstAddr.addr.shortAddr = AssociatedDevList[1].shortAddr;
//      BaseApp_SendMessage(&BaseApp_Single_DstAddr,data2,2,SINGLE_TEST_ID_3);
//    }
    
  
    osal_start_timerEx( BaseApp_TaskID, BASEAPP_OFF_LINE_TASK_MSG_EVENT,  
        (BASEAPP_OFF_LINE_TASK_MSG_TIMEOUT + (osal_rand() & 0x00FF)) );


    return (events ^ BASEAPP_OFF_LINE_TASK_MSG_EVENT);  //����δ������¼�
  }
  
  
  /*3. ������λ������֡�����¼�*/
  if ( events & BASEAPP_TCP_RECEIVE_ERR_MSG_EVENT )
  {
    
    //HAL_TURN_ON_LED2();
    
    return (events ^ BASEAPP_TCP_RECEIVE_ERR_MSG_EVENT);  //����δ������¼�
  }
  
  /*4. ������λ�������¼�*/
  if ( events & BASEAPP_TCP_RECEIVE_TRUE_MSG_EVENT )
  {
    uint8 data[Rx_BUFF] = {0x00};
    uint8 cmd = Frame.CMD;
    uint8 doorId[2] = {Frame.DOOR_ID[0],Frame.DOOR_ID[1]};
    uint8 i=0;
    
    if(Frame.LENG - MIN_LENG)
    {
      //memset(data,Frame.DATA,Frame.LENG - MIN_LENG);
      strncpy(data,Frame.DATA,Frame.LENG - MIN_LENG);
    }
    
    Tcp_Init();             //���ý���ʹ���ڴ������Ҳ�����ȴ��������ٽ���ʹ�ܣ�
    
    switch(cmd) 
    {
      
      /*1. ��ȡ��վ�Ĺ����б� */
      case BASE_CMD_GET_ASSOCLIST:  
        
        for(i=0;i<NWK_MAX_DEVICES;i++) 
        {
          if(AssociatedDevList[i].shortAddr != 0xFFFF) { //Ҳ���ܲ���FFFF��������Ҫע��
            data[i] = AssociatedDevList[i].shortAddr & 0xFF;
            data[i+1] = (AssociatedDevList[i].shortAddr >> 8) & 0xFF;
          } 
          else
          {
            break;    //������ھ�����ѭ��
          }
        }
        
        
        break;
          
      default:
        break;
    }
    
    
    
    HAL_TOGGLE_LED1();      //��תLED��ָʾ
    

    return (events ^ BASEAPP_TCP_RECEIVE_TRUE_MSG_EVENT);  //����δ������¼�
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


  //�жϽ��յ��Ĵ�ID
  switch ( pkt->clusterId )
  {
    //�㲥��������,����1
    case BROADCAST_TEST_ID: 
      break;
      
    //������������,����1
    case SINGLE_TEST_ID:
      break;
      
    //������������,����2
    case SINGLE_TEST_ID_2:
      break;
  }
}





/*********************************************************************
*********************************************************************/

/*********************************
  Funcname:       BaseApp_ProcessEvent
  Description:    ����RF��Ƶ���ݺ���
  Author:         zhuxiankang
  parm:           SendData -> ���͵����� 
                  Cmd      -> �������ͣ�����Ҳ����˵��/��ID/Ⱥ��ID��
*********************************/
void BaseApp_SendMessage(afAddrType_t *DstAddr, uint8 *SendData, uint8 len, uint8 Cmd)
{
  
  if ( AF_DataRequest( //&BaseApp_Periodic_DstAddr,   //Ŀ�Ľڵ�ĵ�ַ��Ϣ
                       DstAddr,
                       &BaseApp_epDesc,               //�˵���Ϣ
                       Cmd,                           //��������
                       len,                           //���ݳ���
                       SendData,                      //����
                       &BaseApp_TransID,              //������ϢID
                       AF_DISCV_ROUTE,                //����ѡ��
                       AF_DEFAULT_RADIUS )            //��������뾶
      == afStatus_SUCCESS )    
  {
    HAL_TOGGLE_LED1();
  }
  else
  {
    
  }
}