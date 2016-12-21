#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "serial.h"


#define COM_PATH "/dev/stdout"



/**
 * [copy_from_old description]
 * @Author:  Krylin
 * @DateTime 2016-12-18T21:13:32+080
 * @param    fd                      the opened com num
 * @param    new_config              the pointer of new_config
 * @param    old_config              the pointer of old_config
 */

int main(int argc, char const *argv[])
{
	char com_write_buf[64];
	char com_read_buf[64];
	struct termios new_config, old_config;

	int sfd = open_com(COM_PATH);
	memset(com_write_buf, 0, sizeof(com_write_buf));
	memset(com_read_buf, 0, sizeof(com_read_buf));

	copy_from_old(sfd, &new_config, &old_config);	//copy the old setting
	if (0 != set_com_config(sfd, &new_config)){		//set config
		perror("com config error!");
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
		strcpy(com_write_buf, argv[1]);

		strcat(com_write_buf, "\n\r");
	}else if (1 == argc) {
		gets(com_write_buf);
		// scanf("%s", com_write_buf);
		strcat(com_write_buf, "\n\r");
	}

	write(fd, com_write_buf, sizeof(com_write_buf));
#endif
	// strtol()
	// while (1) {
	// 	send_byte_com(sfd, 0x55);
	// 	sleep(1);
	// 	send_byte_com(sfd, 0x66);
	// 	sleep(1);
	// }

	


	if (0 != set_com_config(sfd, &old_config)){		//set config
		perror("com restore	 error!");
		exit(-1);
	}
	close(sfd);

    return 0;
}
