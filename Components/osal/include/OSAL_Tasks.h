/**************************************************************************************************
  Filename:       OSAL_Tasks.h
  Revised:        $Date: 2007-10-28 18:41:49 -0700 (Sun, 28 Oct 2007) $
  Revision:       $Revision: 15799 $

  Description:    This file contains the OSAL Task definition and manipulation functions.


  Copyright 2004-2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

#ifndef OSAL_TASKS_H
#define OSAL_TASKS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
#define TASK_NO_TASK      0xFF

/*********************************************************************
 * TYPEDEFS
 */

/*
 * Event handler function prototype
 */
typedef unsigned short (*pTaskEventHandlerFn)( unsigned char task_id, unsigned short event );

//����һ�����ͣ�pTaskEventHandlerFn��һ��������
//ǰ���и�typedef����pTaskEventHandlerFn������һ�ֺ���ָ�����͵ı���
//���ֺ���ָ��ָ��M(task_id,event)�������͵ĺ���������������¼���������
//������������������һ��ָ������ָ��X,��X��ú����ĵ�ַ��,�Ϳ��������
//ԭ������һ����ʹ���������ָ��X(task_id,event)���������pTaskEventHandlerFn tasksArr[ ]
//���tasksArr[ ]��һ��ָ�����飬���ڲ��ĸ�Ԫ�ض���ָ�룬��OSAL_SampleApp.c�ж�tasksArr[ ]
//�Ķ������ǿ���֪��macEventLoop��nwk_event_loopһֱ��SampleApp_ProcessEvent��ЩԪ�ض���
//ָ����������ն���ָ��M(task_id,event)�������͵ĺ���,Ҳ���Ǹ�����¼���������

//�����Ҿ��ã����� pTaskEventHandlerFn tasksArr[ ]��֮�����ͨ��tasksArr[ ]ֱ��
//����tasksArr[ ](task_id,event)��Ҳ����ͨ��tasksArr[ ]�����Ԫ��ֱ�ӵ���
//macEventLoop(task_id,event)...SampleApp_ProcessEvent(task_id,event),����Щ�����¼�����
//��������uint16���͵�.���osal_start_system()�����events = (tasksArr[idx])( idx, events );eventsҲ��uint16���͵�

/*********************************************************************
 * GLOBAL VARIABLES
 */

extern const pTaskEventHandlerFn tasksArr[];
extern const uint8 tasksCnt;
extern uint16 *tasksEvents;

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Call each of the tasks initailization functions.
 */
extern void osalInitTasks( void );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_TASKS_H */
