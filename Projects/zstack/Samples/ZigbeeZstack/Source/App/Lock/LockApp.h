/**************************************************************************************************
  Filename:       LockApp.h
  Date:           2016-12-13 $
  Revision:       1.0
  Description:    ����Ӧ�ó���ͷ�ļ�
  Author:         zhuxiankang
**************************************************************************************************/

#ifndef LOCKAPP_H
#define LOCKAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"

/*********************************************************************
 * ����
 */

/*Zigbee�豸�ڵ�򵥶˵�����������*/
#define LOCKAPP_ENDPOINT           20         //�˵�Ŷ���Ϊ20
#define LOCKAPP_PROFID             0x0F08     //��ID
#define LOCKAPP_DEVICEID           0x0001     //�豸ID
#define LOCKAPP_DEVICE_VERSION     0          //�豸�汾��
#define LOCKAPP_FLAGS              0          //����  
#define LOCKAPP_MAX_CLUSTERS       4          //�������/�������أ�����
  
  
  



/*�������ͣ���ID��*/
#define BROADCAST_TEST_ID   0x01          //Э�����㲥��������
#define SINGLE_TEST_ID      0x02          //�ն˵������ݲ���,����1
#define SINGLE_TEST_ID_2    0x03          //�ն˵������ݲ���,����2
#define SINGLE_TEST_ID_3    0x04          //Э�����������ݸ���վ����
  




  
  
  
/*��������ˢ����������*/
#define LOCKAPP_OFF_LINE_TASK_MSG_TIMEOUT   3000    //������������ִ��ʱ����0.8s
#define LOCKAPP_OFF_LINE_TASK_MSG_EVENT     0x0001  //������������ʱ�¼���־
  
  
  
/*Э������ַ*/
#define COORDINATOR_ADDR                    0x0000  
  
  
  

  
  
  
/*********************************************************************
 * ��������
 */
extern void LockApp_Init( uint8 task_id );
extern UINT16 LockApp_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LOCKAPP_H */
