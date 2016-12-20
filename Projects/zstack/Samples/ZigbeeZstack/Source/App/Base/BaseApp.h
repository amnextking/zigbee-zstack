/**************************************************************************************************
  Filename:       BaseApp.h
  Date:           2016-12-13 $
  Revision:       1.0
  Description:    ����Ӧ�ó���ͷ�ļ�
  Author:         zhuxiankang
**************************************************************************************************/

#ifndef BASEAPP_H
#define BASEAPP_H

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
#define BASEAPP_ENDPOINT           20         //�˵�Ŷ���Ϊ20
#define BASEAPP_PROFID             0x0F08     //��ID
#define BASEAPP_DEVICEID           0x0001     //�豸ID
#define BASEAPP_DEVICE_VERSION     0          //�豸�汾��
#define BASEAPP_FLAGS              0          //����
#define BASEAPP_MAX_CLUSTERS       2          //�������/����ظ���(������Ҫ��Ϊ�㲥���鲥)
#define BASEAPP_PERIODIC_CLUSTERID 1          //�㲥��ID,������������
#define BASEAPP_FLASH_CLUSTERID     2         //�鲥��ID,������������

  
/*��������ˢ����������*/
#define BASEAPP_OFF_LINE_TASK_MSG_TIMEOUT   800    //������������ִ��ʱ����0.8s
#define BASEAPP_OFF_LINE_TASK_MSG_EVENT     0x0001  //������������ʱ�¼���־
  

  
// Flash Command Duration - in milliseconds
#define BASEAPP_FLASH_DURATION               1000
  
  
  
/*********************************************************************
 * ��������
 */
extern void BaseApp_Init( uint8 task_id );
extern UINT16 BaseApp_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BASEAPP_H */
