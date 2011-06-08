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

#define PORT 8081
#define BUF 1024
#define USERNAME_LEN 21
#define PASSWORD_LEN 21
#define PATH 50
#define MSG 50
#define FILE_SIZE 10240 

#define OFFLINE 'f'
#define NOT_EXIST 'n'
#define WRONG_PASSWORD 'w'
#define ONLINE 'o'
#define TOO_LONG 'l'
#define TAR_OFFLINE 't'
#define FILE_TOO_LONG 'e'
#define FAIL_SEND 's'
#define ACCPET 'A'
#define ACCEPTED 'y'
#define SENT 'S'
#define FILE_RCV 'r'

#define ALREADY_EXIST 'a'
#define REG 'R'

void extract_path(char path[], const char buf[]);
void extract_username(char username[], const char buf[]);
