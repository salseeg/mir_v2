
#include <pth.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int mir_fd;
static unsigned int line_states;

//установка бита в единицу
void set1( char c)
{
	line_states |= 1<<c;
}
//установка бита в ноль
void set0( char c)
{
	line_states &= ~(1<<c);
}
	
//эмулятор прерываний
void * int_emul(void *_arg)
{
	for (;;)
	{
		pth_sleep(1);
		ioctl(mir_fd,line_states);
	}
}

//возврат управления scheduler'у
int pth_y( ClientData x, Tcl_Interp *a, int argc, char * argv[])
{
	pth_yield(NULL);
	return TCL_OK;
}

int pup ( ClientData x, Tcl_Interp *a, int argc, char *argv[])
{
	char c = atoi(argv[1]);
	set1(c);
	return TCL_OK;
}
int pdown ( ClientData x, Tcl_Interp *a, int argc, char *argv[])
{
	char c = atoi(argv[1]);
	set0(c);
	return TCL_OK;
}

struct make_call_t
{
	char * s;
	unsigned char n;
};

void * make_call(void * arg)
{
	struct make_call_t * data = arg;
	int i;
	for (i=0; i< strlen(data->s);i++)
	{
		int j=data->s[i]-'0';
		unsigned char n = data->n;
		printf("[%c]\n",data->s[i]);
		//если ноль набор 10
		if (j == 0) j=10;
		//уровни для набора цифр могут
		//различаться !!!!
		set1(n);
		pth_usleep(300000);
		while (j-- > 0){
			set0(n);
			pth_usleep( 60000);
			set1(n);
			pth_usleep( 40000);
		}
		set1(n);
		pth_usleep( 300000);
	}
	return;
		
}

int pcall ( ClientData x, Tcl_Interp *a, int argc, char *argv[])
{
	struct make_call_t * data = (struct make_call_t *) malloc(sizeof(*data));
	printf("Get %s and %s\n",argv[1],argv[2]);
	data->n = atoi(argv[1]);
	data->s = argv[2];
	pth_spawn(PTH_ATTR_DEFAULT, make_call, data);
	return TCL_OK;
}

void My_TclExt()
{
	mir_fd = open("/dev/mir", O_RDWR);
	if (mir_fd < 0)
	{
		printf("Error open /dev/mir\n");
		return;
	}
	//вводим в игру нити
	pth_init();
	//включаем препывания
	pth_spawn(PTH_ATTR_DEFAULT, int_emul, NULL);
	
	
	
	
}
