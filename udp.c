//
//  udp.c
//  Hachidori
//
//  Created by 高松 圭太 on 12/02/22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "udp.h"

#define PORT 2002
#define TARGET_IP "192.168.2.5"
// #define TARGET_IP "192.168.2.14"

int udp_receive(char* buf, int size){    
    int sock;
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    int s = (int)recv(sock, buf, size, 0);    
    close(sock);
    
    return s;
}

void udp_send(char* buf, int size){
    int sock;
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(TARGET_IP);
    
    sendto(sock, buf, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    
    close(sock);
}

void udp_send_original(char* buf, int size, char* ip, int port){
    int sock;
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    sendto(sock, buf, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    
    close(sock);
}