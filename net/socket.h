#ifndef __MIR__SOCKET
#define __MIR__SOCKET
//
//	Сокет
//

enum sock_state{		// Состояния сокета:
	sock_state__free,	// 	свободен
	sock_state__connected,	// 	подключен
	sock_state__binded,	// 	привязан
	sock_state__listened	// 	слушает
};

enum sock_blocking{		// Блокировка
	sock_blocking__on,	//	включен
	sock_blocking__off	//	выключена
};

struct inet_address{		// Адрес второй стороны
	char * name;		//	имя (somwhere.net / 122.22.33.4)
	int port;		//	порт (21)
};

class C_socket{
	private:
		int sock_id;					// хендл сокета
		sock_state state;				// состояние сокета
		inet_address other_side;			// адрес второй стороны
	public:
		C_socket();					// создание сокета
		C_socket(int sock_id,inet_address os);		// новое соединение
		~C_socket();					// удаление сокета
		int connect(char * server_name,int server_port);// подключение сокета к удаленному серверу
		int bind(int port);				// привязка сокета к локальному порту
		C_socket * accept();				// прием поступающего соединения
		int listen(int queue_size);			// устанавливает размер очереди для поступающих соединений
		int read(void * buf,int len);			// чтение данных
		int write(const void * buf,int len);		// запись данных
		int close();					// завершение соединения
		inet_address * get_other_side();		// адрес инициатора поступивщего соединения
		int set_blocking(sock_blocking b);		// устанавливает/снимает блокировку
};
#endif
