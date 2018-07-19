#ifndef SOCKS_H
#define SOCKS_H

#include "mstd.h"

//returns socket on success
//  on failure returns 0
sock_h socks_connect(char *dest_ip, char *dest_port, char *ip, char *port, char *usrname, char *password);

//returns socket on success
//  on failure returns 0
sock_h tor_connect(char *ip, char *port);

#endif
