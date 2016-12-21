#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SPEED B9600	//remember 'B' is additional
#define DATA_BITS CS8	//chose CS7 or CS8
#define STOP_BITS 1		//set the stop_bits '1' or '2'
#define PARITY N		//chose	from 'N' 'E' 'O' 'S'
/**
 * [set_com_config description]
 * @Author:  Krylin
 * @DateTime 2016-12-18T21:00:59+080
 * @param    fd                      the com's node file
 * @param    config                  the pointer of old_config
 * @return                           0 if successed
 */
int open_com(const char *com_path)
{
	int sfd = open(com_path, O_RDWR | O_NOCTTY | O_NDELAY);
	if (sfd < 0) {
		perror("open com_port error:");
		exit(-1);
	}

	if (fcntl(sfd, F_SETFL, 0) < 0) {
		perror("fcntl F_SETFL:");
		exit(-1);
	}

	if (0 == isatty(sfd)){
		perror("it's not a com !!:");
		exit(-1);
	}
	return sfd;
}

int set_com_config(int sfd, struct termios *config)
{

	cfmakeraw(config);

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

	tcflush(sfd ,TCIFLUSH);			//flush the buffer

	if ((tcsetattr(sfd, TCSANOW, config) != 0)) {
		perror("sfd set:");
		exit (-1);
	}

}

void copy_from_old(int sfd,
   struct termios *new_config,
   struct termios *old_config)
{
   if (tcgetattr(sfd, old_config) != 0)
   {
	   perror("get old_config:");
	   exit(-1);
   }

   *new_config = *old_config;

}

int send_byte_com(int sfd, char data_byte)
{
	char write_buf_cmd[1];
	write_buf_cmd[0] = data_byte;

	write(sfd, write_buf_cmd, sizeof(write_buf_cmd));

}

int read_byte_com(int sfd)
{
	char read_buf_cmd[1];
	char data_byte;

	read(sfd, read_buf_cmd, sizeof(read_buf_cmd));
	data_byte = read_buf_cmd[0];

	return data_byte;
}
