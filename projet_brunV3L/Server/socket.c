#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include "socket.h"
int creerSocket(int port, int type){
    static struct sockaddr_in adresse;
    int sock;
    sock = socket(AF_INET, type, 0);
    if(sock==-1) return -1;
    bzero((char *)&adresse, sizeof(adresse));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(port);
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(sock,(struct sockaddr*) &adresse, sizeof(adresse))==-1)
        return -1;

    return sock;
}

int creerSocketUDP(int port){return(creerSocket(port, SOCK_DGRAM));};

int creerSocketTCP(int port){return(creerSocket(port, SOCK_STREAM));};