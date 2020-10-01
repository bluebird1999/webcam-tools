/*
 * time.c
 *
 *  Created on: Sep 16, 2020
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
//program header

//server header
#include "misc.h"
#include "log.h"
/*
 * static
 */
//variable

//function;



/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
unsigned int get_nowtime_ms(void)
{
    struct timeval tv;
    unsigned int time;
    gettimeofday(&tv, NULL);
    time = tv.tv_sec*1000  + tv.tv_usec / 1000;
    return time;
}

void get_nowtime_str(char *str)
{
    struct tm *ptm;
    long ts;

    ts = time(NULL);
    struct tm tt = {0};
    ptm = localtime_r(&ts, &tt);

	sprintf(str, "%04d%02d%02d%02d%02d%02d", ptm->tm_year+1900,
											 ptm->tm_mon+1,
											 ptm->tm_mday,
											 ptm->tm_hour,
											 ptm->tm_min,
											 ptm->tm_sec);

    return;
}
