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

#include <string.h>

#include "sockets.h"

void extract_username(char username[], const char buf[])
{
    int i = 1;

    while (1) {
        if (buf[i] == ' ') {
            username[i - 1] = '\0';
            return;
        }
        username[i - 1] = buf[i];
        i++;
    }
}

void extract_second(char second[], const char buf[])
{
    int i = 0, offset = 0;

    while (buf[offset] != ' ') {
        offset++;
    }
    strcpy(second, buf + offset + 1);
}

void encap_usrname(char buf[], char op, const char username[])
{
    int len = strlen(username);

    buf[0] = op;
    strcpy(buf + 1, username);
    buf[len + 1] = '\0';
}

void encap_msg(char buf[], char op, const char username[], const char msg[])
{
    encap_usrname(buf, op, username);
    strcat(buf, " ");
    strcat(buf, msg);
}
