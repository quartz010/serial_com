#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#define SPEED B9600	//remember 'B' is additional
#define DATA_BITS CS8	//chose CS7 or CS8
#define STOP_BITS 1		//set the stop_bits '1' or '2'
#define PARITY N		//chose	from 'N' 'E' 'O' 'S'

#define PATH "/dev/ttyS1"
/**
 * [set_com_config description]
 * @Author:  Krylin
 * @DateTime 2016-12-18T21:00:59+080
 * @param    fd                      the com's node file
 * @param    config                  the pointer of old_config
 * @return                           0 if successed
 */
int open_com()
{
	int fd = open(PATH, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0) {
		perror("open com_port error:");
		exit(-1);
	}

	if (fcntl(fd, F_SETFL, 0) < 0) {
		perror("fcntl F_SETFL:");
		exit(-1);
	}

	if (0 == isatty(fd)){
		perror("it's not a com !!:");
		exit(-1);
	}
	return fd;
}

int set_com_config(int fd, struct termios *config)
{
	config->c_cflag &= ~CSIZE;

#if 1 == STOP_BITS
	config->c_cflag &= ~CSTOPB;
#elif 2 == STOP_BITS
	config->c_cflag |= CSTOPB;
#endif

#if N == PARITY
	config->c_cflag &= ~PARENB;
	config->c_cflag &= ~INPCK;
#endif

	config->c_cflag |= DATA_BITS;	//
	cfsetispeed(config, SPEED);		//
	cfsetospeed(config, SPEED);

	config->c_cc[VTIME] = 0;
	config->c_cc[VMIN] = 1;

	tcflush(fd ,TCIFLUSH);			//flush the buffer

	if ((tcsetattr(fd, TCSANOW, config) != 0)) {
		perror("fd set:");
		exit (-1);
	}


}
/**
 * [copy_from_old description]
 * @Author:  Krylin
 * @DateTime 2016-12-18T21:13:32+080
 * @param    fd                      the opened com num
 * @param    new_config              the pointer of new_config
 * @param    old_config              the pointer of old_config
 */
void copy_from_old(int fd,
	struct termios *new_config,
	struct termios *old_config)
{
	if (tcgetattr(fd, old_config) != 0)
	{
		perror("get old_config:");
		exit(-1);
	}

	*new_config = *old_config;

}

int main(int argc, char const *argv[])
{
	char write_buf[64];
	char write_buf_cmd_1[] = {0xf0};
	char write_buf_cmd_2[] = {0x00};
	char read_duf[64];
	struct termios new_config, old_config;
	int fd = open_com();

	memset(write_buf, 0, sizeof(write_buf));
	copy_from_old(fd, &new_config, &old_config);	//copy the old setting
	cfmakeraw(&new_config);

	if (0 != set_com_config(fd, &new_config)){		//set config
		perror("com init error!");
		exit(-1);
	}
#if 0
	for (size_t i = 0; i < 10; i++) {				//com_read
		read(fd, read_duf, sizeof(read_duf));
		printf("%s\n", read_duf);
		/* code */
	}
#endif

#if 0
	if (argc > 1) {									//com_write_str
		strcpy(write_buf, argv[1]);

		strcat(write_buf, "\n\r");
	}else if (1 == argc) {
		gets(write_buf);
		// scanf("%s", write_buf);
		strcat(write_buf, "\n\r");
	}

	write(fd, write_buf, sizeof(write_buf));
#endif
	// strtol()
	while (1) {
		write(fd, write_buf_cmd_1, sizeof(write_buf_cmd_1));
		sleep(1);
		write(fd, write_buf_cmd_2, sizeof(write_buf_cmd_2));
		sleep(1);
	}





	if (0 != set_com_config(fd, &new_config)){		//set config
		perror("com restore	 error!");
		exit(-1);
	}
	close(fd);

    return 0;
}
