#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <signal.h>


char * music__raw_filename = "/usr/share/mir/mir_music";

/* 
 * в случае ошибки функция возвратит -1 
 * обработка результата возлагается на основную программу 
 */
#define LEVEL 100

char * mem;

void term_hand(int){
	delete mem;
	exit(0);
}

int init_music(){
	int dev;
	struct stat inf;
	size_t s;
	int speed = 22050;  /* raw files must be in that format */
	int bits = 16; 
	int stereo = 0;
	int volume_level;

	/* mir_music - symbol link to real *.raw file */
	dev = open(music__raw_filename,O_RDONLY);
	if (dev < 0) {
		perror("init_music");
		return -1;
	}
	fstat(dev,&inf); 
	s = inf.st_size;
	mem = new char[s];
	read(dev, mem, s);
	close(dev);
		
	/* configuring mixer */
	dev = open("/dev/mixer",O_WRONLY);
	if (dev < 0){
		printf("init_music :  не могу открыть mixer\n");
	}
	volume_level = (LEVEL << 8)|LEVEL;
	ioctl(dev,SOUND_MIXER_WRITE_PCM,&volume_level);
	ioctl(dev,SOUND_MIXER_WRITE_VOLUME,&volume_level);
	close(dev);

	dev = open("/dev/dsp",O_WRONLY);
	if (dev == -1) {
		perror("init_music");
		delete mem;
		return -1;
	}
	ioctl(dev, SNDCTL_DSP_SAMPLESIZE, &bits);
	ioctl(dev, SNDCTL_DSP_STEREO, &stereo);
	ioctl(dev, SNDCTL_DSP_SPEED, &speed);
	if (fork() == 0){
		signal(SIGTERM,&term_hand);
		for(;;){
			write(dev,mem,s);
		}
	} else {
		return 0;
	}
}
