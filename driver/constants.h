/***************************************************************
 * 
 * 
 *
 *	���������.
 *	
 *	TODO ����� ������ ����
 *
 *
 */ 

#ifndef MIR__CONSTANTS_H
#define MIR__CONSTANTS_H

#define N_INNER 		8 		  /*	���-�� ����� �����	*/
#define N_OUTER 		4		/* 	���-�� ���� �����	*/
#define N_LINES			16		/*	���-�� �����	*/
#define N_CONN			8		/*	���-�� ����������	*/
#define N_SPECIAL		1		/*	���-�� ���� �����	*/
#define NUMBER_LENGTH		20		/*	���-�� ���� � ������	*/

#define BASE_PORT		0x150		/*	������� ����	*/
#define N_REGS			7		/*	���-�� ���������	*/

/*	�������� ��� ���������	*/
#define OFS_PV			0		/*	������� ������	*/
#define	OFS_KNA			1		/*	����� ������ ��	*/
#define OFS_MT_ADR		2		/*	����� �� �������	*/
#define OFS_MT_SS		3		/*	����� � ������ �� �������	*/
#define OFS_R_SH		4		/*	���� ������	*/
#define OFS_IK			5		/*	���������� ����	*/
#define OFS_SK			6		/*	����������� ����	*/

/*		*/
#define NO_CONNECTION		-1		/*	������� � ��������� ����������	*/

#define	NO_OWNER		-1		/*	��������� �������	*/

#define M_ON			1		/*	��������� ���������	*/
#define M_OFF			0		/*	���������� ���������	*/

#define T_ST_HI			1		/*	������� ������� �������	*/
#define T_ST_LO			0		/*	����� ������� �������	*/

/*	��������� ������	*/
#define SH_BROKEN		0		/*	����� ��������� */
#define	SH_CONNECTED		1		/*	����� �������	*/

/*	�������� ���� ������� �����	*/
#define ST_O_FREE		0
#define ST_O_INNER_CONNECTION	5
#define ST_O_CALL		2
#define ST_O_CONNECTION		3
#define ST_O_WAIT_4_CONNECT	4		/*	������ ������, ������� ����	*/	
#define ST_O_DISCONNECTED	1		/*	���������� ������� ������	*/
#define ST_O_REDIAL_DIGIT	6		/*	����� ���������� ���������� ������	*/
#define ST_O_REDIAL_PAUSE	7		/* 	pause between digits */

/*	�������� ���� ���������� �����	*/
#define	ST_I_DISCONNECTED	0		/*	�������� "������"	*/
#define ST_I_FREE		1		/* 	������ ��������		*/	
#define ST_I_READY		2		/*	������ �������		*/
#define ST_I_FIRST_DIGIT	3		/*	����� ����� �����	*/
#define ST_I_WAIT_4_CONNECT	4		/*	����� ��������		*/
#define ST_I_CONNECTION		5		/*	����������		*/
#define ST_I_CH_0_LINE		6		/*	����� ����� �����	*/
#define ST_I_NUM_RETRANSLATION	7	/*	������������ ������	*/
#define ST_I_SERVICE		8		/*	���			*/
#define ST_I_CONNECTION_SERVICE	9	/*	��� � ���������	*/
#define ST_I_SECOND_DIGIT	10		/*	����� ��������	*/
#define ST_I_PRE_CONNECTION	11		/*	��������� ��������� ������	*/

#define OUTER_REQ_DISCONNECT_TIMEOUT	900000		/*	���������� ������� ������	*/
#define READY_TIMEOUT			20000000	/*	������� �������� ������ �����	*/
#define	WAIT_4_CONNECT_TIMEOUT		45000000	/*	������� �������� ����������	*/
#define OUTER_INC_RING_TIMEOUT		4000000		/*	����� ����������� ���������� �������	*/
#define REQUIRED_DISCONNECT_TIME	200000		/*	������� ������� �� �������� � �������� ���������	*/ 
#define TIME_2_MAIN			2500000		/*	����� �� ����������� �� �������� ��� ������ ������	*/

#define RETRANSLATION_NUMBER	5		/*	����������� ���-�� ���� ��� ��������� ��������(������� � ST_I_CONNECTION)	*/
#define END_OF_NUMBER   	60000000	/*	������� ��������� ������	*/


#define DIGIT_OUTER		0		/*	����� �� �������	*/
//#define DIGIT_SERVICE		1		/*	����� �� ������		*/

/*	�������		*/
#define	SIG_NO			-1		/*	��� �������		*/
#define SIG_RING		0		/*	�����			*/
#define SIG_BUSY		1		/*	������			*/
#define SIG_DENY		2		/*	����������		*/
#define SIG_NOTIFY		3		/*	�����������		*/
#define SIG_CTRL		4		/*	�������� ������		*/
#define SIG_READY		5		/*	����� ������� "������"	*/
#define	SIG_OK			6		/*	������ ��������� �������	*/
#define SIG_REDIAL		7		/*	����� ���������� ���������� ������	*/
#define SIG_REDIAL_INTER	8		/*	����������� �����	*/
#define SIG_CONNECTION		9		/*	��� �������� �� ������������ ������ � ����������	*/

#define SI_COUNT_INFINITY	-1

#define SIG_RING_PULSE		800000
#define SIG_RING_PAUSE		3200000
#define SIG_RING_COUNT		SI_COUNT_INFINITY
#define SIG_BUSY_PULSE		300000
#define SIG_BUSY_PAUSE		300000
#define SIG_BUSY_COUNT		SI_COUNT_INFINITY
#define SIG_DENY_PULSE  	50000
#define SIG_DENY_PAUSE  	50000
#define	SIG_DENY_COUNT		SI_COUNT_INFINITY
#define SIG_NOTIFY_PULSE	250000
#define SIG_NOTIFY_PAUSE        2525000
#define SIG_NOTIFY_COUNT	SI_COUNT_INFINITY
#define SIG_CTRL_PULSE		800000
#define SIG_CTRL_PAUSE		3200000
#define SIG_CTRL_COUNT		SI_COUNT_INFINITY
#define	SIG_READY_PULSE		40000000
#define	SIG_READY_PAUSE		0
#define SIG_READY_COUNT		SI_COUNT_INFINITY
#define	SIG_OK_PULSE		100000
#define SIG_OK_PAUSE    	300000
#define SIG_OK_COUNT		SI_COUNT_INFINITY
#define SIG_REDIAL_PULSE	60000

#define SIG_REDIAL_PAUSE	40000
#define SIG_REDIAL_COUNT	SI_COUNT_INFINITY

#define SIG_REDIAL_INTER_PULSE	600000
#define SIG_REDIAL_INTER_PAUSE	0
#define SIG_REDIAL_INTER_COUNT	1
#define SIG_CONNECTION_PULSE	100000
#define SIG_CONNECTION_PAUSE	300000
#define SIG_CONNECTION_COUNT	3



/*	��������� ��� ����������� ������ (���)	*/
#define	PULSE_TIME_MIN		20000		/*	����������� ����� ��������	*/
#define PULSE_TIME_MAX		200000		/*	������������ ����� ��������	*/
#define PAUSE_TIME_MIN		20000		/*	����������� ����� �����		*/
#define PAUSE_TIME_MAX		200000		/*	������������ ����� �����	*/
#define INTER_TIME_MIN		250000		/*	����������� ����� ����������� �����	*/
#define INTER_TIME_MAX		600000000	/*	������������ ����� ����������� �����	*/

/*	������ recognize_digit()	*/
#define	E_DIGIT_RECOGNITION_ERROR	-1	/*	������ ����� ������������ ����� (������� �������) =>"������"	*/
#define E_DIGIT_RECOGNITION_NOT_ENDED	-2	/*	����� ����� �� ��������, ������ ��������	*/
#define	E_DIGIT_RECOGNITION_NOT_STARTED	-3	/*	����� �� ��������, ���	*/

/* 	read_phone_masks()	*/
#define READ_PHONE_MASKS_BUF_LEN 	240

/*	check_phone_masks()	�������� ������������ ������ �����	*/
#define E_MASK_NOT_FOUND		0	/*	����� ����� �� �������	*/
#define E_MASK_NOT_COMPLITED		1	/*	���� ����� ������������ ��� ��	*/
#define E_MASK_COMPLITED		2	/*	����� ����� �����	*/


#endif

