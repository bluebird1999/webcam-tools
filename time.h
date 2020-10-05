/*
 * time.h
 *
 *  Created on: Sep 16, 2020
 *      Author: ning
 */

#ifndef TOOLS_TIME_H_
#define TOOLS_TIME_H_

/*
 * header
 */

/*
 * define
 */

/*
 * structure
 */
typedef struct time_struct_t{
    unsigned short  year;
    unsigned short  month;
    unsigned short  day;
    unsigned short  hour;
    unsigned short  minute;
    unsigned short  second;
} time_struct_t;

typedef struct scheduler_time_t{
    int start_hour;
    int start_min;
	int start_sec;
    int stop_hour;
    int stop_min;
	int stop_sec;
} scheduler_time_t;

typedef struct scheduler_slice_t {
	time_t start;
	time_t end;
};
/*
 * function
 */
unsigned int get_nowtime_ms(void);
void get_nowtime_str(char *str);

#endif /* TOOLS_TIME_H_ */
