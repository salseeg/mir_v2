#ifndef MIR__TYPES_H
#define MIR__TYPES_H

#include "constants.h"

/*	���������� �����	*/
struct status_bit{
	short state_fixed;				/*	������������� ��������� (SH_CONNECTED/SH_BROKEN)	*/
	short state_flow;				/* 	�������	�������� (SH_CONNECTED/SH_BROKEN)	*/
	unsigned long time_fixed;			/* 	������������ �������������� ��������� (���)	*/
	unsigned long time_flow;			/*	������������ �������� ���������	(���)	*/
	unsigned short powered;
};

struct digit_recognition{
	short	count;				/*	������� ���������� ���������	*/
	short	may_count;			/*	���� ����������� (��� 1) ��������� �������	*/
};

/*	������ ��� ������ ������������� �������� : ������, �����, �������� ������, ������� �������	*/
struct timer{
	short	type;				/*	��� �������	*/
	short	state;				/*	��������� (0/1)	*/
	long	time_to_change;			/*	����� �� ����� ���������	*/
	short	owner;				/*	��������� ������� (��������� �����)	*/
	int	count;
};

struct service_buffer{
	short length;				/*	����� ����� ����	*/
	short digits[NUMBER_LENGTH];		/*	���������� �������������������	*/
};

/**********************************************************************************************************/
struct status_array{
	struct status_bit inner[N_INNER], outer[N_OUTER];
	long	delta;					/*	��������� ������� � ����������� ������	*/
};

struct digits_array{
	struct digit_recognition inner[N_INNER],outer[N_OUTER];
};

struct timers_array{
	struct timer inner[N_INNER],outer[N_OUTER];
};

struct states_array{
	short	inner[N_INNER],outer[N_OUTER];		/* 	������ ���������� �����	*/
};

struct last_numbers_array{
	short 	inner[N_INNER][NUMBER_LENGTH];		/*	��������� ��������� �����	*/
};

struct line_numbers_array{
	short	inner[N_INNER],outer[N_OUTER];		/*	����� ��� ������ �����	*/
};
 
struct connection{
	short 	lines[N_LINES];				/*	��� �� ��������� ��������� � ����������	*/
	short	quantity;				/*	���-�� ��������� � ����������	*/
};

struct cur_con{
	short  inner[N_INNER],outer[N_OUTER];		/*	������� ����������	*/
};

struct main_lines_array{
	short inner[N_INNER],outer[N_OUTER];		/*	����� ����� �������� ����������	*/
};

struct srv_buf_array{
	struct service_buffer inner[N_INNER];
};
/***********************************************************************************************************/	
struct station{
	struct states_array states;			/*	������ ��������� �����	*/
	struct status_array status;			/*	��������� ������ �����	*/
	struct timers_array timers;			/*	������	*/
	struct digits_array digits;			/*	���������� �����	*/
	struct last_numbers_array last_numbers;		/*	��������� ��������� �����	*/
	struct line_numbers_array line_numbers;		/*	������ ��� �����	*/
	unsigned char registers[N_REGS];		/*	�������� ��������	*/
	short next_free_connection;			/*	������ ���������� ���������� ���� (-1 ?)	*/
	struct connection connections[N_CONN];		/*	������ ���������� == �������	*/
	short	inner2matrix[N_INNER];			/*	����� ����� ����� - ����� �� �������	*/
	short	outer2matrix[N_OUTER];			/*	����� ����� ����� - ����� �� �������	*/
	short	special2matrix[N_SPECIAL];		/*	����� ���� �����  - ����� �� �������	*/ /*	������	*/	
	short	incoming_connections[N_INNER];		/*	����� ��������� ����������	*/
	struct	cur_con current_connections;		/*	����� �������� ����������	*/	
	struct 	main_lines_array main_lines;		/*	������ ����� ������� ���������	*/
	struct 	srv_buf_array service_buffers;		/*	������������������ ���������� ���� � ������ �������������� �����	*/
	short 	line_owners[N_OUTER];			/* 	line reserving	*/
};

#endif
