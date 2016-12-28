#ifndef APP_TCP_H_
#define APP_TCP_H_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "hal_types.h"
#include<ioCC2530.h>
#include <string.h>
#include <stdbool.h>
#include "OSAL.h"
#include "dri_uart.h"



/*����֡��󳤶�*/
#define Rx_BUFF                 0xFF
#define Tx_BUFF                 0xFF


/* ------------------------------------------------------------------------------------------------
 *                                      variable
 * ------------------------------------------------------------------------------------------------
 */
#ifndef APP_TCP_C_
  #define TCP_C_Extern extern
#else
  #define TCP_C_Extern
#endif


/*ȫ�ֱ���*/
typedef struct{
  
  //�ͽ������ݵĴ�������й�
  uint8 RxData;           //���յ�������
  uint8 RxStatus;         //����λ�����������͵�����֡���������
  uint8 RxProgress;       //��ǰһ֡���ݵĽ��ս���
  uint8 RxCount;          //����ֵ
  
  
  //�������ݵľ�������
  uint8 CMD;              //������
  uint8 LENG;             //֡����
  uint8 DOOR_ID[2];       //����ID
  uint8 RESP;             //����״̬
  uint8 DATA[Rx_BUFF];    //���ݣ������Խ���255�ֽ�
  uint8 LRC;              //У��
  
}Typedef_Frame;

TCP_C_Extern Typedef_Frame Frame;      //��ǰ�ļ���BaseApp.c��ȫ�ֱ���




/* ------------------------------------------------------------------------------------------------
 *                                          Define
 * ------------------------------------------------------------------------------------------------
 */

/*IPORT�˿ڶ���*/
#define RST         P0_1                    //�͵�ƽ��λ��Ч
#define FUNCTION    P0_4                    //Ĭ������£�TCP Server��TCP Client�������ӽ���ʱ����͵�ƽ�����ӶϿ�ʱ����ߵ�ƽ               


/****************************************����֡�궨��******************************************/

/*����֡���������*/
#define FRAME_RX_START          11          //��ʼ׼���������ݻ��߿���
#define FRAME_RX_OK             22          //����������һ֡��������
#define FRAME_RX_ERR            33          //����֡����

/*һ֡���ݵĽ��ս���*/
#define FRAME_RX_SOP_START      1           //SOP ֡ͷ1
#define FRAME_RX_SOP_CHECK      2           //SOP ֡ͷ2
#define FRAME_RX_LENG           3           //LENG ֡���ݳ���
#define FRAME_RX_CMD            4           //CMD �������������
#define FRAME_RX_DOOR_ID        5           //DOOR_ID����ID
#define FRAME_RX_DATA           6           //DATA ���������
#define FRAME_RX_LRC            7           //LRC ���ݵ�����
#define FRAME_RX_EOP            8           //EOP ֡β
#define FRAME_RX_END            9           //����֡������մ������˳�����֡





/*����֡�ֽ�*/
#define SOP                     0xAA
#define EOP                     0x0E      

/*����֡����С����*/
#define MIN_LENG                0x03    //��λ����������֡����С���ȣ���CMD ~ DATA[n]�ĳ���
#define FPB_MIN_LENG            0x04    //��λ����������֡����С����

/*����֡�ĵ�ǰ���������*/
#define MAX_CMD                 0x02    //������1����


/****************************************�������ͺ궨��******************************************/

//��������
#define BASE_CMD_INIT                 0x00        //�����ʼ����������
#define BASE_CMD_GET_ASSOCLIST        0x01        //��ȡ��վ�Ĺ����б�   
#define BASE_CMD_OPEN_DOOR            0x02        //Զ�̿���



//����״̬
#define BASE_RESP_OK            0xEE        //�����ɹ�


//֡������
#define BASE_RESP_LENG_ERR      0x10        //��������֡������
#define BASE_RESP_CMD_ERR       0x11        //������δ֪
#define BASE_RESP_LRC_ERR       0x12        //LRCУ�����
#define BASE_RESP_EOP_ERR       0x13        //֡β����

//�������ͷ���
//#define BASE_RESP_NO_DOOR_LIST  0x20        //û�����������б���Ϣ





/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */

//����֡���մ���
void Tcp_Init(void);
void Tcp_Receive(uint8 Rx_Data);
void Tcp_Process(uint8 Rx_Data);
void Tcp_Send(uint8 FPB_CMD, uint8* DoorId, uint8 RESP, uint8 *data, uint8 DataLength);













/**************************************************************************************************
 */
#endif