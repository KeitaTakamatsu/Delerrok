//
//  udp.h
//  Hachidori
//
//  Created by 高松 圭太 on 12/02/22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Hachidori_udp_h
#define Hachidori_udp_h
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void udp_send(char* buf, int size);
int udp_receive(char* buf, int size);
void udp_send_original(char* buf, int size, char* ip, int port);
#endif
