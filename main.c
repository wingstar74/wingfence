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

static struct pin_driver_t pin7;
static struct pin_driver_t pin8;

static struct timer_t timer;
static struct adc_driver_t adc;
/* Define a shell command. */
static struct fs_command_t power_consumption_cmd;
static struct shell_args_t shell_args;


volatile static char bOk = 1;
volatile static uint16_t sampleHigh=0;
volatile static uint16_t sampleLow=0;
volatile static uint16_t sampleNone=0;

/**
 * The shell command callback for "/pc".
 */
static int power_consumption_cmd_cb(int argc,
                      const char *argv[],
                      chan_t *out_p,
                      chan_t *in_p,
                      void *arg_p,
                      void *call_arg_p)
{
	char bOkCopy;
	uint16_t sH;
	uint16_t sL;
	uint16_t sN;
	sys_lock();
	//bOkCopy = bOk;
	sH=sampleHigh;
	sL=sampleLow;
	sN=sampleNone;
	sys_unlock();
    /* Write the result to the shell output channel. */
    /*std_fprintf(out_p,
                FSTR("PulsOk=%s\r\n"),
                (bOkCopy ? "yes" : "no"));*/
	/*adc_convert(&adc,
                &s,
                1);*/
	std_fprintf(out_p,
                FSTR("High=%d\r\nLow=%d\r\nNone=%d\r\n"), sH, sL, sN);
				
	 
    return (0);
}

/**
 * Timer callback called in interrupt context.
 */
static void timer_callback(void *arg_p)
{
	static char pos = 0;
	
	
	if (pos==1) {
		uint16_t sample;
		adc_convert_isr(&adc, &sample);
		sampleNone=sample;
		
		pin_write(&pin7, 1);
		pin_write(&pin8, 0);
		time_sleep(250);
		adc_convert_isr(&adc, &sample);
		sampleHigh=sample;
		
		pin_write(&pin7, 0);
		pin_write(&pin8, 1);
		time_sleep(250);
		adc_convert_isr(&adc, &sample);
		sampleLow=sample;
		
		//pin_write(&pin7, 0);
		pin_write(&pin8, 0);
		
		//perform diagnostics
		bOk=(bOk ? 0 : 1);
		
		pin_toggle(&led);
		//csample=sample;
	}
	pos++;
	if (pos>=2)
		pos=0;
}

int main()
{
	char qinbuf[32];
    struct uart_driver_t uart;
	struct time_t timeout;

    /* Start the system. */
    sys_start();

    /* Initialize the UART. */
    uart_module_init();
    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    /* Set standard output to the UART. */
    sys_set_stdout(&uart.chout);
	
	/* Initialize the LED pin as output and set its value to 1. */
    pin_init(&led, &pin_led_dev, PIN_OUTPUT);
    pin_write(&led, 1);
	
	pin_init(&pin7, &pin_d7_dev, PIN_OUTPUT);
	pin_write(&pin7, 0);
	
	pin_init(&pin8, &pin_d8_dev, PIN_OUTPUT);
	pin_write(&pin8, 0);

	adc_init(&adc,
             &adc_0_dev,
             &pin_a0_dev,
             ADC_REFERENCE_VCC,
             1);
	
	timeout.seconds = 0;
    timeout.nanoseconds = 4000000;
	//timeout.nanoseconds = 100000000;	

	/* Register a shell command. */
    fs_command_init(&power_consumption_cmd, FSTR("/pc"), power_consumption_cmd_cb, NULL);
    fs_command_register(&power_consumption_cmd);
	
	timer_init(&timer,
               &timeout,
               timer_callback,
               NULL,
               TIMER_PERIODIC);
			   
	timer_start(&timer);
	
	
	std_printf(FSTR("%d\r\n"), CONFIG_SYSTEM_TICK_FREQUENCY);
	/* Print the system information. */
    std_printf(sys_get_info());
	/*while (1) {
		std_printf(FSTR("Hello world!\n"));
		thrd_usleep(1000000);
	}*/
	/* Call the shell main function. */
    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_main(&shell_args);

	
	return (0); 
} 