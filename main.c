/** 
   wingfence
   Copyright (C) 2016  Björn Lind

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
   
   This file is part of wingfence
   
*/
   
#include "simba.h"

int main()
{
    struct uart_driver_t uart;

    /* Start the system. */
    sys_start();

    /* Initialize the UART. */
    uart_module_init();
    uart_init(&uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&uart);

    /* Set standard output to the UART. */
    sys_set_stdout(&uart.chout);

	while (1) {
		std_printf(FSTR("Hello world!\n"));
		thrd_usleep(1000000); 
	}
	
	return (0); 
} 