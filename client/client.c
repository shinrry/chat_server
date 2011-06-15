/* Copyright (C) 
 * 2010 - He Anda
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>

#include "client.h"

/**
 * @brief receiving data from server
 *
 * @param active_socket current socket
 */
void clientd(int active_socket)
{
    char buf[BUF];
    int len;
    while ((len = recv(active_socket, buf, sizeof(buf), 0)) > 0)
    {
        write(1, buf, len); /*showing the message from server*/ 
    }
}

int connect_init()
{
    char hostname[] = "localhost";
    struct hostent *hp;
    struct sockaddr_in name;
    int ns, len;

    /*get host name.*/
    if (gethostname(hostname, sizeof(hostname)) < 0) {
        perror("gethostname");
        exit(1);
    }

    /*look up our host's network address.*/
    if ((hp = gethostbyname(hostname)) == NULL) {
        fprintf(stderr, "unknown host: %s.\n", hostname);
        exit(1);
    }

    /*create a socket in the INET domain.*/
    if ((ns = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    /*create the address of the server.*/
    memset(&name, 0, sizeof(struct sockaddr_in));

    name.sin_family = AF_INET;
    name.sin_port = htons(PORT);
    memcpy(&name.sin_addr, hp->h_addr_list[0], hp->h_length);
    len = sizeof(struct sockaddr_in);

    /*connect to the server.*/
    if (connect(ns, (struct sockaddr *) &name, len) < 0) {
        perror("connect");
        exit(1);
    }

    return ns;
}

int main(int argc, const char *argv[])
{
    int n, active_socket, len;
    char buf[BUF], message[BUF];
    int cnt = 0;
    pthread_t pid;
    char tmp;
    extern int fd;

    active_socket = connect_init();
    tmp = login(active_socket, argv[1], argv[2]);

    memset(message, 0, sizeof(message));

    memset(buf, 0, sizeof(buf));
    while ((n = read(0, buf, sizeof(buf))) > 0)
    {
        if (send(active_socket, buf, n, 0) < 0) {
            perror("send");
            exit(1);
        }
            
        memset(buf, 0, sizeof(buf));
    }
}


char login(int socket, const char username[], const char password[])
{
    char message[MSG];
    pthread_t pid;

    encap_msg(message, LOGIN, username, password);
    send(socket, message, strlen(message), 0);
    if (recv(socket, message, sizeof(message), 0) > 0) {
        if (LOGIN_YES == message[0]) {
            pthread_create(&pid, NULL, (void *)clientd, (void *)socket);
        }
        return message[0];
    }
}

void talk(int socket, const char target_username[], const char content[])
{
    char message[MSG];
    encap_msg(message, TALK, target_username, content);
    send(socket, message, strlen(message), 0);
}

char reg(int socket, const char username[], const char password[])
{
    char message[MSG];

    encap_msg(message, REGISTER, username, password);
    send(socket, message, strlen(message), 0);
    if (recv(socket, message, sizeof(message), 0) > 0) {
        return message[0];
    }
}
