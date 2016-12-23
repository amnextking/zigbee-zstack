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

#include "dri_uart.h"

/* ------------------------------------------------------------------------------------------------
 *                                      variable
 * ------------------------------------------------------------------------------------------------
 */



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
#define FRAME_RX_SEQ            4           //SEQ ֡����
#define FRAME_RX_BASE_ID        5           //BASE_ID��վID
#define FRAME_RX_DOOR_ID        6           //DOOR_ID����ID
#define FRAME_RX_RESP           7           //RESP



#define FRAME_RX_DEV_ID         7           //DEV_ID Ŀ���豸ID,��վID���վ��������ID
#define FRAME_RX_CMD            8           //CMD �������������
#define FRAME_RX_DATA           9           //DATA ���������
#define FRAME_RX_LRC            10          //LRC ���ݵ�����
#define FRAME_RX_EOP            11          //EOP ֡β
#define FRAME_RX_END            12          //����֡������մ������˳�����֡

/*����֡�ֽ�*/
#define SOP                     0xAA

#define SEq                     0x01        

#define START_DEV_PC            0x01        //��ʼ�豸ΪPC
#define START_DEV_BASE          0x02        //��ʼ�豸Ϊ��վ
#define START_DEV_DOOR          0x03        //��ʼ˭��Ϊ����

#define END_DEV_NONE            0x00
#define END_DEV_PC              0x01        //Ŀ���豸ΪPC   
#define END_DEV_BASE            0x02        //Ŀ���豸Ϊ��վ
#define END_DEV_DOOR            0x03        //Ŀ���豸Ϊ����

#define EOP                     0x0E      










/*����֡��󳤶�*/
#define Rx_BUFF                 0xFF
#define Tx_BUFF                 0xFF

/****************************************�������ͺ궨��******************************************/

//��������
#define BASE_CMD_REQUEST        0x00        //�Ǿ����������PC������������

#define BASE_CMD_READID         0xA1        //��վID�鿴
#define BASE_CMD_OPENDOOR       0x03        //����




//����״̬
#define BASE_RESP_OK            0xEE        //�����ɹ�
#define BASE_RESP_SOP_ERR       0x10        //֡ͷ����
#define BASE_RESP_SEQ_ERR       0x11        //֡���д���
#define BASE_START_DEV_ERR      0x12        //��ʼ�豸ʶ�����
#define BASE_END_DEV_ERR        0x13        //Ŀ���豸ʶ�����
#define BASE_RESP_LRC_ERR       0x14        //LRCУ�����
#define BASE_RESP_LENG_ERR      0x15        //��������֡������
#define BASE_RESP_CMD_ERR       0x16        //������δ֪
#define BASE_RESP_EOP_ERR       0x17        //֡β����






/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */

//����֡���մ���
void Tcp_Init(void);
void Tcp_Receive(uint8 Rx_Data);
void Tcp_Process(uint8 Rx_Data);
//bool TcpClient_IsCmdExist(uint8 RxCmd);



//main������ѯ����֡
//void TcpClient_IsFullFrame(void);

//��վ���������[����֡���մ���]
//void TcpClient_ErrFrame_Process(void);

//��վ���������[����֡������ȷ]
//void TcpClient_BaseCMD_Process(void);

//void TcpClient_BaseCMD_ReadID(void);

//���͸�PC������֡��������
//void TcpClient_createFrame(uint8* frame,uint8 SEQ, uint8 FPB_CMD, uint8 RESP, uint8 *data,uint8 dataLength);






void Tcp_SendtoPC(uint8 CMD, uint8* Id, uint8 RESP, uint8 *data, uint8 DataLength); //�������ݸ�PC






/**************************************************************************************************
 */
#endif