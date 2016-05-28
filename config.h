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

#ifndef __CONFIG_H__
#define __CONFIG_H__


/* Default system tick frequency. */
#ifndef SYS_TICK_FREQUENCY
#    define SYS_TICK_FREQUENCY 250
#endif 

/**
 * Build with assert.
 */
#ifndef CONFIG_ASSERT
#    define CONFIG_ASSERT               1
#endif

/**
 * Stack profiling.
 */
#ifndef CONFIG_PROFILE_STACK
#    define CONFIG_PROFILE_STACK        1
#endif

/**
 * Use a preemptive scheduler.
 */
#ifndef CONFIG_PREEMPTIVE_SCHEDULER
#    define CONFIG_PREEMPTIVE_SCHEDULER 0
#endif

/**
 * Start the monitor thread.
 */
#ifndef CONFIG_MONITOR_THREAD
#    define CONFIG_MONITOR_THREAD       1
#endif

#endif
