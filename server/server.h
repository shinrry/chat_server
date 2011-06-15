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

#include "sockets.h"

#define USER_MAX 100

/*
 *global variables
 */

typedef struct 
{
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
	char status;
	int socket;
} user; 

user ls[USER_MAX] = {"Haibara", "the", OFFLINE, -1,
	"Conan", "quick", OFFLINE, -1,
	"Sonoko", "brown", OFFLINE, -1,
	"Kogoro", "fox", OFFLINE, -1,
	"Vermouth", "jumps", OFFLINE, -1,
	"Camel", "over", OFFLINE, -1};

int user_count = 6;

user * find_by_username(const char username[]);
void append_usr(char buf[], int ns);
void login(char buf[], int ns);
char validate(user *p, char password[], int ns);
void logoff(user *p, int socket);
void list(char buf[], int socket);
void broadcast(char flag, user *p);
void serverd(int ns);
/*char trans_file(char buf[], int socket_sender, char src[]);*/
void talk(char buf[], int socket);
