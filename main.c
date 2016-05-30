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

static struct pin_driver_t led;
static struct timer_t timer;
static struct adc_driver_t adc;


/**
 * Timer callback called in interrupt context.
 */
static void timer_callback(void *arg_p)
{
	static char pos = 0;
	
	
	if (pos==1) {
		uint16_t sample;
		adc_convert_isr(&adc, &sample);
		/* Toggle the LED on/off. */
		pin_toggle(&led); //Simulate +1
		time_sleep(250);
		adc_convert_isr(&adc, &sample);
		pin_toggle(&led); //Simulate -1
		time_sleep(250);
		adc_convert_isr(&adc, &sample);
		pin_toggle(&led); //Simulate 0 (none)
	}
	pos++;
	if (pos>=2)
		pos=0;
}

int main()
{
    struct uart_driver_t uart;
	struct time_t timeout;

    /* Start the system. */
    sys_start();

    /* Initialize the UART. */
    uart_module_init();
    uart_init(&uart, &uart_device[0], 38400, NULL, 0);
    uart_start(&uart);

    /* Set standard output to the UART. */
    sys_set_stdout(&uart.chout);
	
	/* Initialize the LED pin as output and set its value to 1. */
    pin_init(&led, &pin_led_dev, PIN_OUTPUT);
    pin_write(&led, 1);

	adc_init(&adc,
             &adc_0_dev,
             &pin_a0_dev,
             ADC_REFERENCE_VCC,
             10);
	
	timeout.seconds = 0;
    timeout.nanoseconds = 4000000;
	//timeout.nanoseconds = 100000000;	

	timer_init(&timer,
               &timeout,
               timer_callback,
               NULL,
               TIMER_PERIODIC);
			   
	timer_start(&timer);
	
	
	std_printf(FSTR("%d\r\n"), CONFIG_SYSTEM_TICK_FREQUENCY);
	while (1) {
		std_printf(FSTR("Hello world!\n"));
		thrd_usleep(1000000);
	}
	
	return (0); 
} 