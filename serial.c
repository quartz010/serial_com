#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#define SPEED B115200	//remember 'B' is additional
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
int open_com()
{
	fd = open(PATH, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0) {
		perror("open com_port error:");
		exit(-1);
	}

	if (fcntl(fd, F_SETFL, 0) < 0) {
		perror("fcntl F_SETFL:");
		exit(-1);
	}

	if (0 == isatty(STDIN_FILENO)){
		perror("it's not a com !!");
		exit(-1);
	}

	return fd;
}

int set_com_config(int fd, struct termios *config)
{
	config->c_cflag &= ~CSIZE;

#if 1 == STOP_BITS
	config->c_cflag &= ~CSTOPB;
#elseif 2 == STOP_BITS
	config->c_cflag |= CSTOPB;
#endif

#if N == PARITY
	config->c_cflag &= ~PARENB;
	config->c_cflag &= ~INPCK;
#endif

	config->c_cflag |= DATA_BITS	//
	cfsetispeed(config, SPEED);		//
	cfsetospeed(config, SPEED);

	config->c_cc[VTIME] = 0;
	config->c_cc[VMIN] = 1;

	tcflush(fd ,TCIFLUSH);			//flush the buffer

	if ((tcsetattr(fd, TCSANOW, config) != 0) {
		perror("fd set:")
		return -1;
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
		perror("ger old_config:");
		exit(-1);
	}

	*new_config = *old_config;

}

int main(int argc, char const *argv[])
{

    struct termios new_config, old_config;
	int fd = open_com();

	copy_from_old(fd, &new_config, &old_config);
	cfmakeraw(&new_config);

	if (0 != set_com_config(int fd, &new_config)){
		printf("%s\n", "com init error!");
		exit(-1);
	}



    return 0;
}
