/**************************************************************************************************
  Filename:       LockApp.c
  Date:           2016-12-13 $
  Revision:       1.0
  Description:    ��վӦ�ó���
  Author:         zhuxiankang
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "ZGlobals.h"
#include "AF.h"
#include "aps_groups.h"
#include "ZDApp.h"

#include "LockApp.h"
//#include "LockAppHw.h"    �����ʱ����

#include "OnBoard.h"


/* HAL */
#include "hal_lcd.h" //������Ӳ��������ͷ�ļ�
#include "hal_led.h"
#include "hal_key.h"
#include "MT_UART.h"
#include "MT_APP.h"
#include "MT.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// This list should be filled with Application specific Cluster IDs.
const cId_t LockApp_ClusterList[LOCKAPP_MAX_CLUSTERS] = //����������б�
{
  LOCKAPP_PERIODIC_CLUSTERID,    //�㲥�Ĵ�:0x01
  LOCKAPP_FLASH_CLUSTERID        //�鲥�Ĵ�:0x02
};

const SimpleDescriptionFormat_t LockApp_SimpleDesc = //Zigbee�򵥶˵�����������
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

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in LockApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t LockApp_epDesc;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

uint8 LockApp_TaskID;   // ����ID Task ID for internal task/event processing
                          // This variable will be received when
                          // LockApp_Init() is called.
devStates_t LockApp_NwkState;   //����״̬

uint8 LockApp_TransID;  // ��������ID This is the unique message ID (counter)

afAddrType_t LockApp_Periodic_DstAddr;  //�������ݵĹ㲥Ŀ�ĵ�ַ
afAddrType_t LockApp_Flash_DstAddr;     //�������ݵ��鲥Ŀ�ĵ�ַ

aps_Group_t LockApp_Group;              //����Ϣ����

uint8 LockAppPeriodicCounter = 0;       //���ʹ���ͳ��
uint8 LockAppFlashCounter = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
void LockApp_HandleKeys( uint8 shift, uint8 keys ); //��������
void LockApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void LockApp_SendPeriodicMessage( void );
void LockApp_SendFlashMessage( uint16 flashTime );

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      LockApp_Init
 *
 * @brief   Initialization function for the Generic App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */



/*�����ʼ����������������������������������������*/

void LockApp_Init( uint8 task_id )
{ 
  LockApp_TaskID = task_id;         //osal���������ID�������û���������������ı�
  LockApp_NwkState = DEV_INIT;      //����״̬��ʼ��ΪDEV_INIT
  /*��ʼ��Ӧ���豸���������ͣ��豸���͵ĸı䶼Ҫ����һ���¼���ZDO_STATE_CHANGE�����������Ϊ
//ZDO״̬�����˸ı䡣�������豸��ʼ����ʱ��һ��Ҫ������ʼ��Ϊʲô״̬��û�С���ô����Ҫȥ���
//�������������Ƿ������½������߼�����ڵ����硣������һ��������⣬���ǵ�NV_RESTORE�����õ�
//ʱ��NV_RESTORE�ǰ���Ϣ�����ڷ���ʧ�洢���У�����ô���豸�ϵ����ĳ����������ʱ����������
//״̬�洢�ڷ���ʧ�洢���У���ô��ʱ��ֻ��Ҫ�ָ�������״̬��������Ҫ���½������߼���������*/
  LockApp_TransID = 0;              //�������кų�ʼ��0,��Ϣ����ID������Ϣʱ��˳��֮�֣�
  
  //------------------------���ô���---------------------------------
  MT_UartInit();                    //���ڳ�ʼ��
  MT_UartRegisterTaskID(task_id);   //ע�ᴮ������
  HalUARTWrite(0,"UartInit OK\n", sizeof("UartInit OK\n"));
  //-----------------------------------------------------------------
  
  // Device hardware initialization can be added here or in main() (Zmain.c).
  // If the hardware is application specific - add it here.
  // If the hardware is other parts of the device add it in main().

 #if defined ( BUILD_ALL_DEVICES )
  // The "Demo" target is setup to have BUILD_ALL_DEVICES and HOLD_AUTO_START
  // We are looking at a jumper (defined in LockAppHw.c) to be jumpered
  // together - if they are - we will start up a coordinator. Otherwise,
  // the device will start as a router.
  if ( readCoordinatorJumper() )
    zgDeviceLogicalType = ZG_DEVICETYPE_COORDINATOR;
  else
    zgDeviceLogicalType = ZG_DEVICETYPE_ROUTER;
#endif // BUILD_ALL_DEVICES
//�öε���˼�ǣ���������� HOLD_AUTO_START �궨�壬
//����������оƬ��ʱ�����ͣ�������̣�
//ֻ���ⲿ�����Ժ�Ż�����оƬ����ʵ������Ҫһ����ť���������������̡�
  
#if defined ( HOLD_AUTO_START ) 
  // HOLD_AUTO_START is a compile option that will surpress ZDApp
  //  from starting the device and wait for the application to
  //  start the device.
  ZDOInitDevice(0);   //ZDO��ʼ��
#endif

  
  /* ���÷������ݵķ�ʽ��Ŀ�ĵ�ַѰַģʽ*/
  //---------------------------
  //������Ϣ���㲥����
  // Setup for the periodic message's destination address
  // Broadcast to everyone
  /*�㲥�������豸�����ݵĹ㲥��ַ��ʼ��*/
  LockApp_Periodic_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;//15:�㲥
  LockApp_Periodic_DstAddr.endPoint = LOCKAPP_ENDPOINT;         //�˵�ţ�20
  LockApp_Periodic_DstAddr.addr.shortAddr = 0xFFFF;               //16λ�����ַ:0xFFFF
  
  /*������һ���豸*/
//  (WXL_LockApp_Single_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;;
//  WXL_LockApp_Single_DstAddr.endPoint = WXL_LOCKAPP_ENDPOINT;
//  LockApp_Periodic_DstAddr.addr.shortAddr = 0xFFFF;
 
//  LockApp_Periodic_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;    //2:����
//  LockApp_Periodic_DstAddr.endPoint = LOCKAPP_ENDPOINT;         //�˵�ţ�20
  //  LockApp_Periodic_DstAddr.addr.shortAddr = 0x0000;               //16λ�����ַ:0x0000
  
  
  
  
  //--------------------------
  /*������鲥��Ϣ��ʼ��*/
  // Setup for the flash command's destination address - Group 1
  LockApp_Flash_DstAddr.addrMode = (afAddrMode_t)afAddrGroup;    //1:�鲥
  LockApp_Flash_DstAddr.endPoint = LOCKAPP_ENDPOINT;           //�˵�ţ�20
  LockApp_Flash_DstAddr.addr.shortAddr = LOCKAPP_FLASH_GROUP;  //16λ�����ַ:0x0001,ͬʱ����Ϊ���

  // Fill out the endpoint description.
  /*���屾�豸����ͨ�ŵ�APS��˵�������*/
  /*�˵���Ϣ��ʼ��*/
  LockApp_epDesc.endPoint = LOCKAPP_ENDPOINT;                   //LockApp EP��������EP�ţ�20
  LockApp_epDesc.task_id = &LockApp_TaskID;                     //LockApp EP������������ID����ϸ������ͷ�� 
  LockApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&LockApp_SimpleDesc;   //LockApp EP��������
  LockApp_epDesc.latencyReq = noLatencyReqs;                      //��ʱ����

  // Register the endpoint description with the AF
  /*��AF��Ǽ�EP������*/
  /*�Ǽ�endpoint description ��AF,Ҫ�Ը�Ӧ�ý��г�ʼ������AF���еǼǣ�����Ӧ�ò�����ôһ��EP��
  ����ͨ����ʹ�ã���ô�²�Ҫ���й��ڸ�Ӧ�õ���Ϣ����Ӧ��Ҫ���²�����Щ���������Զ��õ��²����
  �ϡ�*/
  afRegister( &LockApp_epDesc );

  // Register for all key events - This app will handle all key events
  /*ע�ᰴ���¼�*/
  RegisterForKeys( LockApp_TaskID );

  // By default, all devices start out in Group 1 Ϊ��˸��Ϣ���õ���
  /*����Ϣ��ʼ��*/
  LockApp_Group.ID = 0x0001;                          //��ţ���LOCKAPP_FLASH_GROUPһ��
  osal_memcpy( LockApp_Group.name, "Group 1", 7  );   //�趨����
  aps_AddGroup( LOCKAPP_ENDPOINT, &LockApp_Group ); //�Ѹ���Ǽ���ӵ�APS��

  /*���֧��LCD����ʾһ���ַ�*/ 
#if defined ( LCD_SUPPORTED )
  HalLcdWriteString( "LockApp", HAL_LCD_LINE_1 );
#endif
}

/*********************************************************************
 * @fn      LockApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */

/*�¼�����������������������������������������*/
uint16 LockApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;     //���յ�����Ϣ����Ϣ��
  (void)task_id;  

  /*������¼��ǽ���ϵͳ��Ϣ*/ //�����ϵͳ��Ϣ�ٽ����ж�
  //ϵͳ��Ϣ���Է�Ϊ�����¼���AF���ݽ����¼�������״̬�ı��¼�
  if ( events & SYS_EVENT_MSG )
  {
    //�����ǽ��յ������ݰ������սṹ��afIncomingMSGPacket_t�ĸ�ʽ������Ϣ
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( LockApp_TaskID ); //����ϵͳ��Ϣ��������Ϣ��
    
    //������յ���Ϣ
    while ( MSGpkt )//���յ�
    {
      //���жϽ��յ�����Ϣ���ĸ��¼�����
      switch ( MSGpkt->hdr.event )
      {
      
        /*���������¼�*/
        case KEY_CHANGE:
          LockApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
          break; //ִ�о���İ�����������������sampleAPP.c��

          
          
         /*��Ϣ������Ӧ�¼�*/
        // ���������¼�,���ú���AF_DataRequest()��������
        // ������õ����Ľ�����Ϣ�¼�
        // Received when a messages is received (OTA) for this endpoint
        case AF_INCOMING_MSG_CMD:
          LockApp_MessageMSGCB( MSGpkt );//���ûص��������յ������ݽ��д���
          break;

        //LockApp.c LockApp_ProcessEvent������
        /*�豸����״̬�仯�¼�*/
        // ֻҪ����״̬�����ı䣬��ͨ��ZDO_STATE_CHANGE�¼�֪ͨ���е�����ע�⣬���������񶼻��յ�����Ϣ��
          
        //����������һ����ʱ����osal_start_timerEx()�������ʱ����������״̬�仯����¼�������
        //����ּ������״̬�����仯ʱ֪ͨ���е������豸��������
        //Ҳ����������ʱ�ϴ����������ݣ�����
        case ZDO_STATE_CHANGE:
          LockApp_NwkState = (devStates_t)(MSGpkt->hdr.status);//��ȡ�豸��ǰ״̬
          
          //����������Ҫ��ʱ�㲥���ݵ��豸
          //����Э����һ���ǲ��ö�ʱ�㲥���ݵ�,��ô�����������
          if ( (LockApp_NwkState == DEV_ZB_COORD)
              || (LockApp_NwkState == DEV_ROUTER)
              || (LockApp_NwkState == DEV_END_DEVICE) )
          {
            // Start sending the periodic message in a regular interval.
            /*��һ�����������ʱ��*/
            //�����ʱ����Ϊ����������Ϣ���õģ��Ҿ�����Ϊ�����������,�û��Լ���ӵ�����,
            //ֻ�������¼����������ⷢ����Ϣ��,һ���Ƿ���flash��˸��Ϣ��������Ѱַ��
            //����һ���Ƿ���periodic������Ϣ�����ڹ㲥��������һ���豸������״̬�����˱仯��
            //����Ҫ����ͬһ�����е������豸�����Ҫ���й㲥֪ͨ�����豸�����͵���Ϣ��Ӧ�û�������豸�����͡�
            //������֪���������Բ��ԡ������ģ��Ժ�����׵ġ�
            //���£������ʱ��ֻ��Ϊ����������Ϣ�����ģ��豸������ʼ��������￪ʼ������һ��������Ϣ�ķ��ͣ�
            //Ȼ���ܶ���ʼ��ȥ.
            //LockApp.c LockApp_ProcessEvent������
            osal_start_timerEx( LockApp_TaskID,
                              LOCKAPP_SEND_PERIODIC_MSG_EVT,
                              LOCKAPP_SEND_PERIODIC_MSG_TIMEOUT );
          }
          else
          {
            // Device is no longer in the network
          }
          break;

        default:
          break;
      }

      // Release the memory
      //���ϰ��յ�ϵͳ��Ϣ������¼��������ˣ��ͷ���Ϣռ�õ��ڴ�
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next - if one is available
      /*ָ��ָ����һ��"�ѽ��յ��ġ��۳�����while ( MSGpkt )�ڣݷ��ڻ������Ĵ�������¼�����
      // LockApp_ProcessEvent�������¼����Ӧ������while ( MSGpkt )���´����¼���
      //ֱ��������û�еȴ������¼�Ϊֹ��*/
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( LockApp_TaskID );
    }

    // return unprocessed events
     //��������򣬷���δ������¼���return��osal_start_system()�µ�events = (tasksArr[idx])( idx, events )����У�������osal_start_system()����ѯ�ٽ���˺������д���
    return (events ^ SYS_EVENT_MSG);
  }

  //LockApp.c LockApp_ProcessEvent������
  // Send a message out - This event is generated by a timer
  //  (setup in LockApp_Init()).
  /* ������¼������ⷢ����Ϣ�����¼��ɶ�ʱ������*/
  //����Ƕ�ʱ�¼�������ж�ʱ�¼�����
  if ( events & LOCKAPP_SEND_PERIODIC_MSG_EVT )
  {
    // Send the periodic message
    
    //�����������¼�������LockApp_SendPeriodicMessage()�����굱ǰ���������¼���
    //Ȼ��������ʱ��������һ�����������飬
    //����һ��ѭ����ȥ��Ҳ��������˵���������¼��ˣ�
    //������Ϊ��������ʱ�ɼ����ϴ�����
    
    LockApp_SendPeriodicMessage();          //�����Ƿ��ͺ���������Ҫ����������������������������������������

    // Setup to send message again in normal period (+ a little jitter)
    //����Ϊ����LockApp_TaskID���¼�LOCKAPP_SEND_PERIODIC_MSG_EVT�趨һ����ʱ����
    //��ʱʱ��Ϊ (LOCKAPP_SEND_PERIODIC_MSG_TIMEOUT + (osal_rand() & 0x00FF))��
    //��ʱ��һ���������е����񽫱�ͨ�����¼�������
    //ѭ�����ö�ʱ��
    osal_start_timerEx( LockApp_TaskID, LOCKAPP_SEND_PERIODIC_MSG_EVT,
        (LOCKAPP_SEND_PERIODIC_MSG_TIMEOUT + (osal_rand() & 0x00FF)) );

    // return unprocessed events
    return (events ^ LOCKAPP_SEND_PERIODIC_MSG_EVT);
  }

  // Discard unknown events
  return 0;
}

/*********************************************************************
 * Event Generation Functions
 */
/*********************************************************************
 * @fn      LockApp_HandleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
void LockApp_HandleKeys( uint8 shift, uint8 keys )
{
  (void)shift;  // Intentionally unreferenced parameter
  
  //�������1�����£����鲥����
  if ( keys & HAL_KEY_SW_1 )
  {
    /* This key sends the Flash Command is sent to Group 1.
     * This device will not receive the Flash Command from this
     * device (even if it belongs to group 1).
     */
    LockApp_SendFlashMessage( LOCKAPP_FLASH_DURATION );
  }
  
  //�������2�����£����Ƴ���
  if ( keys & HAL_KEY_SW_2 )
  {
    /* The Flashr Command is sent to Group 1.
     * This key toggles this device in and out of group 1.
     * If this device doesn't belong to group 1, this application
     * will not receive the Flash command sent to group 1.
     */
    aps_Group_t *grp;
    grp = aps_FindGroup( LOCKAPP_ENDPOINT, LOCKAPP_FLASH_GROUP );
    if ( grp )
    {
      // Remove from the group
      aps_RemoveGroup( LOCKAPP_ENDPOINT, LOCKAPP_FLASH_GROUP );
    }
    else
    {
      // Add to the flash group
      aps_AddGroup( LOCKAPP_ENDPOINT, &LockApp_Group );
    }
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
        HalLedBlink(HAL_LED_2,0,50,500);      //LED2���500ms��˸
        
        #if defined(ZDO_COORDINATOR)
        HalUARTWrite(0,"CoordinatorEB-Pro 57 Receive '11' OK\n", sizeof("CoordinatorEB-Pro 57 Receive 'D1' OK\n"));//���ڷ���
        HalLedBlink(HAL_LED_1,0,50,500);      //LED1���500ms��˸
        LockApp_SendPeriodicMessage();      //Э�������յ�D1�󷵻�D1���նˣ����ն�LED1Ҳ��˸
        #endif
      }
      else
      {
        HalLedSet(HAL_LED_2,HAL_LED_MODE_ON);
      }
      break;

    //�յ��鲥����
    case LOCKAPP_FLASH_CLUSTERID:
      flashTime = BUILD_UINT16(pkt->cmd.Data[1], pkt->cmd.Data[2] );
      HalLedBlink( HAL_LED_4, 4, 50, (flashTime / 4) );
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
    HalLedSet(HAL_LED_1,HAL_LED_MODE_ON);
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
