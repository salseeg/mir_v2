#ifndef MIR__CONFIG
#define MIR__CONFIG

//
//	Конфигурирует и инициализирует основные объекты
//

#include <expat.h>
#include "aux/ring_.h"
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 4096
#define NUMBER_BUF_SIZE 100
#define FUNC_NAME_BUF 100

#include "net/server.h"
#include "hard.h"
#include "melodies.h"
#include "busses.h"
#include "inner_line.h"
#include "outer_line.h"
#include "masks.h"
#include "connection_masks.h"

#ifdef LOG_CONFIG
#include "log/log.h"
#endif


enum command_quantity{
	CQ_one,
	CQ_several
};

struct command {
	char * name;
	enum command_quantity count;
	void (*handler)(const XML_Char ** attr);
};

C_ring_< C_ring_<command> > * start;
C_ring_<command> * end;

/***********************************************************************************/
//	обработчики
/***********************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////
static melody_name curr_melody;

void frame_load(const XML_Char ** attr){
	int got = 0;
	int i = 0;
	melody_frame fr;
	while (attr[i]){
		if (!strcmp(attr[i], "state")){
			if (!strcmp(attr[i + 1], "high")){
				fr.state = melody_state__high;
				got++;
			} else if (!strcmp(attr[i + 1], "low")){
				fr.state = melody_state__low;
				got++;
			}
		}else if (!strcmp(attr[i], "duration")){
			fr.duration = atol(attr[i + 1]);
			fr.duration *= 1000;
			got++;
		}
		i += 2;
	}
	if (got == 2){
		(*Melodies)[curr_melody]->add_frame(fr);
#ifdef LOG_CONFIG		
		Log->rec() << ".";
#endif
		start->push(new C_ring_<command>);
	}
}

void melody(const XML_Char ** attr){
	melody_name name = melody__initial_pause;
	bool loop = false;
		
	int got = 0;
	int i = 0;
	while(attr[i]){
		if (!strcmp(attr[i], "name")){
			if (!strcmp(attr[i + 1], "ring")){
				name = melody__ring;
				got++;
			}else if (!strcmp(attr[i + 1], "busy")){
				name = melody__busy;
				got++;
			}else if (!strcmp(attr[i + 1], "deny")){
				name = melody__deny;
				got++;
			}else if (!strcmp(attr[i + 1], "notify")){
				name = melody__notify;
				got++;
			}else if (!strcmp(attr[i + 1], "ring_control")){
				name = melody__ring_control;
				got++;
			}else if (!strcmp(attr[i + 1], "ready")){
				name = melody__ready;
				got++;
			}else if (!strcmp(attr[i + 1], "ok")){
				name = melody__ok;
				got++;
			}else if (!strcmp(attr[i + 1], "got_connected")){
				name = melody__got_connected;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_0")){
				name = melody__digit_0;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_1")){
				name = melody__digit_1;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_2")){
				name = melody__digit_2;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_3")){
				name = melody__digit_3;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_4")){
				name = melody__digit_4;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_5")){
				name = melody__digit_5;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_6")){
				name = melody__digit_6;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_7")){
				name = melody__digit_7;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_8")){
				name = melody__digit_8;
				got++;
			}else if (!strcmp(attr[i + 1], "pulse_digit_9")){
				name = melody__digit_9;
				got++;
			}else if (!strcmp(attr[i + 1], "initial_pause")){
				name = melody__initial_pause;
				got++;
			}else{
#ifdef LOG_CONFIG				
				Log->set_priority(log_priority__warning);
				Log->rec() << "Неизвестная мелодия : " << attr[i + 1];
				Log->write();
#endif
			}
#ifdef LOG_CONFIG			
			Log->set_priority(log_priority__debug);
			Log->rec() << "       мелодия : " << attr[i + 1];
#endif
		} else if (!strcmp(attr[i], "loop")){
			if (!strcmp(attr[i + 1], "on")){
				loop = true;
				got++;
			}else if(!strcmp(attr[i + 1], "off")){
				loop = false;
				got++;
			}
		}
		i += 2;
	}
	if (got == 2) {
		C_melody * mm = new C_melody(loop);
		(*Melodies)[name] = mm;
		
		curr_melody = name;

		struct command * cmd = new command;
		C_ring_<command> * context = new C_ring_<command>;

		cmd->name = new char[6];
		::strcpy(cmd->name, "frame");
		cmd->count = CQ_several;
		cmd->handler = frame_load;
		context->add(cmd);
	
		start->push(context);
	}
}

void melodies(const XML_Char ** attr){
	int count;
	int i = 0;
	while (attr[i]){
		if (!::strcmp(attr[i], "count")){
			count = atoi((char *) attr[i + 1]);
			Melodies = new C_melodies(count);
#ifdef LOG_CONFIG			
			Log->set_priority(log_priority__debug);
			Log->rec() << "Мелодии [" << count << "]";
			Log->write();
#endif 
			
			struct command * cmd = new command;
			C_ring_<command> * context = new C_ring_<command>;
			
			cmd->name = new char[7];
			::strcpy(cmd->name, "melody");
			cmd->count = CQ_several;
			cmd->handler = melody;
			context->add(cmd);
			
			start->push(context);
			
			break;
		}
		i += 2;
	}	
}
///////////////////////////////////////////////////////////////////////////////////
void matrix_init(const XML_Char ** attr){
	int i = 0;
	int valid = 0; 
	int x = -1;
	int y = -1;

	start->push(new C_ring_<command>);

	while(attr[i]){
		if (!strcmp(attr[i], "x")){
			x = atoi(attr[i + 1]);
			valid++;
		}else if (!strcmp(attr[i], "y")){
			y = atoi(attr[i + 1]);
			valid++;
		}
		i += 2;
	}
	if (valid == 2){
		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
				Hard->matrix(i, j, off);
#ifdef LOG_CONFIG		
		Log->set_priority(log_priority__debug);
		Log->rec() << "Матрица комутаций обнулена.";
		Log->write();
#endif
	}
}

void hard(const XML_Char ** attr){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
	
	cmd = new struct command;
	cmd->name = new char[12];
	::strcpy(cmd->name, "matrix_init");
	cmd->count = CQ_one;
	cmd->handler = matrix_init;
	context->add(cmd);
	
	start->push(context);		// добавляем контекст


	int i = 0;
	int got = 0;
	char * drv_fn = NULL;
	int n_inner = -1;
	int n_outer = -1;
	while(attr[i]){
		if (!strcmp(attr[i], "driver_filename")){	
			drv_fn = (char *) attr[i + 1];
			got++;
		}else if(!strcmp(attr[i], "n_inner")){
			n_inner = atoi((char *) attr[i + 1]);
			got++;
		}else if(!strcmp(attr[i], "n_outer")){
			n_outer = atoi((char *) attr[i + 1]);
			got++;
		}
		i += 2;
	}
	if (got == 3){
		Hard = new C_hard(drv_fn, n_inner, n_outer);
#ifdef LOG_CONFIG
		Log->set_priority(log_priority__debug);
		Log->rec() << "Инициализирован интерфейс с аппаратурой ";
		Log->rec() << n_inner << "x" << n_outer;
		Log->write();
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////////
void server(const XML_Char ** attr){
	int i = 0;
	
	start->push(new C_ring_<command>);		//	добавляем пустой контекст
	
	while (attr[i]){
		if(!strcmp(attr[i], "port")){
			Server = new C_server(atoi(attr[i + 1]));
#ifdef LOG_CONFIG			
			Log->set_priority(log_priority__debug);
			Log->rec() << "Сервер ожидает входящие соединения. [ " << attr[i + 1] << " ]";
			Log->write();
#endif
		}else if(!strcmp(attr[i], "max_connections")){
			server_max_connections = atoi(attr[i + 1]);
		}else if(!strcmp(attr[i], "plugins_path")){
			plugins_path = new char[::strlen(attr[i + 1]) + 1];
			::strcpy(plugins_path, attr[i + 1]);
		}
		i += 2;
	}
}
///////////////////////////////////////////////////////////////////////////////////


void misc(const XML_Char ** attr){
	int i = 0;
	
	start->push(new C_ring_<command>);		//	добавляем пустой контекст
	
	while (attr[i]){
		if(!strcmp(attr[i], "recognition__low_time_min")){
			recognition__low_time_min = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "recognition__low_time_max")){
			recognition__low_time_max = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "recognition__high_time_min")){
			recognition__high_time_min = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "recognition__high_time_max")){
			recognition__high_time_max = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "recognition__inter_time_min")){
			recognition__inter_time_min = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "recognition__inter_time_max")){
			recognition__inter_time_max = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "line__outer_requried_ring_time")){
			line__outer_requried_ring_time = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "line__outer_incoming_ring_timeout")){
			line__outer_incoming_ring_timeout = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "line__outer_required_disconnect_timeout")){
			line__outer_required_disconnect_timeout = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "line__required_disconnet_time")){
			line__required_disconnet_time = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "line__ready_timeout")){
			line__ready_timeout = atol(attr[i + 1]);
		}else if(!strcmp(attr[i], "line__wait_4_connect_timeout")){
			line__wait_4_connect_timeout = atol(attr[i + 1]);
		}
		i += 2;
	}
}
///////////////////////////////////////////////////////////////////////////////////


void station_line(const XML_Char ** attr){
	start->push(new C_ring_<command>);
	
	int got = 0;	// обязат параметры
	
	enum {in, out} type = in;
	int id = -1; 
	int operator_bus_id = -1;
	int own_bus_id = -1;
	int client_id = -1;
	int priority = 0;

	for (int i = 0; attr[i]; i += 2){
		if (!strcmp(attr[i], "id")){
			id = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "type")){
			if (!strcmp(attr[i + 1], "inner")){
				type = in;
				got++;
			}else if (!strcmp(attr[i + 1], "outer")){
				type = out;
				got++;
			}			
		}else if (!strcmp(attr[i], "own_bus_id")){
			own_bus_id = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "operator_bus_id")){
			operator_bus_id = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "client_id")){
			client_id = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "priority")){
			priority = atoi(attr[i + 1]);
		}		
	}
	if (got > 4){
		C_line * l;
		if (type == in){
			l = new C_inner_line(id);
		} else {
			l = new C_outer_line(id);
		}
		l->own_bus_id = own_bus_id;
		l->client_id = client_id;
		l->operator_bus_id = operator_bus_id;
		Station->lines[id] = l;
#ifdef LOG_CONFIG
		Log->set_priority(log_priority__debug);
		Log->rec() << "    Линия " << id << ((type == in) ? " внутреняя":" внешняя");
		Log->write();
#endif
	}
}

void station_connection(const XML_Char ** attr){
	start->push(new C_ring_<command>);

	if (!strcmp(attr[0], "id")){
		int id = atoi(attr[1]);
		Station->free_connections.add(new C_connection(id));
#ifdef LOG_CONFIG
		Log->set_priority(log_priority__debug);
		Log->rec() << "    Соединение " << id;
		Log->write();
#endif
	}
}

void station(const XML_Char ** attr){
	int i = 0;
	C_ring_<command> * context = new C_ring_<command>;
	
	int n_i = -1;
	int n_o = -1;
	int got = 0;
	while(attr[i]){
		if (!strcmp(attr[i], "n_inner")){
			n_i = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "n_outer")){
			n_o = atoi(attr[i + 1]);
			got++;
		}
		i +=2;
	}

	if (got > 1){
		Station = new C_station(n_i, n_o);
		Station->lines = n_i + n_o;
		
		struct command * cmd;
	
		cmd = new struct command;
		cmd->name = new char[5];
		::strcpy(cmd->name, "line");
		cmd->count = CQ_several;
		cmd->handler = station_line;
		context->add(cmd);
		
		cmd = new struct command;
		cmd->name = new char[11];
		::strcpy(cmd->name, "connection");
		cmd->count = CQ_several;
		cmd->handler = station_connection;
		context->add(cmd);
#ifdef LOG_CONFIG
		Log->set_priority(log_priority__debug);
		Log->rec() << "Станция [ внутр: " << n_i << ", внешн: " << n_o << " ]";
		Log->write();
#endif
	}
	start->push(context);
}

///////////////////////////////////////////////////////////////////////////////////
void bus_line(const XML_Char ** attr){
	start->push(new C_ring_<command>);
	if (!::strcmp(attr[0], "id")){
		int id = ::atoi(attr[1]);
		Busses->ring.get()->lines.add(Station->lines[id]);
#ifdef LOG_CONFIG		
		Log->set_priority(log_priority__debug);
		Log->rec() << "       линия " << id;
		Log->write();
#endif

	}
}

void bus(const XML_Char ** attr){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
		
	bool roll = true;
	int got = 0;
	int  i = 0;
	int id = -1;
	while (attr[i]){
		if (!strcmp("roll", attr[i])){
			roll = (!strcmp(attr[i + 1], "true"));
		}else if (!strcmp("id",attr[i])){
			id = atoi(attr[i + 1]);
			got++;
		}
		i+=2;
	}
	if (got){
		Busses->ring.push(new C_bus(id, roll));
	
		cmd = new struct command;
		cmd->name = new char[5];
		::strcpy(cmd->name, "line");
		cmd->count = CQ_several;
		cmd->handler = bus_line;
		context->add(cmd);
#ifdef LOG_CONFIG		
		Log->set_priority(log_priority__debug);
		Log->rec() << "    пучек " << id; 
		Log->write();
#endif
	}
	start->push(context);		// добавляем контекст
}

void busses(const XML_Char ** attr){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
	
	cmd = new struct command;
	cmd->name = new char[4];
	::strcpy(cmd->name, "bus");
	cmd->count = CQ_one;
	cmd->handler = bus;
	context->add(cmd);
	
	start->push(context);		// добавляем контекст

	Busses = new C_busses;
#ifdef LOG_CONFIG
	Log->set_priority(log_priority__debug);
	Log->rec() << "Пучки";
	Log->write();
#endif
}
//////////////////////////////////////////////////////////////////////////////////
void mask_client(const XML_Char ** attr){
	start->push(new C_ring_<command>);
	if (!strcmp(attr[0], "id")){
		int id = atoi(attr[1]);
		Masks->ring.get(-1)->allow_client(id);
#ifdef LOG_CONFIG
		Log->rec() << " " << id;
#endif
	}
}

void mask(const XML_Char ** attr){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
	int got = 0;
	char * number = NULL;
	int bus_id = -1;
	int skip_len = -1;
	char * action_name = NULL;
	for (int i = 0; attr[i]; i += 2){
		if (!strcmp(attr[i], "number")){
			number = (char *) attr[i + 1];
			got++;
		}else if (!strcmp(attr[i], "bus_id")){
			bus_id = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "skip_len")){
			skip_len = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "action")){
			action_name = (char *) attr[i + 1];
			got++;
		}	
	}
	if (got > 3){
		C_mask * m = new C_mask;
		if (!(m->set(number, action_name, bus_id, skip_len))){
			Masks->ring.add(m);

			cmd = new struct command;
			cmd->name = new char[7];
			strcpy(cmd->name, "client");
			cmd->count = CQ_several;
			cmd->handler = mask_client;
			context->add(cmd);
#ifdef LOG_CONFIG
			Log->set_priority(log_priority__debug);
			Log->rec() << "   маска " << number << " (" << skip_len << ") ";
			Log->rec() << action_name << ", пучек " << bus_id << ", клиенты :";	
#endif
		} else{
			delete m;
		}
	}
	start->push(context);
}

void masks(const XML_Char **){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
	
	cmd = new struct command;
	cmd->name = new char[5];
	strcpy(cmd->name, "mask");
	cmd->count = CQ_several;
	cmd->handler = mask;
	context->add(cmd);

	start->push(context);

	Masks = new C_masks();
#ifdef LOG_CONFIG
	Log->set_priority(log_priority__debug);
	Log->rec() << "Маски";
	Log->write();
#endif	
}
//////////////////////////////////////////////////////////////////////////////////
void con_mask_client(const XML_Char ** attr){
	start->push(new C_ring_<command>);
	if (!strcmp(attr[0], "id")){
		int id = atoi(attr[1]);
		Connection_masks->ring.get(-1)->allow_client(id);
#ifdef LOG_CONFIG
		Log->rec() << " " << id;
#endif 
	}
}

void con_mask(const XML_Char ** attr){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
	int got = 0;
	char * number = NULL;
	int arg = -1;
	char * action_name = NULL;
	for (int i = 0; attr[i]; i += 2){
		if (!strcmp(attr[i], "number")){
			number = (char *) attr[i + 1];
			got++;
		}else if (!strcmp(attr[i], "arg")){
			arg= atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "action")){
			action_name = (char *) attr[i + 1];
			got++;
		}	
	}
	if (got > 2){
		C_connection_mask * m = new C_connection_mask;
		if (!(m->set(number, action_name, arg))){
			Connection_masks->ring.add(m);

			cmd = new struct command;
			cmd->name = new char[7];
			strcpy(cmd->name, "client");
			cmd->count = CQ_several;
			cmd->handler = con_mask_client;
			context->add(cmd);
#ifdef LOG_CONFIG
			Log->set_priority(log_priority__debug);
			Log->rec() << "   маска " << number << "  ";
			Log->rec() << action_name << " [ " << arg << " ] клиенты :";	
#endif
		} else{
			delete m;
		}
	}
	start->push(context);
}

void con_masks(const XML_Char **){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
	
	cmd = new struct command;
	cmd->name = new char[17];
	strcpy(cmd->name, "connection_mask");
	cmd->count = CQ_several;
	cmd->handler = con_mask;
	context->add(cmd);

	start->push(context);

	Connection_masks = new C_connection_masks();
#ifdef LOG_CONFIG
	Log->set_priority(log_priority__debug);
	Log->rec() << "Маски соединения";
	Log->write();	
#endif
}

////////////////////////////////////////////////////////
void matrix_connect(const XML_Char ** attr){
	C_ring_<command> * context = new C_ring_<command>;
	start->push(context);

	int cord = -1;
	int line = -1;
	int got = 0;
	int i = 0;
	while (attr[i]){
		if (!strcmp(attr[i], "cord")){
			cord = atoi(attr[i + 1]);
			got++;
		}else if (!strcmp(attr[i], "line")){
			line = atoi(attr[i + 1]);
			got++;
		}
		i += 2;
	}
	if (got > 1){
		Hard->matrix(cord, line, on);
	}
}


void music(const XML_Char **){
	struct command * cmd;
	C_ring_<command> * context = new C_ring_<command>;
	
	cmd = new struct command;
	cmd->name = new char[16];
	strcpy(cmd->name, "matrix_connect");
	cmd->count = CQ_several;
	cmd->handler = matrix_connect;
	context->add(cmd);

	start->push(context);

//	if (init_music() < 0){
//		Log->set_priority(log_priority__error);
//		Log->rec() << "Ошибка включения музыки";
//		Log->write();	
//	}else{
//		Log->set_priority(log_priority__debug);
//		Log->rec() << "Проигрывание музыки включено";
//		Log->write();	
//	}
}



/**********************************************************************************
**********************************************************************************/
void mir_config(const XML_Char ** attr){
	struct command * cmd;
	C_ring_<command> * n_context = new C_ring_<command>;
	
	cmd = new struct command;
	cmd->name = new char[5];
	strcpy(cmd->name, "Misc");
	cmd->count = CQ_one;
	cmd->handler = misc;
	n_context->add(cmd);
	
	cmd = new struct command;
	cmd->name = new char[7];
	strcpy(cmd->name, "Busses");
	cmd->count = CQ_one;
	cmd->handler = busses;
	n_context->add(cmd);
	
	cmd = new struct command;
	cmd->name = new char[9];
	strcpy(cmd->name, "Melodies");
	cmd->count = CQ_one;
	cmd->handler = melodies;
	n_context->add(cmd);
	
	cmd = new struct command;
	cmd->name = new char[5];
	strcpy(cmd->name, "Hard");
	cmd->count = CQ_one;
	cmd->handler = hard;
	n_context->add(cmd);

	cmd = new struct command;
	cmd->name = new char[7];
	strcpy(cmd->name, "Server");
	cmd->count = CQ_one;
	cmd->handler = server;
	n_context->add(cmd);

	cmd = new struct command;
	cmd->name = new char[9];
	strcpy(cmd->name, "Station");
	cmd->count = CQ_one;
	cmd->handler = station;
	n_context->add(cmd);
	
	cmd = new struct command;
	cmd->name = new char[6];
	strcpy(cmd->name, "Masks");
	cmd->count = CQ_one;
	cmd->handler = masks;
	n_context->add(cmd);

	cmd = new struct command;
	cmd->name = new char[17];
	strcpy(cmd->name, "Connection_masks");
	cmd->count = CQ_one;
	cmd->handler = con_masks;
	n_context->add(cmd);

	cmd = new struct command;
	cmd->name = new char[7];
	strcpy(cmd->name, "music");
	cmd->count = CQ_one;
	cmd->handler = music;
	n_context->add(cmd);
		
	start->push(n_context);
}
/***********************************************************************************/
void fill_first_context(){
	struct command * cmd;
	C_ring_<command> * f_context = new C_ring_<command>;
	
	cmd = new struct command;
	cmd->name = new char[11];
	strcpy(cmd->name, "mir_config");
	cmd->count = CQ_one;
	cmd->handler = mir_config;
	f_context->add(cmd);

	start->add(f_context);
}

/***********************************************************************************/
static void start_handler(void * data,const XML_Char * name,const XML_Char ** attr){
	C_ring_<command> * context = start->get();	
	int i, n = context->quantity();

	for (i = 0; i < n; i++, context->roll(1)){
		struct command * cmd = context->get();
		if (!strcmp(cmd->name, name)){
			void (*func)(const XML_Char **attr) = cmd->handler;
				
			if (end->get() != cmd){
				end->push(cmd);
			}
			func(attr);			
		}
	}
}

static void end_handler(void * data,const XML_Char * name){
	C_ring_<command> * context = start->pop();	
	int i, n = context->quantity();
	for (i = 0; i < n; i++) {
		struct command * cmd = context->pop();
		delete []  cmd->name;
		delete cmd;
	}
	delete context;
	
	int finish = 0;
	while (!finish){
		command * end_cmd = end->get();
		if (!strcmp(end_cmd->name, name)){
			if (end_cmd->count == CQ_one){
				end->pop();
			}
			finish++;
		} else{
			end->pop();
			finish++;
		}
	}
}	

int configure(char * filename){
	XML_Parser parser = XML_ParserCreate(NULL);
	FILE * file;
	char buf[BUF_SIZE];
	int done = 0;

	start = new C_ring_< C_ring_<command> >;
	end = new C_ring_<command>;
	
	Log = new C_log("MIR station");

	if((file = fopen(filename, "r")) == NULL){
		Log->set_priority(log_priority__alert);
		Log->rec() << "Can't open file : " << filename;
		Log->write();
		return -1;
	}
	
	XML_SetElementHandler(parser, start_handler, end_handler);

	fill_first_context();

	while(!done){
		int len;
		len = fread(buf, 1, BUF_SIZE,file);
		if (ferror(file)){
			Log->set_priority(log_priority__error);
			Log->rec() << "Error reading file " << filename;
			Log->write();
			return -1;
		}
		if (!XML_Parse(parser, buf, len, done = feof(file))){
			Log->set_priority(log_priority__error);
			Log->rec() << "Error parsing line " << XML_GetCurrentLineNumber(parser);
			Log->rec() << " : " <<  XML_ErrorString(XML_GetErrorCode(parser));
			Log->write();
			return -1;
		}
	}
	
	XML_ParserFree(parser);

	return 0;
}

#endif
