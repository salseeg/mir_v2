#include <linux/kernel.h>	/*	k*			*/
#include <linux/module.h>  	/* 	MODULE_*	*/
#include <linux/fs.h>		/*	struct file	*/
#include <linux/wrapper.h>
#include <asm/uaccess.h>	/* get_user put_user */
#include <linux/ioport.h>	/* request_region */
#include <linux/wait.h>		/* sleep_on	*/
#include <asm/io.h>			/* inb outb */
#include <linux/time.h>		/* gettimeofday */
#include <linux/sched.h>
#include <linux/tqueue.h>
#include <linux/interrupt.h>	/*	IMMEDIATE_BH	*/
#include <linux/version.h>

/*	#include "types.h"	*/
/*	#include "constants.h"	*/
/*#include "driver.h"*/
#include <mir/interface/driver.h>

/*#define DEBUG*/

MODULE_AUTHOR("(no)");
MODULE_DESCRIPTION("Driver for MIR Station");
MODULE_LICENSE("GPL");

MODULE_PARM(in_port, "i");
MODULE_PARM(n_in, "i");
MODULE_PARM(out_port, "i");
MODULE_PARM(n_out, "i");

MODULE_PARM_DESC(in_port, "Порт внутренних линий");
MODULE_PARM_DESC(n_in, "Количество внутренних линий");
MODULE_PARM_DESC(out_port, "Порт внутренних линий");
MODULE_PARM_DESC(n_out, "Количество внешних линий");

static unsigned int in_port;
static unsigned int n_in;
static unsigned int out_port;
static unsigned int n_out;

extern void *kmalloc(size_t, int);
extern void kfree(const void *);
void wrap(char *s)
{ 	
		printk("mir: %s\n",s);
		return;
}




#define MIR_MAJOR  254 	/* резервируем этот номер для нашего устройства */
#define MIR_NAME "MIR Station"
#define MIR_IRQ 7				/* IRQ которое мы используем */
#define BASE_PORT	0x150		/* начальный/базовый порт */
#define EXT_PORT	16			/* количество портов вобщем */
#define ONOFF_PORT 0x153		/* порт для включения/выключения прерывания */
#define irq_on outb(1,ONOFF_PORT);
#define irq_off outb(0,ONOFF_PORT);
#define INNER_PORT 	0x150		/* порт внутрених линий */
#define OUTER_PORT	0x151		/* порт внешних линий */
#define TIME_XXX	13000		/* время помехи */

#ifdef DEBUG
static struct timeval t1;
static long ff = 0;
#endif

/* Буфер хранения портов */
static struct photo {
		char inner;				/*	размер переменных должен соответствовать */ 
		char outer;				/*  количеству портов */
}ph;

static struct xxx_array {
		unsigned long *inner;
		unsigned long *outer;
}xxx;		/* время помехи */

static struct timeval perv_t,cur_t;


/* состояния абонентов */
static struct bit_history  *inner; 
static struct bit_history  *outer;
static unsigned long delta_time;

static wait_queue_head_t base;		

/* хранение состояния различных ключей */
typedef unsigned char uchar;
static uchar buf_kna;
static uchar buf_pv;
static uchar buf_kv;
static uchar buf_ik;
static uchar buf_sk;

static char opened = 0;

static int mir_open (struct inode *in, struct file *f)
{
#ifdef DEBUG
		wrap ("Кто-то стучится # Someone knocking (open)");
#endif
		if (opened) {
				return -EBUSY;
		}
		else {
				opened = 1;
				MOD_INC_USE_COUNT;
				init_waitqueue_head(&base); /* инициализация для 2.4.x */
				return 0;
		}
}

static ssize_t mir_read (struct file *file, char *buf, size_t len, loff_t *offset)
{
		struct read_line_states *p = (struct read_line_states *)buf;		
		char *arr;
		enum LT line_type;
		char * bb;
		long i;
#ifdef DEBUG
		wrap ("Операция чтения # Read operation");
#endif
		get_user((struct bit_history *)arr,&(p->bits));
		get_user(line_type,&(p->line_type));

		/* заполнение структуры */
		put_user(delta_time,&(p->delta_time));
		if (line_type == LT_inner){
				bb = (char *)inner;
				for (i = 0;i< n_in * sizeof(struct bit_history); i++){
						put_user(bb[i],&(arr[i]));
				}
		} else {
				bb = (char *)outer;
				for (i = 0;i< n_out * sizeof(struct bit_history); i++){
						put_user(bb[i],&(arr[i]));
				}
		}



		/*
		   do{
		   len --;
		   put_user(p[len],&(buf[len]));
		   } while(len > 0);
		   */
		return 0;

}

#define KNA_ADDR 0x151
#define PV_ADDR	0x150
#define	KV_ADDR 0x154
#define IK_ADDR	0x155
#define SK_ADDR	0x156

static ssize_t mir_write (struct file *f, const char *buf, size_t len, loff_t *offset)
{
		unsigned long i;
		struct write_command  command;
		char * tmp = (char*) &command;
		char  data;
		unsigned short port;

		for (i = 0; i < sizeof ( command); i++){
				get_user( tmp[i], &(buf[i]));
		}

		switch (command.type) {
				case CT_init:
						/* Инициализация структур */
						do_gettimeofday(&perv_t);
						i = n_in;
						do{
								--i;
								inner[i].fixed_time = 0;
								inner[i].flow_state = 0;
								inner[i].flow_time = 0;
								xxx.inner[i] = 0;
						}while (i > 0);
						i = n_out;
						do{
								--i;
								outer[i].fixed_time = 0;
								outer[i].flow_state = 0;
								outer[i].flow_time = 0;
								xxx.outer[i]  = 0;
						}while (i > 0);
						/* обнуление регистров ключей прогр и аппаратных */
						outb(1, KNA_ADDR);
						buf_kna = 1;
						outb(0,PV_ADDR);
						buf_pv	= 0;
						outb(0,KV_ADDR);
						buf_kv = 0;
						outb(0,IK_ADDR);
						buf_ik = 0;
						outb(0,SK_ADDR);
						buf_sk = 0;
#ifdef DEBUG
						wrap("Включаем прерывание # Enable IRQ");
#endif
						irq_on;
						break;
				case CT_matrix:
#define MATR_ADDR	0x152		// регистр матрицы
#define REG_STROBE	0x153		// регистр строба
#define STROBE 	outb(1, REG_STROBE)
						// проключить точку коммутации
						{unsigned char c = (command.state << 7) | (command.cord_number << 4) \
										| command.line_number;
						outb(c, MATR_ADDR);
						STROBE;
						//printk("matrix %d %d %d = %x\n",command.state, command.cord_number, command.line_number, c);
						}break;
				case CT_sleep:
						interruptible_sleep_on(&base);
						break;
				default:
						switch (command.type) {
								case CT_KNA:
										// software invertor
										if (command.state == on) {
												command.state = off;
										} else if (command.state == off) {
												command.state = on;
										} else return -1;
										// end invertor
										port = KNA_ADDR;
										data = buf_kna;
										break;
								case CT_PV:
										port = PV_ADDR;
										data = buf_pv;
										break;
								case CT_KV:
										port = KV_ADDR;
										data = buf_kv;
										break;
								case CT_IK:
										port = IK_ADDR;
										data = buf_ik;
										break;
								case CT_SK:
										port = SK_ADDR;
										data = buf_sk;
										break;
								default:
										return -1;
						}
						switch (command.state){
								case off:
										data &= ~(1 << command.line_number);
										break;
								case on:
										data |= (1 << command.line_number);
										break;
								default:
										return -1;
						}
						//	refresh buffers by m0e
						switch (command.type) {
								case CT_KNA:
										buf_kna = data;
										break;
								case CT_PV:
										buf_pv = data;
										break;
								case CT_KV:
										buf_kv = data;
										break;
								case CT_IK:
										buf_ik = data;
										break;
								case CT_SK:
										buf_sk = data;
										break;
								default:
						}
						outb(data, port);

		}



#ifdef DEBUG
		do_gettimeofday(&t1);
		t1.tv_usec = t1.tv_usec - cur_t.tv_usec;
		if (ff > 100) {
				ff = 0;
				printk("%ld\n",t1.tv_usec);
		}
		else ff++;
#endif

		return 0;
}

static int mir_close (struct inode *in, struct file *fi)
{
		irq_off
				MOD_DEC_USE_COUNT;
		opened = 0;
		return 0;
}

static void bottom_half (void *p)
{
		unsigned char mask,tmp;
		unsigned long delta;
		unsigned long i;
#ifdef DEBUG
		wrap("Bottom half");
#endif
		delta = (unsigned long)( (cur_t.tv_sec-perv_t.tv_sec)*1000000+(cur_t.tv_usec - perv_t.tv_usec) );
		delta_time = delta;
		i = n_in; mask = 0x80;			/* маска зависит от количества */
		do{
				i--; tmp = ((ph.inner & mask) != 0);
				xxx.inner[i] += delta;
				if ((tmp && !inner[i].flow_state) || (!tmp && inner[i].flow_state)){
						if (xxx.inner[i] > TIME_XXX){
								inner[i].fixed_time = inner[i].flow_time;
								inner[i].flow_state = tmp;
								inner[i].flow_time = xxx.inner[i];
								xxx.inner[i] = 0;
						}
				}
				else{
						inner[i].flow_time += xxx.inner[i];
						if (inner[i].flow_time > 30*60*1000000){
								inner[i].flow_time -= 10*60*1000000;
						}
						xxx.inner[i] = 0;
				}
				mask >>= 1;
		}while (i > 0);
		/* без процедур - быстрее */

		i = n_out; mask = 0x08;		/* маска зависит от количества */
		do{
				i--; tmp = ((ph.outer  & mask) != 0);
				xxx.outer[i] += delta;
				/* наличие питания на линии */
				outer[i].powered = ((ph.outer >> 4) & mask) != 0;
				if ((tmp && !outer[i].flow_state) || (!tmp && outer[i].flow_state)){
						if (xxx.outer[i] > TIME_XXX){
								outer[i].fixed_time = outer[i].flow_time;
								outer[i].flow_state = tmp;
								outer[i].flow_time = xxx.outer[i];
								xxx.outer[i] = 0;
						}
				}
				else{
						outer[i].flow_time += xxx.outer[i];
						if (outer[i].flow_time > 30*60*1000000){
								outer[i].flow_time -= 10*60*1000000;
						}
						xxx.outer[i] = 0;
				}
				mask >>= 1;
		}while (i > 0);
		perv_t.tv_sec = cur_t.tv_sec;
		perv_t.tv_usec = cur_t.tv_usec;
		wake_up(&base);
		return;
}

/* Bottom half для обработки прерывания */
static struct tq_struct my_bh = {
		{NULL,NULL},
		0, 
		&bottom_half, 
		NULL
};

/*	spinlock_t tqueue_lock = {};	*/

static void irq_handler (int irq, void * dev_id, struct pt_regs *regs)
{
#ifdef DEBUG
		wrap("Прерывание # Interrupt");
#endif
		/* первая инструкция - чтение !!!!! */
		/* фотографируем порты*/
		ph.inner = inb(in_port);	 
		ph.outer = inb(out_port);

		if (ph.outer & 0xf) wrap ("There is 1");/*  ????????*/

		/* берем текущее время */
		do_gettimeofday(&cur_t);

		/* ставим в очередь  bottom_half */
		queue_task(&my_bh, &tq_immediate);

		mark_bh(IMMEDIATE_BH);
		return;
}

static struct file_operations fops = {
owner:          THIS_MODULE,	/* for 2.4.x	*/
read:		&mir_read,	/* read 	*/
write:		&mir_write,	/* write 	*/
open:		&mir_open,	/* open 	*/
release:	&mir_close, /* release 	*/
};

static int __init init_module()
{
		if (register_chrdev(MIR_MAJOR,MIR_NAME,&fops) < 0){
				printk("mir: Unable to register device\n");
				return -1;
		}
		if (request_irq(MIR_IRQ,&irq_handler,SA_INTERRUPT,MIR_NAME,0) < 0){
				printk("mir: IRQ %d is busy\n",MIR_IRQ);
				return -1;
		}
		if (check_region(BASE_PORT,EXT_PORT) < 0){
				printk("mir: Requested port are busy");
				return -1;
		}

		inner = (struct bit_history * ) kmalloc(n_in * sizeof(*inner),GFP_KERNEL);
		if (inner == NULL){
				wrap("No memory for inner");
				return -1;
		}
		xxx.inner = (unsigned long *) kmalloc(n_in * sizeof (long),GFP_KERNEL);
		if (xxx.inner == NULL) {
				wrap("No memory for xxx.inner");
				return -1;
		}

		outer = (struct bit_history * ) kmalloc(n_out * sizeof(*outer),GFP_KERNEL);
		if (outer == NULL){
				wrap("No memory for outer");
				return -1;
		}
		xxx.outer = (unsigned long *) kmalloc(n_out * sizeof (long),GFP_KERNEL);
		if (outer == NULL){
				wrap("No memory for outer");
				return -1;
		}

		printk("Installed [IN %d:%d; OUT %d:%d]\n",in_port,n_in,out_port,n_out);
		request_region(BASE_PORT,EXT_PORT,MIR_NAME);
		return 0;
}

static void __exit cleanup_module()
{
		release_region(BASE_PORT,EXT_PORT);
		kfree (inner);
		kfree (outer);
		free_irq(MIR_IRQ,NULL);
		unregister_chrdev(MIR_MAJOR,MIR_NAME);
}
// vim:tw=4:ts=4:nowrap:
