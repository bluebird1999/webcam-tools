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
#define __USE_XOPEN
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
//program header

//server header
#include "time.h"
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
unsigned int time_get_now_ms(void)
{
    struct timeval tv;
    unsigned int time;
    gettimeofday(&tv, NULL);
    time = tv.tv_sec*1000  + tv.tv_usec / 1000;
    return time;
}

void time_get_now_str(char *str)
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

long long int time_date_to_stamp(char *date)
{
	struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));
	strptime(date,"%Y%m%d%H%M%S",tmp_time);
	time_t t = mktime(tmp_time);
	free(tmp_time);
	return t;
}

long long int time_get_now_stamp(void)
{
    struct timeval tv;
    unsigned long long int time;
    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    return time;
}

int time_stamp_to_date(long long int stamp, char *dd)
{
	struct tm *tmp_time = localtime(&stamp);
//	strftime(dd, 32, "%04Y%02m%02d%02H%02M%02S", tmp_time);
	sprintf(dd, "%04d%02d%02d%02d%02d%02d", tmp_time->tm_year+1900,
			tmp_time->tm_mon+1,
			tmp_time->tm_mday,
			tmp_time->tm_hour,
			tmp_time->tm_min,
			tmp_time->tm_sec);
	return 0;
}
