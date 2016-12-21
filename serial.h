#ifndef _SERIAL_H_
#define _SERIAL_H_

int open_com(const char *com_path);

int set_com_config(int sfd, struct termios *config);

void copy_from_old(int sfd,
   struct termios *new_config,
   struct termios *old_config);

int send_byte_com(int sfd, char data_byte);

int read_byte_com(int sfd);


#endif
