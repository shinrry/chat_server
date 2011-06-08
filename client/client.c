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
 * @brief set the message to echo
 *
 * @param flag the signal sent from server or the client itself
 * @param message[] the result string
 */
void set_echo(char flag, char message[])
{
    switch (flag)
    {
        case OFFLINE:
            strcpy(message, "*** Logged in successful.\n");
            break;
        case NOT_EXIST:
            strcpy(message, "*** User not found.\n");
            break;
        case WRONG_PASSWORD:
            strcpy(message, "*** Incorrect password for this user.\n");
            break;
        case ONLINE:
            strcpy(message, "*** This user has already logged in.\n");
            break;
        case TOO_LONG:
            strcpy(message, "*** The length of username is limited to 20 characters.\n");
            break;
        case TAR_OFFLINE:
            strcpy(message, "*** User not online now.\n");
            break;
        case FAIL_SEND:
            strcpy(message, "*** Unable to send the specific file.\n");
            break;
        case FILE_TOO_LONG:
            strcpy(message, "Sorry, file's size is limited to 10KB.\n");
            break;
        case ACCPET:
            strcpy(message, "You are about to receive a file.\n");
            break;
        case ACCEPTED:
            strcpy(message, "*** You have received a file successfully.\n");
            break;
        case SENT:
            strcpy(message, "*** You have sent the file successfully.\n");
            break;
        default:
            strcpy(message, "*** Sorry, an unknown error occured.\n");
            break;
    }
}

/**
 * @brief receives a file
 *
 * @param active_socket current socket
 *
 * @return error code
 */
char rcv_file(int active_socket)
{
    char file_buf[FILE_SIZE], path[PATH];
    int len, fd;

    memset(file_buf, 0, sizeof(file_buf));

    set_echo(ACCPET, file_buf);
    write(1, file_buf, strlen(file_buf));

    if ((len = recv(active_socket, path, sizeof(path), 0)) > 0) /*get path from server*/ 
        ;
    if ((len = recv(active_socket, file_buf, sizeof(file_buf), 0)) > 0)
        ;

    if (-1 == (fd = creat(path, 0666))) /*create a new file*/
    {
        perror("creat");
        exit(1);
    }
    write(fd, file_buf, len);
    return ACCEPTED;
}

/**
 * @brief deamon of client
 *
 * @param active_socket current socket
 */
void clientd(int active_socket)
{
    char buf[BUF];
    int len;
    while ((len = recv(active_socket, buf, sizeof(buf), 0)) > 0)
    {
        if (1 == len) /*receiving a 1-byte signal from server*/ 
        {
            if (FILE_RCV == buf[0]) /*about to receive a file*/ 
            {
                buf[0] = rcv_file(active_socket);
            }
            set_echo(buf[0], buf);
            len = strlen(buf);
        }
        write(1, buf, len); /*showing the message from server*/ 
    }
}

int main()
{
    int n, active_socket, len;
    char buf[BUF], hostname[HOST_LEN], message[BUF];
    struct hostent *hp;
    struct sockaddr_in name;
    int cnt = 0;
    pthread_t pid;
    char tmp;
    extern int fd;

    memset(message, 0, sizeof(message));
    while(scanf(" !login %s", hostname) <= 0)
    {
        printf("Invalid command.\n");
        printf("Usage: !login [hostname | ip_address].\n");
    }

    /*get host name.*/
    if (gethostname(hostname, sizeof(hostname)) < 0)
    {
        perror("gethostname");
        exit(1);
    }

    /*look up our host's network address.*/
    if ((hp = gethostbyname(hostname)) == NULL)
    {
        fprintf(stderr, "unknown host: %s.\n", hostname);
        exit(1);
    }

    /*create a socket in the INET domain.*/
    if ((active_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
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
    if (connect(active_socket, (struct sockaddr *) &name, len) < 0)
    {
        perror("connect");
        exit(1);
    }

    strcpy(message, "username: ");
    write(1, message, strlen(message));
    memset(buf, 0, sizeof(buf));
    while ((n = read(0, buf, sizeof(buf))) > 0)
    {
        if (0 == cnt) /*send username*/ 
        {
            cnt++;
            if (send(active_socket, buf, n, 0) < 0)
            {
                perror("send");
                exit(1);
            }
            strcpy(message, "password: ");
            write(1, message, strlen(message));
        }
        else if (1 == cnt) /*send password*/ 
        {
            cnt++;
            if (send(active_socket, buf, n, 0) < 0)
            {
                perror("send");
                exit(1);
            }

            /*wait for validation info*/
            if ((len = recv(active_socket, buf, 1, 0)) > 0)
            {
                set_echo(buf[0], message);
                write(1, message, strlen(message));
                if (buf[0] != OFFLINE) /*failed to log in*/ 
                {
                    exit(1);
                }
                /*Create a thread*/
                if (pthread_create(&pid, NULL, (void *)clientd, (void *)active_socket) < 0)
                {
                    perror("thread");
                    exit(1);
                }
            }
        }
        else /*send commands*/ 
        {
            if (send(active_socket, buf, n, 0) < 0)
            {
                perror("send");
                exit(1);
            }
            if (0 == strncmp("!file", buf, 5)) /*about to send a file*/ 
            {
                set_echo(send_file(buf, active_socket), buf);
                write(1, buf, strlen(buf));
            }
            else if (0 == strncmp("!logoff\n", buf, n)) /*log off*/ 
            {
                exit(0);
            }
            
        }
        memset(buf, 0, sizeof(buf));
    }
}

/**
 * @brief send a file
 *
 * @param buf[] command, carrying target username and path
 * @param socket current socket
 *
 * @return error code
 */
char send_file(const char buf[], int socket)
{
    char path[PATH];
    struct stat st;
    off_t fsize;
    ssize_t flen;
    char file_buf[FILE_SIZE];
    extern int fd;

    memset(path, 0, sizeof(path));
    extract_path(path, buf);
    if (-1 == (fd = open(path, O_RDONLY))) /*open the file*/ 
    {
        perror("open file");
        exit(1);
    }
    if (-1 == fstat(fd, &st))
    {
        perror("stat");
        exit(1);
    }
    fsize = st.st_size;
    if (fsize >= FILE_SIZE) /*file's size too large*/ 
    {
        return FILE_TOO_LONG;
    }

    flen = read(fd, file_buf, FILE_SIZE);
    close(fd); /*close the file*/ 
    if (send(socket, file_buf, flen, 0) < 0) /*send the file*/ 
    {
        return FAIL_SEND;
    }
    else
    {
        return SENT;
    }
}

