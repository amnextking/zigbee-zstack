/**************************************************************************************************
  Filename:       BaseApp.h
  Date:           2016-12-13 $
  Revision:       1.0
  Description:    ��վӦ�ó���ͷ�ļ�
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
#define BASEAPP_MAX_CLUSTERS       4          //�������/�������أ�����
  
  
  



/*�������ͣ���ID��*/
#define BROADCAST_TEST_ID   0x01          //Э�����㲥��������
#define SINGLE_TEST_ID      0x02          //�ն˵������ݲ���,����1
#define SINGLE_TEST_ID_2    0x03          //�ն˵������ݲ���,����2
#define SINGLE_TEST_ID_3    0x04          //Э�����������ݸ���������
  
  
/*��վ��ʱ������ʱû�ã�*/
#define BASEAPP_OFF_LINE_TASK_MSG_TIMEOUT   2000    //��վ��������
#define BASEAPP_OFF_LINE_TASK_MSG_EVENT     0x0001  //��վ��������ʱ�¼���־
  

/*�㲥��ַ*/
#define BROADCAST_ADDR            0xFFFF  
  
  
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
