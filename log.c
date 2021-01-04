/*
 * misc.c
 *
 *  Created on: Aug 27, 2020
 *      Author: ning
 */


/*
 * header
 */
//system header
#include <stdio.h>
#include <pthread.h>
#include <syscall.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <malloc.h>
#ifdef DMALLOC_ENABLE
#include <dmalloc.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
//program header
#include "../manager/global_interface.h"
//server header
#include "log.h"
#include "time.h"
/*
 * static
 */
//variable

//function;
static void log_wrapper(const char* format, va_list args_list, char *file, int line, int level);

/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
static void log_wrapper(const char* format, va_list args_list, char *file, int line, int level)
{
	char timestr[32];
	time_get_now_str_format(timestr);
	printf("[%s]", timestr);
	if( _config_.debug_level > DEBUG_SERIOUS )
		printf("[%s:%d]", file+3,line);
	switch(level){
		case DEBUG_SERIOUS:
			printf("[S] ");
			break;
		case DEBUG_WARNING:
			printf("[W] ");
			break;
		case DEBUG_INFO:
			printf("[I] ");
			break;
		case DEBUG_VERBOSE:
			printf("[V] ");
			break;
	}
    vprintf(format, args_list);
    printf("\n");
}

int log_new(char* file, int line, int level, const char* format, ...)
{
	int ret = 0;
    va_list marker;
    if( _config_.debug_level>=level ) {
    	va_start(marker, format);
    	log_wrapper(format, marker, file, line, level);
    	va_end(marker);
    }
    return ret;
}
