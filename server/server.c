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

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "server.h"

/**
 * @brief find the user according to username
 *
 * @param username[] target username
 *
 * @return the pointer
 */
struct user * find_by_username(const char username[])
{
	struct user *p = NULL;
	int i;
	extern struct user ls[];

	for (i = 0; i < USER_CNT; i++)
	{
		if (0 == strcmp(username, ls[i].username))
		{
			p = &(ls[i]);
			break;
		}
	}
	return p;
}

/**
 * @brief validate the user's username and password
 *
 * @param p pointer pointing to the user
 * @param password[] password read from stdin
 * @param ns socket of the user
 *
 * @return 
 */
char validate(struct user *p, char password[], int ns)
{
	if (0 == strcmp(password, p->password)) /*password correct*/ 
	{
		if (OFFLINE == p->status)
		{
			p->status = ONLINE;
			p->socket = ns;
			return OFFLINE;
		}
		else if (ONLINE == p->status)
		{
			return ONLINE;
		}
	}
	else /*password incorrect*/ 
	{
		return WRONG_PASSWORD;
	}
}

/**
 * @brief execute when a user logs off
 *
 * @param p the pointer pointing to that user
 */
void logoff(struct user *p)
{
	p->status = OFFLINE;
	p->socket = -1;
	broadcast(DEPARTURE, p);
}

/**
 * @brief list all online users
 *
 * @param buf[] result string
 */
void list(char buf[])
{
	int i, j, offset = 0;
	extern struct user ls[];

	memset(buf, 0, sizeof(buf));
	for (i = 0; i < USER_CNT; i++)
	{
		if(ONLINE == ls[i].status)
		{
			for (j = 0; j < 3; j++)
			{
				buf[offset++] = '*';
			}
			buf[offset++] = ' ';
			for (j = 0; j < strlen(ls[i].username); j++)
			{
				buf[offset++] = ls[i].username[j];
			}
			buf[offset++] = '\n';
		}
	}
}

/**
 * @brief broadcast a message each time when a user logs in or logs off
 *
 * @param flag indicating whether logging in or logging off occurs
 * @param p the pointer pointing to the user
 */
void broadcast(int flag, struct user *p)
{
	char message[MSG] = "*** ";
	int i;
	extern struct user ls[];

	strcat(message, p->username);
	switch (flag)
	{
		case JOIN:
			strcat(message, " has joined.\n");
			break;
		case DEPARTURE:
			strcat(message, " has departed.\n");
			break;
		default:
			perror("internal error");
			exit(1);
	}
	for (i = 0; i < USER_CNT; i++)
	{
		if(-1 != ls[i].socket)
		{
			if(send(ls[i].socket, message, strlen(message), 0) < 0)
			{
				perror("send");
				exit(1);
			}
		}
	}
}

/**
 * @brief daemon of server
 *
 * @param ns the user's socket
 */
void serverd(int ns)
{
	char buf[BUF], username[USERNAME_LEN], password[PASSWORD_LEN];
	int len, cnt = 0;
	struct user *p = NULL;

	memset(username, 0, sizeof(username));
	memset(password, 0, sizeof(password));

	while ((len = recv(ns, buf, sizeof(buf), 0)) > 0)
	{
		if (0 == cnt) /*receive username*/
		{
			cnt++;
			strncpy(username, buf, strlen(buf) - 1);
			p = find_by_username(username);
		}
		else if (1 == cnt) /*receive password*/
		{
			cnt++;
			strncpy(password, buf, strlen(buf) - 1);

			if (NULL == p) /*user not found*/ 
			{
				buf[0] = NOT_EXIST;
			}
			else
			{
				buf[0] = validate(p, password, ns);
			}
			if (send(ns, buf, 1, 0) < 0) /*send a single byte*/
			{
				perror("send");
				exit(1);
			}
			broadcast(JOIN, p);
		}
		else if (0 == strcmp("!logoff\n", buf)) /*receives 'logoff' command*/ 
		{
			logoff(p);
			close(ns);
			break;
		}
		else if (0 == strcmp("!list\n", buf)) /*receives 'list' command*/ 
		{
			list(buf);
			if(send(ns, buf, strlen(buf), 0) < 0)
			{
				perror("send");
				exit(1);
			}
		}
		else if (0 == strncmp(">>", buf, 2)) /*receives 'talk' command*/ 
		{
			if ((buf[0] = talk(buf, p->username)) != '\0') /*failed*/ 
			{
				if (send(ns, buf, 1, 0) < 0) /*send a single byte*/
				{
					perror("send");
					exit(1);
				}
			}
		}
		else if (0 == strncmp("!file", buf, 5)) /*receives 'file' command*/ 
		{
			if ((buf[0] = trans_file(buf, ns, username)) != '\0') /*failed*/ 
			{
				if (send(ns, buf, 1, 0) < 0) /*send a single byte*/
				{
					perror("send");
					exit(1);
				}
			}
		}
		memset(buf, 0, sizeof(buf));
	}
}

/**
 * @brief executes when needing to transmit file
 *
 * @param buf[] the file-transmitting command
 * @param socket_sender sender's socket
 * @param src[] sender's name
 *
 * @return error code
 */
char trans_file(char buf[], int socket_sender, char src[])
{
	char username[USERNAME_LEN], path[PATH];
	int len;
	struct user *receiver = NULL;
	char file_buf[FILE_SIZE];

	len = strlen(buf);
	buf[len - 1] = '\0';

	memset(path, 0, sizeof(path));
	memset(username, 0, sizeof(username));
	extract_path(path, buf);
	extract_username(username, buf);

	receiver = find_by_username(username); /*finding target user*/ 
	if (NULL == receiver)
	{
		return NOT_EXIST; /*user not found*/ 
	}
	else if(OFFLINE == receiver->status)
	{
		return TAR_OFFLINE;
	}

	buf[0] = FILE_RCV;
	if(send(receiver->socket, buf, 1, 0) < 0) /*send 1-byte signal to receiver*/ 
	{
		perror("send");
		exit(1);
	}

	if (send(receiver->socket, path, strlen(path), 0) < 0) /*send path to receiver*/ 
	{
		perror("send");
		exit(1);
	}
	if ((len = recv(socket_sender, file_buf, sizeof(file_buf), 0)) > 0) /*receive the file from sender*/ 
	{
		if (send(receiver->socket, file_buf, len, 0) < 0) /*send file to receiver*/ 
		{
			perror("send");
			exit(1);
		}
	}
	return '\0';
}

/**
 * @brief executes when users talk
 *
 * @param buf[] contents
 * @param src[] sender's username
 *
 * @return error code
 */
char talk(char buf[], const char src[])
{
	char username[USERNAME_LEN];
	int i;
	struct user *p = NULL;
	char buf_send[BUF] = "*** ";

	memset(username, 0, sizeof(username));
	for (i = 0; ; i++)
	{
		if (':' == buf[i + 2])
		{
			break;
		}
		else if (USERNAME_LEN == i) /*username too long*/ 
		{
			return TOO_LONG;
		}
		username[i] = buf[i + 2]; /*'buf' begins with '>>'*/ 
	} /*extract target username*/ 
	p = find_by_username(username); /*finding target socket*/ 
	if (NULL == p)
	{
		return NOT_EXIST; /*user not found*/ 
	}
	else if(OFFLINE == p->status)
	{
		return TAR_OFFLINE;
	} /*extract target socket*/ 

	strcat(buf_send, src);
	strcat(buf_send, buf + i + 2);
	if(send(p->socket, buf_send, strlen(buf_send), 0) < 0)
	{
		perror("send");
		exit(1);
	}
	return '\0';
}

int main()
{
	int n, main_socket, ns, len;
	struct sockaddr_in name;
	int cnt = 0;
	pthread_t pid;

	if((main_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}

	/*Create the address of the server*/
	memset(&name, 0, sizeof(struct sockaddr_in));

	name.sin_family = AF_INET;
	name.sin_port = htons(PORT);
	len = sizeof(struct sockaddr_in);

	/*Use the wildcard address*/
	n = INADDR_ANY;
	memcpy(&name.sin_addr, &n, sizeof(long));

	/*Bind the socket to the address*/
	if (bind(main_socket, (struct sockaddr *) &name, len) < 0)
	{
		perror("bind");
		exit(1);
	}

	/*Listen for connections*/
	if (listen(main_socket, 5) < 0)
	{
		perror("listen");
		exit(1);
	}

	while(1)
	{
		/*Accept a connection*/
		if ((ns = accept(main_socket, (struct sockaddr *) &name, &len)) < 0)
		{
			perror("accept");
			exit(1);
		}
		/*Create a thread*/
		if (pthread_create(&pid, NULL, (void *)serverd, (void *)ns) < 0)
		{
			perror("thread");
			exit(1);
		}
	}
	close(main_socket);
	return 0;
}