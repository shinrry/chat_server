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

#define USER_CNT 6
#define JOIN 0
#define DEPARTURE 1

/*
 *global variables
 */

struct user 
{
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
	char status;
	int socket;
} ls[] = {"Haibara", "the", OFFLINE, -1,
	"Conan", "quick", OFFLINE, -1,
	"Sonoko", "brown", OFFLINE, -1,
	"Kogoro", "fox", OFFLINE, -1,
	"Vermouth", "jumps", OFFLINE, -1,
	"Camel", "over", OFFLINE, -1};

struct user * find_by_username(const char username[]);
char validate(struct user *p, char password[], int ns);
void logoff(struct user *p);
void list(char buf[]);
void broadcast(int flag, struct user *p);
void serverd(int ns);
char trans_file(char buf[], int socket_sender, char src[]);
char talk(char buf[], const char src[]);
